#include "prjMCUX.h"
using namespace pugi;
using namespace std;
prj_ptr::login loginMCUX(prjMCUX::detect, [](const string & file){return new prjMCUX(file);});
bool prjMCUX::detect(const std::string& file) {
    if (file.find(".project") != std::string::npos) {
    } else if (file.find(".cproject") != std::string::npos) {
    } else {
        return false;
    }
    int a = file.find_last_of('\\');
    int b = file.find_last_of('/');
//    int c = path.find_last_of('.');
    a = _Max_value(a, b);
    std::string path = file.substr(0, a + 1);//包含最后的那个'/'
    LOG_INFO << path;
    std::string project = path + ".project";
    xml_document doc;
    if (doc.load_file(project.c_str())) {
        for (auto i:doc.select_nodes("/projectDescription/natures/nature")) {
            std::string str = i.node().text().get();
            LOG_INFO << str;
            if (str.find("mcuxpresso") != std::string::npos) {
                return true;
            }
        }
    }
    return false;
}
prjMCUX::prjMCUX(const std::string &file) {
    if(!detect(file)){
        throw ERROR("not support");
    }
    definedsymbols.insert("__GNUC__");
    int a = file.find_last_of('\\');
    int b = file.find_last_of('/');
//    int c = path.find_last_of('.');
    a = _Max_value(a, b);
    path = file.substr(0, a + 1);//包含最后的那个'/'
    LOG_INFO << path;
    project = path + ".project";
    cproject = path + ".cproject";
    if (!doc.load_file(project.c_str())) {
        throw ERROR("fail to open the file");
    }
    for (auto i:doc.child("projectDescription").child("name")) {
        LOG_INFO << i.value();
        prj_name = i.value();
    }
    if (!doc.load_file(cproject.c_str())) {
        throw ERROR("fail to open the file");
    }
}
int prjMCUX::FindDefinedsymbols() {
    xpath_node_set def = doc.select_nodes("//tool/option");
    for (xpath_node i:def) {
        if (string(i.node().attribute("id").value())
                    .find("compiler.option.preprocessor.def.symbols")
            != string::npos) {
            LOG_INFO << i.node().attribute("id").value();
            for (auto j:i.node()) {
                LOG_INFO << j.attribute("value").value();
                definedsymbols.insert(j.attribute("value").value());
            }
        }
    }
    return 0;
}
int prjMCUX::FindIncludePaths() {
    xpath_node_set def = doc.select_nodes("//tool/option");
    for (xpath_node i:def) {
        if (string(i.node().attribute("id").value())
                    .find("compiler.option.include.paths")
            != string::npos) {
            LOG_INFO << i.node().attribute("id").value();
            for (auto j:i.node()) {
                LOG_INFO << j.attribute("value").value();
                string str = j.attribute("value").value();
                if (str.find('$') != string::npos) {
//                        replace_str(str, "&quot;", "");
                    replace_str(str, "\"${workspace_loc:/${ProjName}/", "");
                    replace_str(str, "}\"", "");
                }
                replace_str(str, "../", "");
                includePaths.insert(str);
            }
        }
    }
    return 0;
}

int prjMCUX::FindSourseItems() {
    set<string> sourceEntries;
    xpath_node_set def = doc.select_nodes("//configuration/sourceEntries/entry");
    for (xpath_node i:def) {
        LOG_INFO << i.node().attribute("name").value();
        sourceEntries.insert(i.node().attribute("name").value());
    }
    vector<string> files;
    for (const auto& i:sourceEntries) {
        listFiles(path + i, files);
    }

    for (const auto& i:files) {
        LOG_INFO << i;
        srcItems.insert(i.substr(path.size()));
    }
    return 0;
}
