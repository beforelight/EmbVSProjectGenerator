//
// Created by 17616 on 2020/12/8.
//

#include "prj.h"
#include "prjCube.h"
#include "prjMCUX.h"
using namespace std;
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
    for (set<string>::iterator i = srcItems.begin(); i != srcItems.end(); i++)
    {
        string ibuf(*i);
        string b = replace_str(ibuf, "/", "\\");
        b = replace_str(b, "..\\", "");
        if (b.size())
        {
            while ((a = b.find("\\", a + 1)) != string::npos)
            {
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
void listFiles(const char *dir, std::vector<std::string> &list) {
    std::string dirNew(dir);
    std::string str;

    if (dirNew.c_str()[dirNew.size() - 1] == '/' ||
        dirNew.c_str()[dirNew.size() - 1] == '\\') {
        dirNew.pop_back();
    }

    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst((dirNew + "/*.*").c_str(), &findData);
    if (handle == -1)        // 检查是否成功
        return;
    do {
        if (findData.attrib & _A_SUBDIR) {
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0) {
                continue;
            }
            str = dirNew + '\\' + findData.name;
            LOG_INFO << str << "\t<dir>";
            listFiles(str.c_str(), list);
        } else {
            str = dirNew + '\\' + findData.name;
            LOG_INFO << str << '\t' << (size_t) findData.size << " bytes.";
            list.push_back(str);
        }
    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // 关闭搜索句柄
}
