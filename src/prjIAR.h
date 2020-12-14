//
// Created by 17616 on 2020/12/14.
//

#ifndef VSPG_PRJIAR_H
#define VSPG_PRJIAR_H
#include "prj.h"

class prjIAR : public prj{
private:
    std::string ewp;
public:
    prjIAR(std::string file);
    int FindDefinedsymbols();
    int FindIncludePaths();
    int FindSourseItems();
    static bool detect(const std::string& file);
};


#endif //VSPG_PRJIAR_H
