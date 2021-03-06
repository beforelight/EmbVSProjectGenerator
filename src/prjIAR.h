﻿#ifndef VSPG_PRJIAR_H
#define VSPG_PRJIAR_H
#include "prj.h"

class prjIAR : public prj{
private:
    std::string ewp;
    pugi::xml_document doc;
public:
    prjIAR(const std::string& file);
    int FindDefinedsymbols() override;
    int FindIncludePaths() override;
    int FindSourseItems() override;
    static bool detect(const std::string& file);
};


#endif //VSPG_PRJIAR_H
