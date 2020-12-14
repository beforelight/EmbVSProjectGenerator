#include "prjMDK.h"
using namespace std;
using namespace pugi;
prj_ptr::login loginMDK(prjMDK::detect, [](const string & file){return new prjMDK(file);});
bool prjMDK::detect(const std::string &file) {
    if (file.find(".uvproj") != string::npos) {
        return true;
    }
    return false;
}
prjMDK::prjMDK(const std::string &file) {
    if(!detect(file)){
        throw ERROR("not support");
    }
    definedsymbols.insert("__CC_ARM");
    int a = file.find_last_of('\\');
    int b = file.find_last_of('/');
    a = _Max_value(a, b);
    path = file.substr(0, a + 1);//包含最后的那个'/'
    LOG_INFO << path;
    uvproj = file;
    if (!doc.load_file(uvproj.c_str())) {
        throw ERROR("fail to open the file");
    }
    for (auto i:doc.child("Project").child("Targets").child("Target").child("TargetName")) {
        LOG_INFO << i.value();
        prj_name = i.value();
    }

}
int prjMDK::FindDefinedsymbols() {
    string define;
    string inc;
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
        while (ss) {
            ss >> define;
            definedsymbols.insert(define);
        }
        ss = stringstream(inc);
        while (ss) {
            ss >> inc;
            includePaths.insert(inc);
        }
    }
    return 0;
}
int prjMDK::FindIncludePaths() {
    //已经在FindDefinedsymbols实现了
    return 0;
}
int prjMDK::FindSourseItems() {
    string str;
    xpath_node_set def = doc.select_nodes("//Files/File/FilePath");
    for (auto i:def) {
        str = i.node().text().get();
        LOG_INFO << str;
        srcItems.insert(str);
    }
    return 0;
}
