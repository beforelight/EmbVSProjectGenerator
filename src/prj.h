#ifndef VSPG_PRJ_H
#define VSPG_PRJ_H
#include <set>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <filesystem>
#include <functional>
#include <utility>
#include "error.h"
#include "pugixml.hpp"
#include "NanoLog.hpp"

//负责将工程文件读入到内存
class prj {
public:
    prj();
    virtual ~prj();
    virtual int FindDefinedsymbols() = 0;
    virtual int FindIncludePaths() = 0;
    virtual int FindSourseItems() = 0;
    virtual int FindGroups();
    virtual int Find();
public:
    std::set<std::string> definedsymbols;//预处理器定义
    std::set<std::string> includePaths;//include目录
    std::set<std::string> srcItems;//源文件
    std::set<std::string> srcGroups;//源文件分组//中间变量
    std::string pathPrj;//项目的路径，包含最后的/
    std::string prjName;//项目名称
};

class prj_ptr : public std::shared_ptr<prj> {
public:
    int Load(const std::string& file);
    class login {
        friend class prj_ptr;
    public:
        login(std::function<bool(const std::string &)> _detect,
              std::function<prj *(const std::string &)> _newAnObj);
    private:
        std::function<bool(const std::string &)> detect;
        std::function<prj *(const std::string &)> newAnObj;
    };
};

#define _Max_value(a, b) ((a>b) ? a : b)
std::string &replace_str(std::string &str, const std::string &to_replaced, const std::string &newchars);
#define REPLACE_CHAR(str) replace_str(str,"\\","/")
void listFiles(const std::filesystem::path &dir, std::vector<std::string> &list);
#endif //VSPG_PRJ_H
