//
// Created by 17616 on 2020/12/12.
//

#include "pgCmake.h"
#include <fstream>
#include "error.h"
using namespace std;
int pgCmake::Generate() {
    ofstream cmake(prjPtr->path + "CMakeLists.txt");
    if (!cmake.is_open()) {
        throw ERROR("fail to open the file");
    }
    cmake << "cmake_minimum_required(VERSION 3.15)" << endl;
    cmake << "project(" << prjPtr->prj_name << ")" << endl;
    cmake << "set(PRJDIR  " << REPLACE_CHAR(prjPtr->path) << ")" << endl;
    for (auto i:prjPtr->definedsymbols) {
        cmake << "add_definitions(-D" << i << ")" << endl;
    }

    for (auto i:prjPtr->includePaths) {
        cmake << "include_directories(" << REPLACE_CHAR(i) << ")" << endl;
    }
    cmake << "file(GLOB_RECURSE SOURCES ";
    for (auto i:prjPtr->srcGroup) {
        cmake << '\t' << REPLACE_CHAR(i) << "/*.*";
    }
    cmake << ')' << endl;
    cmake << "add_executable(" << prjPtr->prj_name << " ${SOURCES})" << endl;
    return 0;
}
