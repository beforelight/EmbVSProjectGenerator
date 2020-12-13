//
// Created by 17616 on 2020/12/12.
//

#ifndef VSPG_PG_H
#define VSPG_PG_H

#include "prj.h"
class pg {
protected:
    prj_ptr prjPtr;
    std::string path_exe;//包括最后的/
public:
    virtual ~pg() {};
    pg(prj_ptr &ptr, std::string &exe_path) : prjPtr(ptr), path_exe(exe_path) {

    };
    virtual int Generate() = 0;
    constexpr const static char resource[] = "resource/";
};


#endif //VSPG_PG_H
