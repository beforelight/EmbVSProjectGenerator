#ifndef VSPG_PRJMDK_H
#define VSPG_PRJMDK_H

#include "prj.h"

class prjMDK :public prj{
private:
    std::string uvproj;
    pugi::xml_document doc;
public:
    prjMDK(const std::string& file);
    int FindDefinedsymbols() override;
    int FindIncludePaths() override;
    int FindSourseItems() override;
    static bool detect(const std::string& file);
};


#endif //VSPG_PRJMDK_H
