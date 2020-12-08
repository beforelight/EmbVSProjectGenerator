//
// Created by 17616 on 2020/12/8.
//

#include "prj.h"
#include "prjCube.h"
int prj::Find() {
    int res = 0;
    res |= FindDefinedsymbols();
    res |= FindIncludePaths();
    res |= FindSourseItems();
    return res;
}
prj::~prj() {}
prj::prj() {}

int prj_ptr::Load(std::string file) {
    if (file.find(".project") != std::string::npos||(file.find(".cproject") != std::string::npos)){
        reset(new prjCube(file));
        return 0;
    }else{

    }
    return -1;
}
std::string &replace_str(std::string &str, const std::string &to_replaced, const std::string &newchars) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos += newchars.length())
    {
        pos = str.find(to_replaced, pos);
        if (pos != std::string::npos)
            str.replace(pos, to_replaced.length(), newchars);
        else
            break;
    }
    return   str;
}
