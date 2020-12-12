//
// Created by 17616 on 2020/12/12.
//

#include "pgVS.h"
#include <sstream>
#include <fstream>
#include "error.h"
#include <cstdlib>
#include <ctime>
using namespace std;
int pgvs::Generate() {
    slnGenerate();
    vcxprojGenerate();
    filtersGenerate();
    userGenerate();
    return 0;
}
int pgvs::slnGenerate(void) {
    //准备文件
    stringstream ss;
    string buf;
    ofstream sinNew(prjPtr->path + prjPtr->prj_name + ".sln");
    ifstream sinTemp(path_exe + resource + "EmbProjectTemplate.sln");
    if (!sinNew.is_open()
        || !sinTemp.is_open()) {
        throw ERROR("fail to open the file");
    }
    ss << sinTemp.rdbuf();
    buf = ss.str();
    replace_str(buf, "EmbProjectTemplate", prjPtr->prj_name);
    sinNew << buf;
    sinTemp.close();
    sinNew.close();
    return 0;
}
int pgvs::vcxprojGenerate(void) {
    //准备文件
    stringstream ss;
    string buf;
    ofstream sinNew(prjPtr->path + prjPtr->prj_name + ".vcxproj");
    ifstream sinTemp(path_exe + resource + "EmbProjectTemplate.vcxproj");
    if (!sinNew.is_open()
        || !sinTemp.is_open()) {
        throw ERROR("fail to open the file");
    }
    ss << sinTemp.rdbuf();
    buf = ss.str();
    replace_str(buf, "EmbProjectTemplate", prjPtr->prj_name);
    sinNew << buf;
    sinTemp.close();
    sinNew.close();
    return 0;
}
int pgvs::filtersGenerate(void) {
    return 0;
}
int pgvs::userGenerate(void) {
    return 0;
}


pgvs::UniqueIdentifier::UniqueIdentifier() {
    if (timeSrandSeed == 0) {
        srand(time(NULL));
        timeSrandSeed = 1;
    }
    Refresh();
}

pgvs::UniqueIdentifier::~UniqueIdentifier() {
}

const char randchar[] = {"0123456789abcdef"};
void pgvs::UniqueIdentifier::Refresh(void) {
    std::string::clear();
    //<UniqueIdentifier>{dc2250e4-e44f-4b53-925e-f71d02f20bba}</UniqueIdentifier>
    std::string::append(1, '{');
    for (size_t i = 0; i < 8; i++) {
        int a = rand() % strlen(randchar);
        std::string::append(1, randchar[a]);
    }

    std::string::append("-");
    for (size_t i = 0; i < 4; i++) {
        int a = rand() % strlen(randchar);
        std::string::append(1, randchar[a]);
    }

    std::string::append("-");
    for (size_t i = 0; i < 4; i++) {
        int a = rand() % strlen(randchar);
        std::string::append(1, randchar[a]);
    }

    std::string::append("-");
    for (size_t i = 0; i < 4; i++) {
        int a = rand() % strlen(randchar);
        std::string::append(1, randchar[a]);
    }

    std::string::append("-");
    for (size_t i = 0; i < 12; i++) {
        int a = rand() % strlen(randchar);
        std::string::append(1, randchar[a]);
    }

    std::string::append(1, '}');
}
int pgvs::UniqueIdentifier::timeSrandSeed = 0;