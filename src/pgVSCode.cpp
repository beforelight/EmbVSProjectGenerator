#include <fstream>
#include <sstream>
#include "pgVSCode.h"
#include "CJsonObject.hpp"
#include "error.h"
#include <filesystem>
#include "resource.hpp"

pg::login loginCode("vscode",
                  [](prj_ptr &ptr, const std::string &exe_path) {
                      return new pgVSCode(ptr, exe_path);
                  }
);
int pgVSCode::Generate() {
    std::stringbuf fin;
#if defined(__linux__)
    fin.sputn((const char *) c_cpp_properties_linux_json, sizeof(c_cpp_properties_linux_json));
#else
    fin.sputn((const char *) c_cpp_properties_win32_json, sizeof(c_cpp_properties_win32_json));
#endif
    std::filesystem::path fout_path(prjPtr->pathPrj + ".vscode/");
    if (!std::filesystem::exists(fout_path)) {
        std::filesystem::create_directory(fout_path);
    }

    std::ofstream fout(prjPtr->pathPrj + ".vscode/" + "c_cpp_properties.json");
    std::stringstream ssContent;
    neb::CJsonObject oJson;

    if (!fout.is_open()) {
        throw ERROR("fail to open the file");
    }
    ssContent << fin.str();
    if (!oJson.Parse(ssContent.str())) {
        throw ERROR("parse json error");
    }
    for (const auto& i:prjPtr->definedsymbols) {
        oJson["configurations"][0]["defines"].Add(i);
    }

    for (const auto& i:prjPtr->includePaths) {
        std::string str = "${workspaceFolder}/";
        str += i;
        oJson["configurations"][0]["includePath"].Add(str);
    }
    fout << oJson.ToFormattedString();
    return 0;
}
