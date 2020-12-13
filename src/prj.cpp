//
// Created by 17616 on 2020/12/8.
//

#include "prj.h"
#include "prjCube.h"
#include "prjMCUX.h"
using namespace std;
using namespace std::filesystem;
int prj::Find() {
    FindDefinedsymbols();
    FindIncludePaths();
    FindSourseItems();
    FindGroup();
    return 0;
}
prj::~prj() {}
prj::prj() {}

int prj::FindGroup() {
    //准备筛选器
    int a = 0;
    for (set<string>::iterator i = srcItems.begin(); i != srcItems.end(); i++) {
        string ibuf(*i);
        string b = replace_str(ibuf, "/", "\\");
        b = replace_str(b, "..\\", "");
        if (b.size()) {
            while ((a = b.find("\\", a + 1)) != string::npos) {
                srcGroup.insert(b.substr(0, a));
            }
        }
    }
    return 0;
}

int prj_ptr::Load(std::string file) {
    if (prjCube::detect(file)) {
        reset(new prjCube(file));
        return 0;
    } else if (prjMCUX::detect(file)) {
        reset(new prjMCUX(file));
        return 0;
    }
    throw std::invalid_argument("no supported type");
}
std::string &replace_str(std::string &str, const std::string &to_replaced, const std::string &newchars) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos += newchars.length()) {
        pos = str.find(to_replaced, pos);
        if (pos != std::string::npos)
            str.replace(pos, to_replaced.length(), newchars);
        else
            break;
    }
    return str;
}
void listFiles(const filesystem::path &dir, vector<std::string> &list) {
    for (auto i:directory_iterator(dir)) {
        if (i.is_directory()) {
            listFiles(i, list);
        } else {
            list.push_back(i.path().string());
        }
    }
}
