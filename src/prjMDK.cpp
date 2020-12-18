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
    int length_a = file.find_last_of('\\');
    int length_b = file.find_last_of('/');
    int length_c = file.find_last_of('.');
    length_a = _Max_value(length_a, length_b);
    pathPrj = file.substr(0, length_a + 1);//包含最后的那个'/'
    LOG_INFO << pathPrj;
    uvproj = file;
    if (!doc.load_file(uvproj.c_str())) {
        throw ERROR("fail to open the file");
    }
    prjName = file.substr(length_a + 1, length_c - length_a - 1);
    LOG_INFO << prjName;

}
int prjMDK::FindDefinedsymbols() {
    string define;
    string inc;
    xpath_node_set nodeSet = doc.select_nodes("//Target/TargetOption/TargetArmAds/Cads/VariousControls");
    for (auto i:nodeSet) {
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
    xpath_node_set nodeSet = doc.select_nodes("//Files/File/FilePath");
    for (auto i:nodeSet) {
        str = i.node().text().get();
        LOG_INFO << str;
        srcItems.insert(str);
    }
    return 0;
}
