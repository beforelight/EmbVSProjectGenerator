//
// Created by 17616 on 2020/12/12.
//

#ifndef VSPG_PG_H
#define VSPG_PG_H

#include "prj.h"
#include "cmdline.h"
class pg {
protected:
    prj_ptr prjPtr;
    std::string path_exe;//包括最后的/
public:
    virtual ~pg() {};
    pg(prj_ptr &ptr, std::string exe_path);
    virtual int Generate();

public:
    constexpr const static char resource[] = "resource/";
    static int Generate(const std::string &_type, prj_ptr &ptr, const std::string &exe_path);
    static cmdline::oneof_reader<std::string> oneof_string();

    class login {
        friend class pg;
    public:
        login(std::string _type, std::function<pg *(prj_ptr &ptr, const std::string &)> _newAnObj);
    private:
        std::string type;
        std::function<pg *(prj_ptr &ptr, const std::string &)> newAnObj;
    };
};


#endif //VSPG_PG_H
