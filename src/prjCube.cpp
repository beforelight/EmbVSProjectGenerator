//
// Created by 17616 on 2020/12/8.
//

#include "prjCube.h"
#include <iostream>
#include <dirent.h>
using namespace pugi;
using namespace std;
prjCube::prjCube(std::string file) {
    std::string path(file);

    if (path.find(".project") != std::string::npos) {
    } else if (path.find(".cproject") != std::string::npos) {
    } else {
        throw std::invalid_argument("project file path =" + file);
    }

    int a = path.find_last_of('\\');
    int b = path.find_last_of('/');
    int c = path.find_last_of('.');
    a = _Max_value(a, b);
    path = path.substr(0, a + 1);//包含最后的那个'/'

    project = path + ".project";
    cproject = path + ".cproject";
}


int prjCube::FindDefinedsymbols() {
    xml_document doc;
    if (doc.load_file(cproject.c_str())) {
        xpath_node_set def = doc.select_nodes("//tool/option");
        for (xpath_node i:def) {
            if (string(i.node().attribute("id").value())
                        .find("compiler.option.definedsymbols")
                != string::npos) {
//                cout<<i.node().attribute("id").value();
                for (auto j:i.node()) {
//                    cout<<j.attribute("value").value();
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
//                cout<<i.node().attribute("id").value();
                for (auto j:i.node()) {
//                    cout<<j.attribute("value").value();
                    string buf = j.attribute("value").value();
                    definedsymbols.insert(replace_str(buf, "../", ""));
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
//                cout<<i.node().attribute("name").value();
            sourceEntries.insert(i.node().attribute("name").value());
        }
    }

    DIR *dirp;
    struct dirent *dp;
    for(auto dir_name: sourceEntries){
        dirp = opendir(dir_name.c_str());
        while ((dp = readdir(dirp)) != NULL) {
//            v.push_back(std::string(dp->d_name));
//                dp.
        }
        (void) closedir(dirp);
    }

    return 0;
    return 0;
}
