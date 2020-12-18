#include <iostream>
#include <vector>
int main(int argc, char *argv[]) {
    //将c程序参数转换为c++数组
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    //适配直接输文件的情况
    if (args.size() == 2) {
        args[1] = "--file=" + args[1];
    }
    extern int vspg(const std::vector<std::string> &_args);
    return vspg(args);
}
