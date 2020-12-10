#include "cmdline.h"
#include <iostream>
#include "prj.h"
using namespace std;
int main(int argc, char *argv[]) {
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    cmdline::parser ps;
    ps.add<string>("file", 'f', "input file about iar\\keil\\eclipse project",
                   true, "");
    ps.add<string>("type", 't', "output project files type",
                   false, "all", cmdline::oneof<string>("vs", "vscode", "cmake", "all"));

    if (args.size() == 2) {
        args[1] = "--file=" + args[1];
    }
    ps.parse_check(args);
    prj_ptr ptr;
    ptr.Load(ps.get<string>("file"));
    ptr->Find();
    return 0;
}
