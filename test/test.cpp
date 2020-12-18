#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "prj.h"
using namespace std;

std::vector<std::string> args = {
        "vspg",
        "-f",
        "????",
        "-t",
        "all"
};

extern int vspg(const std::vector<std::string> &_args);
bool file_match(std::string file1, std::string file2);
void error_op();
void no_match_op();


int main(int argc, char *argv[]) {
    string pathExe;//包括最后的/，是程序上上一级绝对目录
    //获取程序的路径，便于访问资源文件
    char *pgmptr;
    _get_pgmptr(&pgmptr);
    pathExe = pgmptr;
    REPLACE_CHAR(pathExe);
    if (pathExe.find_last_of('/') != string::npos) {
        pathExe = pathExe.substr(0, pathExe.find_last_of('/'));//exe目录
    }
    if (pathExe.find_last_of('/') != string::npos) {
        pathExe = pathExe.substr(0, pathExe.find_last_of('/') + 1);//+1保留最后的那个/ exe目录上一级
    }


    //以MCUX为例子测试各个class pg
    cout << "********<<Start to test class pg>>********" << endl;
    args[2] = pathExe + "test/workspace/MCUXpressoIDE/.cproject";
    args[4] = "all";
    if (0 == vspg(args)) {
        cout << "********<<Compare result>>********" << endl;
        std::vector<std::string> Reacts = {
                pathExe + "test/workspace/MCUXpressoIDE/CMakeLists.txt",
//                pathExe + "test/workspace/MCUXpressoIDE/evkbimxrt1050_hello_world.sln",
//                pathExe + "test/workspace/MCUXpressoIDE/evkbimxrt1050_hello_world.vcxproj",
//                pathExe + "test/workspace/MCUXpressoIDE/evkbimxrt1050_hello_world.vcxproj.filters",
//                pathExe + "test/workspace/MCUXpressoIDE/evkbimxrt1050_hello_world.vcxproj.user",
                pathExe + "test/workspace/MCUXpressoIDE/.vscode/c_cpp_properties.json",
        };
        cout << "********<<Warning>>********" << endl;
        cout << "please check the \""
             << pathExe
             << "test/workspace/MCUXpressoIDE/evkbimxrt1050_hello_world.sln" << "\" yourself" << endl;
        std::vector<std::string> Answers = Reacts;
        for (int i = 0; i < Answers.size(); ++i) {
            Answers[i] += ".answer";
            if (!file_match(Reacts[i], Answers[i])) {
                no_match_op();
                return -1;
            }
        }
    } else {
        error_op();
        return -1;
    }

    //以Cmake为例子测试各个class prj
    cout << "********<<Start to test class prj>>********" << endl;
    args[4] = "cmake";
    std::vector<std::string> test_prj_path = {
            pathExe + "test/workspace/IAR/",
            pathExe + "test/workspace/Keil/",
            pathExe + "test/workspace/STM32CubeIDE/",
    };
    std::vector<std::string> test_prj_file{
            "hello_world.ewp",
            "hello_world.uvprojx",
            ".cproject"
    };
    for (int i = 0; i < test_prj_path.size(); ++i) {
        args[2] = test_prj_path[i] + test_prj_file[i];
        if (0 == vspg(args)) {
            if (!file_match(test_prj_path[i] + "CMakeLists.txt",
                            test_prj_path[i] + "CMakeLists.txt.answer")) {
                no_match_op();
                return -1;
            }
        } else {
            error_op();
            return -1;
        }
    }
    cout << "********<<Test end>>********" << endl;
}

void error_op() {
    cout << "Error when test vspg,the args are:" << endl;
    for (auto i:args) {
        cout << i << "  ";
    }
    cout << endl;
}

void no_match_op() {
    cout << "Some file don't match when test vspg,the args are:" << endl;
    for (auto i:args) {
        cout << i << "  ";
    }
    cout << endl;
}

bool file_match(std::string file1, std::string file2) {
    int line = 0;
    std::ifstream f1(file1);
    std::ifstream f2(file2);
    if (!f1.is_open() || !f2.is_open()) {
        cout << "fail to open the file" << endl;
        return false;
    }
    std::string s1;
    std::string s2;
    while (!f1.eof() && !f2.eof()) {
        std::getline(f1, s1);
        std::getline(f2, s2);
        if (s1 != s2) {
            cout << file1 << "&&\r\n" << file2 << ":Some difference in line " << line << endl;
            return false;
        }
        ++line;
    }
    return true;
}
