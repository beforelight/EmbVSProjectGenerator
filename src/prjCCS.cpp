#include "prjCCS.h"
using namespace pugi;
using namespace std;
prj_ptr::login loginCCS(prjCCS::detect, [](const string &file) { return new prjCCS(file); });
prjCCS::prjCCS(const std::string &file) {
    if (!detect(file)) {
        throw ERROR("not support");
    }
    definedsymbols.insert("__GNUC__");
    definedsymbols.insert("__TMS320C28XX__");
    int length_a = file.find_last_of('\\');
    int length_b = file.find_last_of('/');
    length_a = _Max_value(length_a, length_b);
    pathPrj = file.substr(0, length_a + 1);//包含最后的那个'/'
    LOG_INFO << pathPrj;
    project = pathPrj + ".project";
    cproject = pathPrj + ".cproject";
    if (!doc.load_file(project.c_str())) {
        throw ERROR("fail to open the file");
    }
    for (auto i:doc.child("projectDescription").child("name")) {
        LOG_INFO << i.value();
        prjName = i.value();
    }
    if (!doc.load_file(cproject.c_str())) {
        throw ERROR("fail to open the file");
    }
}

int prjCCS::FindDefinedsymbols() {
    xpath_node_set nodeSet = doc.select_nodes("//tool/option");
    for (xpath_node i:nodeSet) {
        if (string(i.node().attribute("id").value())
                    .find("compilerID.DEFINE")
            != string::npos) {
            LOG_INFO << i.node().attribute("id").value();
            for (auto j:i.node()) {
                LOG_INFO << j.attribute("value").value();
                if (std::string(j.attribute("value").value()).find('$') == std::string::npos) {
                    definedsymbols.insert(j.attribute("value").value());
                }
            }
        }
    }
    return 0;
}
int prjCCS::FindIncludePaths() {
//    xpath_node_set nodeSet = doc.select_nodes("//tool/option");
//    for (xpath_node i:nodeSet) {
//        if (string(i.node().attribute("id").value())
//                    .find("compilerID.INCLUDE_PATH")
//            != string::npos) {
//            LOG_INFO << i.node().attribute("id").value();
//            for (auto j:i.node()) {
//                LOG_INFO << j.attribute("value").value();
//                string buf = j.attribute("value").value();
//                replace_str(buf, "${PROJECT_ROOT}/", "");
//                replace_str(buf, "${C2000WARE_DLIB_ROOT}/", "");
//                replace_str(buf, "${CG_TOOL_ROOT}/", "");
//                replace_str(buf, "${COM_TI_C2000WARE_SOFTWARE_PACKAGE_INCLUDE_PATH}", "");
//
//                replace_str(buf, "${PROJECT_ROOT}", "");
//                replace_str(buf, "${C2000WARE_DLIB_ROOT}", "");
//                replace_str(buf, "${CG_TOOL_ROOT}", "");
//                replace_str(buf, "${COM_TI_C2000WARE_SOFTWARE_PACKAGE_INCLUDE_PATH}", "");
//                includePaths.insert(buf);
//            }
//        }
//    }
//    在FindSourseItems中实现

    return 0;
}
int prjCCS::FindSourseItems() {
    set<string> sourceEntries;
//    xpath_node_set nodeSet = doc.select_nodes("//configuration/sourceEntries/entry");
//    for (xpath_node i:nodeSet) {
//        LOG_INFO << i.node().attribute("name").value();
//        sourceEntries.insert(i.node().attribute("name").value());
//    }
    vector<string> files;
    listFiles(pathPrj, files);
//    for (const auto &i:sourceEntries) {
//        listFiles(pathPrj + i, files);
//    }

    for (const auto &i:files) {
        LOG_INFO << i;
        srcItems.insert(i.substr(pathPrj.size()));
    }
    for (auto i:srcItems) {
        replace_str(i, "\\", "/");
        if (i.find(".h") != std::string::npos || i.find(".hpp") != std::string::npos) {
            includePaths.insert(i.substr(0, i.find_last_of('/')));
        }
    }
    return 0;
}
bool prjCCS::detect(const std::string &file) {
    if (file.find(".project") != std::string::npos) {
    } else if (file.find(".cproject") != std::string::npos) {
    } else {
        return false;
    }
    int length_a = file.find_last_of('\\');
    int length_b = file.find_last_of('/');
    length_a = _Max_value(length_a, length_b);
    std::string path = file.substr(0, length_a + 1);//包含最后的那个'/'
    LOG_INFO << path;
    std::string project = path + ".project";
    xml_document doc;
    if (doc.load_file(project.c_str())) {
        for (auto i:doc.select_nodes("/projectDescription/natures/nature")) {
            std::string str = i.node().text().get();
            LOG_INFO << str;
            if (str.find("com.ti.ccstudio") != std::string::npos) {
                return true;
            }
        }
    }
    return false;
}