#ifndef VSPG_PRJCUBE_H
#define VSPG_PRJCUBE_H

#include "prj.h"
class prjCube : public prj {
private:
    std::string cproject;
    std::string project;
    pugi::xml_document doc;
public:
    prjCube(const std::string &file);
    int FindDefinedsymbols();
    int FindIncludePaths();
    int FindSourseItems();
    static bool detect(const std::string &file);
};


#endif //VSPG_PRJCUBE_H
