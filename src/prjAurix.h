#ifndef VSPG_PRJAURIX_HPP
#define VSPG_PRJAURIX_HPP

#include "prj.h"
class prjAurix : public prj {
private:
    std::string cproject;
    std::string project;
    pugi::xml_document doc;
public:
    prjAurix(const std::string &file);
    int FindDefinedsymbols() override;
    int FindIncludePaths() override;
    int FindSourseItems() override;
    static bool detect(const std::string &file);
};


#endif //VSPG_PRJAURIX_HPP
