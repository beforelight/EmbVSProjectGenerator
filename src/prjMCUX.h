//
// Created by 17616 on 2020/12/12.
//

#ifndef VSPG_PRJMCUX_H
#define VSPG_PRJMCUX_H

#include "prj.h"
class prjMCUX: public prj {
private:
    std::string cproject;
    std::string project;
public:
    prjMCUX(std::string file);
    int FindDefinedsymbols();
    int FindIncludePaths();
    int FindSourseItems();
    static bool detect(std::string file);
};


#endif //VSPG_PRJMCUX_H
