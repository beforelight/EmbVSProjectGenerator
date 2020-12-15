#include <iostream>
#include "cmdline.h"
#include "prj.h"
#include "pg.h"
#include "NanoLog.hpp"
using namespace std;
string pathExe;//包括最后的/，是程序上上一级绝对目录
int main(int argc, char *argv[]) {
    //获取程序的路径，便于访问资源文件
    pathExe = _pgmptr;
    REPLACE_CHAR(pathExe);
    if (pathExe.find_last_of('/') != string::npos) {
        pathExe = pathExe.substr(0, pathExe.find_last_of('/'));//exe目录
    }
    if (pathExe.find_last_of('/') != string::npos) {
        pathExe = pathExe.substr(0, pathExe.find_last_of('/') + 1);//+1保留最后的那个/ exe目录上一级
    }
    //将c程序参数转换为c++数组
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    //设置参数解析器
    cmdline::parser cmdlineParser;
    cmdlineParser.add<string>("file", 'f', "input:project file about iar/keil/eclipse",
                              true, "");
    cmdlineParser.add<string>("type", 't', "output:project type like vs/vscode/cmake/all",
                              false, "all", pg::oneof_string());
    //适配直接输文件的情况
    if (args.size() == 2) {
        args[1] = "--file=" + args[1];
    }
    //解析指令
    cmdlineParser.parse_check(args);
    //初始化log
#ifdef WIN32
    //创建log所在的文件夹
    filesystem::path pathLog("c:/temp/");
    if (!filesystem::exists(pathLog)) {
        filesystem::create_directory(pathLog);
    }
    nanolog::initialize(nanolog::NonGuaranteedLogger(10), pathLog.string(), "vspg_log", 1);
#else
    nanolog::initialize(nanolog::NonGuaranteedLogger(10), "/tmp/", "vspg_log", 1);
#endif
    try {
        //从工程文件中读取到内存
        prj_ptr ptr;
        ptr.Load(cmdlineParser.get<string>("file"));
        ptr->Find();
        //写回到新的工程文件
        pg::Generate(cmdlineParser.get<string>("type"), ptr, pathExe);
    }
    catch (const char *msg) {
        cout << msg << endl;
        return -1;
    }
    catch (const std::exception &e) {
        cout << e.what() << endl;
        return -1;
    }
    cout << "done!" << endl;
    return 0;
}
