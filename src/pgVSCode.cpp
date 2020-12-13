//
// Created by 17616 on 2020/12/12.
//

#include <fstream>
#include <sstream>
#include "pgVSCode.h"
#include "CJsonObject.hpp"
#include "error.h"
#include "direct.h"
int pgVSCode::Generate() {
    std::ifstream fin;
#if defined(__linux__)
    fin.open(path_exe + resource + "c_cpp_properties_linux.json");
#else
    fin.open(path_exe + resource + "c_cpp_properties_win32.json");
#endif
    std::ofstream fout(prjPtr->path + ".vscode/" + "c_cpp_properties.json");
    std::stringstream ssContent;
    neb::CJsonObject oJson;
    if(!fout.is_open()){
        _mkdir((prjPtr->path + ".vscode/").c_str());
        fout.open(prjPtr->path + ".vscode/" + "c_cpp_properties.json");
    }
    if (!fin.is_open() ||
        !fout.is_open()) {
        throw ERROR("fail to open the file");
    }
    ssContent << fin.rdbuf();
    if (!oJson.Parse(ssContent.str())) {
        throw ERROR("parse json error");
    }
    fin.close();
    for (auto i:prjPtr->definedsymbols) {
        oJson["configurations"][0]["defines"].Add(i);
    }

    for (auto i:prjPtr->includePaths) {
        std::string str = "${workspaceFolder}/";
        str += i;
        oJson["configurations"][0]["includePath"].Add(str);
    }
    fout << oJson.ToFormattedString();
    return 0;
}
