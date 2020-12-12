#include "cmdline.h"
#include <iostream>
#include <map>
#include <functional>
#include "prj.h"
#include "pg.h"
#include "pgVS.h"
#include "NanoLog.hpp"
using namespace std;
string path_exe;//包括最后的/
int main(int argc, char *argv[]) {
    //初始化变量和解析器
    path_exe = argv[0];
    int a = replace_str(path_exe, "\\", "/").find_last_of('/');
    path_exe = path_exe.substr(0, a + 1);
    nanolog::initialize(nanolog::NonGuaranteedLogger(10), path_exe.c_str(), "log", 1);
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    cmdline::parser ps;
    ps.add<string>("file", 'f', "input file about iar\\keil\\eclipse project",
                   true, "");
    ps.add<string>("type", 't', "output project files type",
                   false, "all",
                   cmdline::oneof<string>("vs", "vscode", "cmake", "all"));
    //适配直接输文件的情况
    if (args.size() == 2) {
        args[1] = "--file=" + args[1];
    }
    try {
        //解析指令
        ps.parse_check(args);

        //从工程文件中读取到内存
        prj_ptr ptr;
        ptr.Load(ps.get<string>("file"));
        ptr->Find();

        //写回到新的工程文件
        auto vs_op = [&] { pgvs(ptr, path_exe).Generate(); };
        auto vscode_op = [&] {  };
        auto cmake_op = [&] {  };
        if(ps.get<string>("type")=="vs"){vs_op();}
        else if(ps.get<string>("type")=="vscode"){vscode_op();}
        else if(ps.get<string>("type")=="cmake"){cmake_op();}
        else if(ps.get<string>("type")=="all"){vs_op();vscode_op();cmake_op();}
    }
    catch (const char *msg) {
        cout << msg << endl;
    }
    catch (const std::exception &e) {
        cout << e.what() << endl;
    }
    return 0;
}
