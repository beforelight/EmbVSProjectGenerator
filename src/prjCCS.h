//
// Created by 17616 on 2021/10/1.
//

#ifndef VSPG_PRJCCS_H
#define VSPG_PRJCCS_H

#include "prj.h"
class prjCCS : public prj {
private:
    std::string cproject;
    std::string project;
    pugi::xml_document doc;
public:
    prjCCS(const std::string &file);
    int FindDefinedsymbols() override;
    int FindIncludePaths() override;
    int FindSourseItems() override;
    static bool detect(const std::string &file);
};


#endif //VSPG_PRJCCS_H
