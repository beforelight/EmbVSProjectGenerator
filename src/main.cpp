﻿#include "cmdline.h"
#include <iostream>
#include <map>
#include <functional>
#include "prj.h"
#include "pg.h"
#include "pgVS.h"
#include "pgCmake.h"
#include "pgVSCode.h"
#include "NanoLog.hpp"
using namespace std;
string path_exe;//包括最后的/，是程序上上一级绝对目录
int main(int argc, char *argv[]) {
    //获取程序的路径，便于访问资源文件
    path_exe = _pgmptr;
    REPLACE_CHAR(path_exe);
    if (path_exe.find_last_of("/") != string::npos) {
        path_exe = path_exe.substr(0, path_exe.find_last_of("/"));//exe目录
    }
    if (path_exe.find_last_of("/") != string::npos) {
        path_exe = path_exe.substr(0, path_exe.find_last_of("/") + 1);//+1保留最后的那个/ exe目录上一级
    }
    //转换为c++数组
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    //设置解析器
    cmdline::parser ps;
    ps.add<string>("file", 'f', "input file about iar\\keil\\eclipse project",
                   true, "");
    ps.add<string>("type", 't', "output project files type like vs/vscode/cmake/all",
                   false, "all",
                   cmdline::oneof<string>("vs", "vscode", "cmake", "all"));
    //适配直接输文件的情况
    if (args.size() == 2) {
        args[1] = "--file=" + args[1];
    }
    //解析指令
    ps.parse_check(args);
    //创建log所在的文件夹
    filesystem::path log_path("c:/temp/");
    if (!filesystem::exists(log_path)) {
        filesystem::create_directory(log_path);
    }
    //初始化log
    nanolog::initialize(nanolog::NonGuaranteedLogger(10), log_path.string(), "vspg_log", 1);
    try {
        //从工程文件中读取到内存
        prj_ptr ptr;
        ptr.Load(ps.get<string>("file"));
        ptr->Find();

        //写回到新的工程文件
        auto vs_op = [&] { pgvs(ptr, path_exe).Generate(); };
        auto vscode_op = [&] { pgVSCode(ptr, path_exe).Generate(); };
        auto cmake_op = [&] { pgCmake(ptr, path_exe).Generate(); };
        if (ps.get<string>("type") == "vs") { vs_op(); }
        else if (ps.get<string>("type") == "vscode") { vscode_op(); }
        else if (ps.get<string>("type") == "cmake") { cmake_op(); }
        else if (ps.get<string>("type") == "all") {
            vs_op();
            vscode_op();
            cmake_op();
        }
    }
    catch (const char *msg) {
        cout << msg << endl;
        cout<<"******press any key to continues******";
        getchar();
        return -1;
    }
    catch (const std::exception &e) {
        cout << e.what() << endl;
        cout<<"******press any key to continues******";
        getchar();
        return -1;
    }
    return 0;
}
