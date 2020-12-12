//
// Created by 17616 on 2020/12/8.
//

#ifndef VSPG_PRJ_H
#define VSPG_PRJ_H
#include <set>
#include <string>
#include <memory>
#include <vector>
#include <io.h>
#include "pugixml.hpp"
#include "NanoLog.hpp"
//所有路径采用/
//负责将工程文件读入到内存
#define _Max_value(a, b) ((a>b) ? a : b)
class prj {
public:
    prj();
    virtual ~prj();
    virtual int FindDefinedsymbols() = 0;
    virtual int FindIncludePaths() = 0;
    virtual int FindSourseItems() = 0;
    int FindGroup();
    int Find();
public:
    std::set<std::string> definedsymbols;//预处理器定义
    std::set<std::string> includePaths;//include目录
    std::set<std::string> srcItems;//源文件
    std::set<std::string> srcGroup;//源文件分组//中间变量

    std::string path;//项目的路径，包含最后的/
    std::string prj_name;//项目名称
};
std::string& replace_str(std::string& str, const std::string& to_replaced, const std::string& newchars);
#define REPLACE_CHAR(str) replace_str(str,"\\","/")
void listFiles(const char *dir, std::vector<std::string> &list);

class prj_ptr : public std::shared_ptr<prj> {
public:
    int Load(std::string file);
};




#endif //VSPG_PRJ_H
