//
// Created by 17616 on 2020/12/8.
//

#include <iostream>
#include <fstream>
#include "pugixml.hpp"
#include <io.h>
#include "NanoLog.hpp"
#include <vector>
#include <CJsonObject.hpp>
#include <sstream>
#include "error.h"
using namespace pugi;
using namespace std;


void listFiles(const char *dir) {
    char dirNew[200];
    strcpy(dirNew, dir);
    strcat(dirNew, "/*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(dirNew, &findData);
    if (handle == -1)        // 检查是否成功
        return;

    do {
        if (findData.attrib & _A_SUBDIR) {
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;

            cout << findData.name << "\t<dir>\n";

            // 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
            strcpy(dirNew, dir);
            strcat(dirNew, "\\");
            strcat(dirNew, findData.name);

            listFiles(dirNew);
        } else
            cout << dir << findData.name << "\t" << findData.size << " bytes.\n";
    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // 关闭搜索句柄
}

void listFiles(const char *dir, std::vector<string> &list) {
    std::string dirNew(dir);
    std::string str;

    if (dirNew.c_str()[dirNew.size() - 1] == '/' ||
        dirNew.c_str()[dirNew.size() - 1] == '\\') {
        dirNew.pop_back();
    }

    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst((dirNew + "/*.*").c_str(), &findData);
    if (handle == -1)        // 检查是否成功
        return;
    do {
        if (findData.attrib & _A_SUBDIR) {
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0) {
                continue;
            }
            str = dirNew + '\\' + findData.name;
            LOG_INFO << str << "\t<dir>";
            listFiles(str.c_str(), list);
        } else {
            str = dirNew + '\\' + findData.name;
            LOG_INFO << str << '\t' << (size_t) findData.size << " bytes.";
            list.push_back(str);
        }
    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // 关闭搜索句柄
}
int main() {
//    nanolog::initialize(nanolog::NonGuaranteedLogger(10),
//                        "bin/", "test", 1);
//    xml_document doc;
//
////    if (doc.load_file("xmlfile/.cproject")) {
////        xpath_node_set def = doc.select_nodes("//toolChain/tool/option");
////        for(xpath_node i:def){
////            if(string(i.node().attribute("id").value())
////            .find("compiler.option.definedsymbols")
////            !=string::npos){
////                cout<<"id:"<<i.node().attribute("id").value()<<endl;
////            }
////        }
////    } else {
////        printf("no file\r\n");
////    }
//    std::vector<string> files;
//
//    listFiles("C:\\Users\\17616\\Documents\\STM32CubeIDE\\LcSensor", files);
    try {
//        throw "some error";
//        throw std::invalid_argument("arg err");
        throw ERROR("arg err");
    }
    catch (const char *msg) {
        cout << msg << endl;
    }
    catch (const std::exception &e) {
        cout << e.what() << endl;
    }
    std::ifstream fin("bin/resource/c_cpp_properties.json");
    if (fin.is_open()){
        neb::CJsonObject oJson;
        std::stringstream ssContent;
        ssContent << fin.rdbuf();
        if (oJson.Parse(ssContent.str()))
        {
            cout<<oJson["configurations"][0]("name");
        }
        else
        {
            std::cerr << "parse json error" << "\n";// << ssContent.str() << std::endl;
        }
        fin.close();
    }

    return 0;
}
