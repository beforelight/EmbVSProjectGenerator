#ifndef VSPG_PRJMCUX_H
#define VSPG_PRJMCUX_H

#include "prj.h"
class prjMCUX: public prj {
private:
    std::string cproject;
    std::string project;
    pugi::xml_document doc;
public:
    prjMCUX(const std::string& file);
    int FindDefinedsymbols();
    int FindIncludePaths();
    int FindSourseItems();
    static bool detect(std::string file);
};


#endif //VSPG_PRJMCUX_H
