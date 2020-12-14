//
// Created by 17616 on 2020/12/14.
//

#include <sstream>
#include "prjMDK.h"
using namespace std;
using namespace pugi;
bool prjMDK::detect(const std::string &file) {
    if (file.find(".uvproj") != string::npos) {
        return true;
    }
    return false;
}
prjMDK::prjMDK(std::string file) {
    definedsymbols.insert("__CC_ARM");
    if (!detect(file)) {
        throw std::invalid_argument("project file path =" + file);
    }
    int a = file.find_last_of('\\');
    int b = file.find_last_of('/');
//    int c = path.find_last_of('.');
    a = _Max_value(a, b);
    path = file.substr(0, a + 1);//包含最后的那个'/'
    LOG_INFO << path;
    uvproj = file;
    xml_document doc;
    if (doc.load_file(uvproj.c_str())) {
        for (auto i:doc.child("Project").child("Targets").child("Target").child("TargetName")) {
            LOG_INFO << i.value();
            prj_name = i.value();
        }
    }

}
int prjMDK::FindDefinedsymbols() {
    xml_document doc;
    string define;
    string inc;
    if (doc.load_file(uvproj.c_str())) {
        xpath_node_set def = doc.select_nodes("//Target/TargetOption/TargetArmAds/Cads/VariousControls");
        for (auto i:def) {
            define = i.node().child("Define").text().get();
            inc = i.node().child("IncludePath").text().get();
            LOG_INFO << define;
            LOG_INFO << inc;
            stringstream ss;
            replace_str(define, ",", " ");
            replace_str(inc, ";", " ");
            ss = stringstream(define);
            while (ss){
                ss>>define;
                definedsymbols.insert(define);
            }
            ss = stringstream(inc);
            while (ss){
                ss>>inc;
                includePaths.insert(inc);
            }
        }
    }
    return 0;
}
int prjMDK::FindIncludePaths() {
    //已经在FindDefinedsymbols实现了
    return 0;
}
int prjMDK::FindSourseItems() {
    xml_document doc;
    string str;
    if (doc.load_file(uvproj.c_str())) {
        xpath_node_set def = doc.select_nodes("//Files/File/FilePath");
        for (auto i:def) {
            str = i.node().text().get();
            LOG_INFO << str;
            srcItems.insert(str);
        }
    }
    return 0;
}
