#ifndef VSPG_PGVSCODE_H
#define VSPG_PGVSCODE_H
#include "pg.h"

class pgVSCode : public pg{
public:
    ~pgVSCode() {};
    pgVSCode(prj_ptr &ptr,const std::string &exe_path) : pg(ptr,exe_path) {};
    int Generate();
};


#endif //VSPG_PGVSCODE_H
