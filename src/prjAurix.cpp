#include "prjAurix.h"
using namespace pugi;
using namespace std;
prj_ptr::login loginAurix(prjAurix::detect, [](const string &file) { return new prjAurix(file); });
prjAurix::prjAurix(const std::string &file) {
    if (!detect(file)) {
        throw ERROR("not support");
    }
//    definedsymbols.insert("__GNUC__");
    definedsymbols.insert("__TASKING__");
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
    for (auto i: doc.child("projectDescription").child("name")) {
        LOG_INFO << i.value();
        prjName = i.value();
    }
    if (!doc.load_file(cproject.c_str())) {
        throw ERROR("fail to open the file");
    }
}

int prjAurix::FindDefinedsymbols() {
    xpath_node_set nodeSet = doc.select_nodes("//tool/option");
    for (xpath_node i: nodeSet) {
        if (string(i.node().attribute("id").value())
                    .find("com.infineon.aurix.buildsystem.managed.c.compiler.tasking.preprocessor.definedSymbols")
            != string::npos) {
            LOG_INFO << i.node().attribute("id").value();
            for (auto j: i.node()) {
                LOG_INFO << j.attribute("value").value();
                definedsymbols.insert(j.attribute("value").value());
            }
        }
    }
    return 0;
}
int prjAurix::FindIncludePaths() {
//    找到workspace_loc:/HITSIC_TC264DA_ADS
    string HITSIC_TC264DA_ADS;
    xpath_node_set nodeSet = doc.select_nodes("//toolChain/builder");
    for (xpath_node i: nodeSet) {
        if (string(i.node().attribute("id").value()).find(
                "com.infineon.aurix.buildsystem.managed.tasking.builder") != string::npos) {
            LOG_INFO << i.node().attribute("buildPath").value();
            string buf = i.node().attribute("buildPath").value();
            int a = buf.find_first_of('/');
            int b = buf.find_last_of('}');
            HITSIC_TC264DA_ADS = buf.substr(a, b - a);
        }
    }


    nodeSet = doc.select_nodes("//tool/option");
    for (xpath_node i: nodeSet) {
        if (string(i.node().attribute("id").value())
                    .find("com.infineon.aurix.buildsystem.managed.c.compiler.tasking.include")
            != string::npos) {
            LOG_INFO << i.node().attribute("id").value();
            for (auto j: i.node()) {
                LOG_INFO << j.attribute("value").value();
                string buf = j.attribute("value").value();
                replace_str(buf, HITSIC_TC264DA_ADS, "");
                replace_str(buf, "&quot;", "");
                replace_str(buf, "${ProjDirPath}/", "");
                replace_str(buf, "\"", "");
                includePaths.insert(buf);
            }
        }
    }
    return 0;
}
int prjAurix::FindSourseItems() {
//    set<string> sourceEntries;
//    xpath_node_set nodeSet = doc.select_nodes("//configuration/sourceEntries/entry");
//    for (xpath_node i:nodeSet) {
//        LOG_INFO << i.node().attribute("name").value();
//        sourceEntries.insert(i.node().attribute("name").value());
//    }
//    vector<string> files;
//    for (const auto& i:sourceEntries) {
//        listFiles(pathPrj + i, files);
//    }
//
//    for (const auto& i:files) {
//        LOG_INFO << i;
//        srcItems.insert(i.substr(pathPrj.size()));
//    }
    vector<string> files;
    listFiles(pathPrj, files);
//    for (const auto &i:sourceEntries) {
//        listFiles(pathPrj + i, files);
//    }

    for (const auto &i: files) {
        LOG_INFO << i;
        srcItems.insert(i.substr(pathPrj.size()));
    }

//    下面几行是直接通过文件路径添加includePath
//    for (auto i: srcItems) {
//        replace_str(i, "\\", "/");
//        if (i.find(".h") != std::string::npos || i.find(".hpp") != std::string::npos) {
//            includePaths.insert(i.substr(0, i.find_last_of('/')));
//        }
//    }
    return 0;
}
bool prjAurix::detect(const std::string &file) {
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
        for (auto i: doc.select_nodes("/projectDescription/natures/nature")) {
            std::string str = i.node().text().get();
            LOG_INFO << str;
            if (str.find("infineon.aurix") != std::string::npos) {
                return true;
            }
        }
    }
    return false;
}
