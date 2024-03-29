﻿#include "pgCmake.h"
#include <fstream>
#include "error.h"
pg::login loginCmake("cmake",
                  [](prj_ptr &ptr, const std::string &exe_path) {
                      return new pgCmake(ptr, exe_path);
                  }
);
using namespace std;
int pgCmake::Generate() {
    ofstream cmake(prjPtr->pathPrj + "CMakeLists.txt");
    if (!cmake.is_open()) {
        throw ERROR("fail to open the file");
    }
    cmake << "cmake_minimum_required(VERSION 3.15)" << endl;
    cmake << "project(" << prjPtr->prjName << ")" << endl;
    for (const auto& i:prjPtr->definedsymbols) {
        if (!i.empty()) {
            cmake << "add_definitions(-D" << i << ")" << endl;
        }
    }

    for (auto i:prjPtr->includePaths) {
        cmake << "include_directories(" << REPLACE_CHAR(i) << ")" << endl;
    }
    cmake << "file(GLOB_RECURSE SOURCES ";
    for (auto i:prjPtr->srcGroups) {
        cmake << '\t' << REPLACE_CHAR(i) << "/*.c";
        cmake << '\t' << REPLACE_CHAR(i) << "/*.cpp";
    }
    cmake << ')' << endl;
    cmake << "add_executable(" << prjPtr->prjName << " ${SOURCES})" << endl;
    return 0;
}
