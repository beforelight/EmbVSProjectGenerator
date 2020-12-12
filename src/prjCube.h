//
// Created by 17616 on 2020/12/8.
//

#ifndef VSPG_PRJCUBE_H
#define VSPG_PRJCUBE_H

#include "prj.h"
class prjCube : public prj{
private:
    std::string cproject;
    std::string project;
public:
    prjCube(std::string file);
    int FindDefinedsymbols();
    int FindIncludePaths();
    int FindSourseItems();
    static bool detect(std::string file);
};


#endif //VSPG_PRJCUBE_H
