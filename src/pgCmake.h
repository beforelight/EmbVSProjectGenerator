#ifndef VSPG_PGCMAKE_H
#define VSPG_PGCMAKE_H

#include "pg.h"
class pgCmake : public pg{
public:
    ~pgCmake() {};
    pgCmake(prj_ptr &ptr,const std::string &exe_path) : pg(ptr,exe_path) {};
    int Generate() override;
};


#endif //VSPG_PGCMAKE_H
