//
// Created by 17616 on 2020/12/12.
//

#include "pg.h"

#include <utility>
pg::pg(prj_ptr &ptr, std::string _pathExe) : prjPtr(ptr), pathExe(std::move(_pathExe)) {}


std::vector<pg::login *> pgs;
pg::login::login(std::string _type, std::function<pg *(prj_ptr &, const std::string &)> _newAnObj) :
        type(std::move(_type)), newAnObj(std::move(_newAnObj)) {
    pgs.push_back(this);
}

pg::login loginPG("all",
                  [](prj_ptr &ptr, const std::string &exe_path) {
                      return new pg(ptr, exe_path);
                  }
);
//for all
int pg::Generate() {
    std::shared_ptr<pg> pg_ptr;
    for (auto i:pgs) {
        if (i->type != "all") {
            pg_ptr.reset(i->newAnObj(prjPtr, pathExe));
            pg_ptr->Generate();
        }
    }
    return 0;
}
int pg::Generate(const std::string &_type,prj_ptr &ptr, const std::string &_pathExe) {
    std::shared_ptr<pg> pg_ptr;
    for(auto i:pgs){
        if(i->type==_type){
            pg_ptr.reset(i->newAnObj(ptr, _pathExe));
            return pg_ptr->Generate();
        }
    }
    return -1;
}
cmdline::oneof_reader<std::string> pg::oneof_string() {
    cmdline::oneof_reader<std::string> ret;
    for(auto i:pgs){
        ret.add(i->type);
    }
    return ret;
}
