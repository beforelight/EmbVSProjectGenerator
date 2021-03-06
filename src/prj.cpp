﻿#include "prj.h"
using namespace std;
using namespace std::filesystem;
int prj::Find() {
    FindDefinedsymbols();
    FindIncludePaths();
    FindSourseItems();
    FindGroups();
    return 0;
}
prj::~prj() = default;
prj::prj() = default;
int prj::FindGroups() {
    int length_a = 0;
    for (auto srcItem : srcItems) {
        string str = replace_str(srcItem, "/", "\\");
        if (!str.empty()) {
            while ((length_a = str.find('\\', length_a + 1)) != string::npos) {
                srcGroups.insert(str.substr(0, length_a));
            }
        }
    }
    return 0;
}

std::string &replace_str(std::string &str, const std::string &to_replaced, const std::string &newchars) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos += newchars.length()) {
        pos = str.find(to_replaced, pos);
        if (pos != std::string::npos) {
            str.replace(pos, to_replaced.length(), newchars);
        } else {
            break;
        }
    }
    return str;
}
void listFiles(const filesystem::path &dir, vector<std::string> &list) {
    for (const auto& i:directory_iterator(dir)) {
        if (i.is_directory()) {
            listFiles(i, list);
        } else {
            list.push_back(i.path().string());
        }
    }
}
vector<prj_ptr::login *> prj_ptr_list;
prj_ptr::login::
login(std::function<bool(const std::string &)> _detect,
      std::function<prj *(const std::string &)> _newAnObj)
        : detect(std::move(_detect)), newAnObj(std::move(_newAnObj)) {
    prj_ptr_list.push_back(this);
}
int prj_ptr::Load(const std::string& file) {
    for (auto i:prj_ptr_list) {
        if (i->detect(file)) {
            reset(i->newAnObj(file));
            return 0;
        }
    }
    throw ERROR("Unsupported type");
}