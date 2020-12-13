//
// Created by 17616 on 2020/12/12.
//

#ifndef VSPG_PGVS_H
#define VSPG_PGVS_H
#include "pg.h"

class pgvs : public pg {
public:
    ~pgvs() {};
    pgvs(prj_ptr &ptr,std::string &exe_path) : pg(ptr,exe_path) {};
    int Generate();
    int slnGenerate(void);
    int vcxprojGenerate(void);
    int filtersGenerate(void);
    int userGenerate(void);

    //用来生成//<UniqueIdentifier>{dc2250e4-e44f-4b53-925e-f71d02f20bba}</UniqueIdentifier>类似的字符串
    class UniqueIdentifier : public std::string {
    public:
        UniqueIdentifier();
        ~UniqueIdentifier();
        void Refresh(void);
    private:
        static int timeSrandSeed;
    };
};


#endif //VSPG_PGVS_H
