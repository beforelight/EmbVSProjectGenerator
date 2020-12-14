//
// Created by 17616 on 2020/12/14.
//

#ifndef VSPG_PRJMDK_H
#define VSPG_PRJMDK_H

#include "prj.h"

class prjMDK :public prj{
private:
    std::string uvproj;
public:
    prjMDK(std::string file);
    int FindDefinedsymbols();
    int FindIncludePaths();
    int FindSourseItems();
    static bool detect(const std::string& file);
};


#endif //VSPG_PRJMDK_H
