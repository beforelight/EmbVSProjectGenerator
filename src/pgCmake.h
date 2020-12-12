//
// Created by 17616 on 2020/12/12.
//

#ifndef VSPG_PGCMAKE_H
#define VSPG_PGCMAKE_H

#include "pg.h"
class pgCmake : public pg{
public:
    ~pgCmake() {};
    pgCmake(prj_ptr &ptr,std::string &exe_path) : pg(ptr,exe_path) {};
    int Generate();
};


#endif //VSPG_PGCMAKE_H
