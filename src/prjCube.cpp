//
// Created by 17616 on 2020/12/8.
//

#include "prjCube.h"
#include <iostream>
#include <dirent.h>
using namespace pugi;
using namespace std;
prjCube::prjCube(std::string file) {

    if (file.find(".project") != std::string::npos) {
    } else if (file.find(".cproject") != std::string::npos) {
    } else {
        throw std::invalid_argument("project file path =" + file);
    }

    int a = file.find_last_of('\\');
    int b = file.find_last_of('/');
//    int c = path.find_last_of('.');
    a = _Max_value(a, b);
    path = file.substr(0, a + 1);//包含最后的那个'/'
    LOG_INFO << "输入工程的目录在" << path;
    project = path + ".project";
    cproject = path + ".cproject";
    xml_document doc;
    if (doc.load_file(project.c_str())) {
        for (auto i:doc.child("projectDescription").child("name")) {
            LOG_INFO << i.value();
            prj_name = i.value();
        }
    }
}


int prjCube::FindDefinedsymbols() {
    xml_document doc;
    if (doc.load_file(cproject.c_str())) {
        xpath_node_set def = doc.select_nodes("//tool/option");
        for (xpath_node i:def) {
            if (string(i.node().attribute("id").value())
                        .find("compiler.option.definedsymbols")
                != string::npos) {
                LOG_INFO << i.node().attribute("id").value();
                for (auto j:i.node()) {
                    LOG_INFO << j.attribute("value").value();
                    definedsymbols.insert(j.attribute("value").value());
                }
            }
        }
    }
    return 0;
}
int prjCube::FindIncludePaths() {
    xml_document doc;
    if (doc.load_file(cproject.c_str())) {
        xpath_node_set def = doc.select_nodes("//tool/option");
        for (xpath_node i:def) {
            if (string(i.node().attribute("id").value())
                        .find("compiler.option.includepaths")
                != string::npos) {
                LOG_INFO << i.node().attribute("id").value();
                for (auto j:i.node()) {
                    LOG_INFO << j.attribute("value").value();
                    string buf = j.attribute("value").value();
                    includePaths.insert(replace_str(buf, "../", ""));
                }
            }
        }
    }
    return 0;
}
int prjCube::FindSourseItems() {

    xml_document doc;
    set<string> sourceEntries;
    if (doc.load_file(cproject.c_str())) {
        xpath_node_set def = doc.select_nodes("//configuration/sourceEntries/entry");
        for (xpath_node i:def) {
            LOG_INFO << i.node().attribute("name").value();
            sourceEntries.insert(i.node().attribute("name").value());
        }
    }
    vector<string> files;
    for (auto i:sourceEntries) {
        listFiles((path + i).c_str(), files);
    }

    for (auto i:files) {
        LOG_INFO << i;
        srcItems.insert(i.substr(path.size()));
    }
    return 0;
}
