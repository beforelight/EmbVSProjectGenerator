#include "prjIAR.h"
using namespace std;
using namespace pugi;
prj_ptr::login loginIAR(prjIAR::detect, [](const string & file){return new prjIAR(file);});
bool prjIAR::detect(const std::string &file) {
    if (file.find(".ewp") != string::npos ||
        file.find(".ewd") != string::npos) {
        return true;
    }
    return false;
}
prjIAR::prjIAR(const std::string &file) {
    if(!detect(file)){
        throw ERROR("not support");
    }
    definedsymbols.insert("__ICCARM__");
    if (!detect(file)) {
        throw std::invalid_argument("project file path =" + file);
    }
    int length_a = file.find_last_of('\\');
    int length_b = file.find_last_of('/');
    length_a = _Max_value(length_a, length_b);
    pathPrj = file.substr(0, length_a + 1);//包含最后的那个'/'
    LOG_INFO << pathPrj;
    ewp = file.substr(0, file.find_last_of('.')) + ".ewp";
    LOG_INFO << ewp;
    if (!doc.load_file(ewp.c_str())) {
        throw ERROR("fail to open the file");
    }
    for (auto i:doc.child("project").child("configuration").child("name")) {
        LOG_INFO << i.value();
        prjName = i.value();
    }
}
int prjIAR::FindDefinedsymbols() {
    xpath_node_set nodeSet = doc.select_nodes("//configuration/settings/data/option");
    for (auto i:nodeSet) {
        LOG_INFO << i.node().child("name").text().get();
        if (string(i.node().child("name").text().get()) == "CCDefines") {
            for (auto j:i.node().select_nodes("state")) {
                LOG_INFO << j.node().text().get();
                definedsymbols.insert(j.node().text().get());
            }
        }
    }
    return 0;
}
int prjIAR::FindIncludePaths() {
    string str;
    xpath_node_set nodeSet = doc.select_nodes("//configuration/settings/data/option");
    for (auto i:nodeSet) {
        LOG_INFO << i.node().child("name").text().get();
        if (string(i.node().child("name").text().get()) == "CCIncludePath2") {
            for (auto j:i.node().select_nodes("state")) {
                str = j.node().text().get();
                REPLACE_CHAR(str);
                replace_str(str,"$PROJ_DIR$/","");
                LOG_INFO << str;
                includePaths.insert(str);
            }
        }
    }
    return 0;
}
int prjIAR::FindSourseItems() {
    string str;
    xpath_node_set nodeSet = doc.select_nodes("//group/file/name");
    for (auto i:nodeSet) {
        str = i.node().text().get();
        REPLACE_CHAR(str);
        replace_str(str,"$PROJ_DIR$/","");
        LOG_INFO << str;
        srcItems.insert(str);
    }
    return 0;
}
