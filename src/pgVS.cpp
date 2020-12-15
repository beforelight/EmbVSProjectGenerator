#include "pgVS.h"
#include <sstream>
#include <fstream>
#include "error.h"
#include <cstdlib>
#include <ctime>
#include "tinyxml2.h"

pg::login loginVS("vs",
                     [](prj_ptr &ptr, const std::string &exe_path) {
                         return new pgvs(ptr, exe_path);
                     }
);
using namespace std;
using namespace tinyxml2;
int pgvs::Generate() {
    slnGenerate();
    vcxprojGenerate();
    filtersGenerate();
    userGenerate();
    return 0;
}
int pgvs::slnGenerate() {
    //准备文件
    stringstream ss;
    string buf;
    ofstream sinNew(prjPtr->path + prjPtr->prj_name + ".sln");
    ifstream sinTemp(path_exe + resource + "EmbProjectTemplate.sln");

    LOG_INFO<<".sin path:"<<prjPtr->path + prjPtr->prj_name + ".sln";
    LOG_INFO<<".sin path:"<<path_exe + resource + "EmbProjectTemplate.sln";
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
int pgvs::vcxprojGenerate() {
    //准备文件
    stringstream ss;
    string buf;
    ofstream sinNew(prjPtr->path + prjPtr->prj_name + ".vcxproj");
    ifstream sinTemp(path_exe + resource + "EmbProjectTemplate.vcxproj");
    LOG_INFO<<".vcxproj path:"<<prjPtr->path + prjPtr->prj_name + ".vcxproj";
    LOG_INFO<<".vcxproj path:"<<path_exe + resource + "EmbProjectTemplate.vcxproj";
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

    //进行xml修改
    //1.添加宏定义
    //2.添加include目录
    XMLDocument ewp;
    XMLElement* titleElement;
    XMLElement* subElement;
    XMLElement* newElement;
    ewp.LoadFile((prjPtr->path + prjPtr->prj_name + ".vcxproj").c_str());

    titleElement = ewp.FirstChildElement("Project")->LastChildElement("PropertyGroup");
    for (size_t i = 0; i < 4; i++)
    {
        subElement = titleElement->FirstChildElement("NMakePreprocessorDefinitions");
        buf = subElement->GetText();
        for (set<string>::iterator j = prjPtr->definedsymbols.begin(); j != prjPtr->definedsymbols.end(); j++)
        {
            buf += ";";
            buf += *j;
        }
        subElement->SetText(buf.c_str());
        newElement = ewp.NewElement("IncludePath");
        buf.clear();
        for (set<string>::iterator j = prjPtr->includePaths.begin(); j != prjPtr->includePaths.end(); j++)
        {
            buf += "$(MSBuildProjectDirectory)/";
            string ibuf(*j);
            buf += replace_str(ibuf, "\\", "/");
            buf += ";";
        }
        buf += "$(IncludePath)";
        newElement->SetText(buf.c_str());
        titleElement->InsertEndChild(newElement);
        titleElement = titleElement->PreviousSiblingElement("PropertyGroup");
    }
    //3.添加源文件分组
    titleElement = ewp.FirstChildElement("Project")->LastChildElement("ItemGroup");
    //buf.clear();
    for (set<string>::iterator i = prjPtr->srcItems.begin(); i != prjPtr->srcItems.end(); i++)
    {
        if (-1 != (*i).find(".c"))
        {
            newElement = ewp.NewElement("ClCompile");
            string ibuf(*i);
            newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
            titleElement->InsertEndChild(newElement);
        }
    }

    //4.添加头文件分组
    newElement = ewp.NewElement("ItemGroup");
    ewp.FirstChildElement("Project")
            ->InsertAfterChild(ewp.FirstChildElement("Project")->LastChildElement("ItemGroup"),
                               newElement);

    titleElement = ewp.FirstChildElement("Project")->LastChildElement("ItemGroup");
    //buf.clear();
    for (set<string>::iterator i = prjPtr->srcItems.begin(); i != prjPtr->srcItems.end(); i++)
    {
        if (-1 != (*i).find(".h"))
        {
            newElement = ewp.NewElement("ClInclude");
            string ibuf(*i);
            newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
            titleElement->InsertEndChild(newElement);
        }
    }

    //5.添加其他文件分组
    newElement = ewp.NewElement("ItemGroup");
    ewp.FirstChildElement("Project")
            ->InsertAfterChild(ewp.FirstChildElement("Project")->LastChildElement("ItemGroup"),
                               newElement);

    titleElement = ewp.FirstChildElement("Project")->LastChildElement("ItemGroup");
    //buf.clear();
    for (set<string>::iterator i = prjPtr->srcItems.begin(); i != prjPtr->srcItems.end(); i++)
    {
        if (!(-1 != (*i).find(".h")
              || -1 != (*i).find(".c")))
        {
            newElement = ewp.NewElement("None");
            string ibuf(*i);
            newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
            titleElement->InsertEndChild(newElement);
        }
    }

    ewp.SaveFile((prjPtr->path + prjPtr->prj_name + ".vcxproj").c_str());
    return 0;
}
int pgvs::filtersGenerate() {
    //准备文件
    stringstream ss;
    string buf;
    ofstream sinNew(prjPtr->path + prjPtr->prj_name + ".vcxproj.filters");
    ifstream sinTemp(path_exe + resource + "EmbProjectTemplate.vcxproj.filters");
    LOG_INFO<<".vcxproj.filters path:"<<prjPtr->path + prjPtr->prj_name + ".vcxproj.filters";
    LOG_INFO<<".vcxproj.filters path:"<<path_exe + resource + "EmbProjectTemplate.vcxproj.filters";
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

    //进行xml修改
    XMLDocument ewp;
    XMLElement* titleElement;
    XMLElement* subElement;
    XMLElement* newElement;
    XMLElement* newElement2;
    ewp.LoadFile((prjPtr->path + prjPtr->prj_name + ".vcxproj.filters").c_str());

    //1.
    titleElement = ewp.FirstChildElement("Project")->LastChildElement("ItemGroup");
    titleElement->DeleteChildren();
    for (set<string>::iterator i = prjPtr->srcGroup.begin(); i != prjPtr->srcGroup.end(); i++)
    {
        newElement = ewp.NewElement("Filter");
        string ibuf(*i);
        replace_str(ibuf, "..\\", "");
        if(ibuf==".."){
            continue;
        }
        newElement->SetAttribute("Include", ibuf.c_str());
        newElement2 = ewp.NewElement("UniqueIdentifier");
        UniqueIdentifier a;
        newElement2->SetText(a.c_str());
        newElement->InsertFirstChild(newElement2);
        titleElement->InsertEndChild(newElement);
        //cout << (*i) << endl;
    }

    //2.添加源文件分组
    newElement = ewp.NewElement("ItemGroup");
    ewp.FirstChildElement("Project")
            ->InsertAfterChild(ewp.FirstChildElement("Project")->LastChildElement("ItemGroup"),
                               newElement);
    titleElement = ewp.FirstChildElement("Project")->LastChildElement("ItemGroup");
    //buf.clear();
    for (set<string>::iterator i = prjPtr->srcItems.begin(); i != prjPtr->srcItems.end(); i++)
    {
        if (-1 != (*i).find(".c"))
        {
            newElement = ewp.NewElement("ClCompile");
            string ibuf(*i);
            newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
            string b = replace_str(ibuf, "/", "\\");
            replace_str(b, "..\\", "");
            if (b.find('\\') != string::npos)
            {
                newElement2 = ewp.NewElement("Filter");
                int a = b.find_last_of('\\');
                b = b.substr(0, a);
                newElement2->SetText(b.c_str());
                newElement->InsertEndChild(newElement2);
            }

            titleElement->InsertEndChild(newElement);
        }
    }

    //3.添加头文件分组
    newElement = ewp.NewElement("ItemGroup");
    ewp.FirstChildElement("Project")
            ->InsertAfterChild(ewp.FirstChildElement("Project")->LastChildElement("ItemGroup"),
                               newElement);

    titleElement = ewp.FirstChildElement("Project")->LastChildElement("ItemGroup");
    //buf.clear();
    for (set<string>::iterator i = prjPtr->srcItems.begin(); i != prjPtr->srcItems.end(); i++)
    {
        if (-1 != (*i).find(".h"))
        {
            newElement = ewp.NewElement("ClInclude");
            string ibuf(*i);
            newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
            string b = replace_str(ibuf, "/", "\\");
            replace_str(b, "..\\", "");
            if (b.find('\\') != string::npos)
            {
                newElement2 = ewp.NewElement("Filter");
                int a = b.find_last_of('\\');
                b = b.substr(0, a);
                newElement2->SetText(b.c_str());
                newElement->InsertEndChild(newElement2);
            }
            titleElement->InsertEndChild(newElement);
        }
    }

    //4.添加其他文件分组
    newElement = ewp.NewElement("ItemGroup");
    ewp.FirstChildElement("Project")
            ->InsertAfterChild(ewp.FirstChildElement("Project")->LastChildElement("ItemGroup"),
                               newElement);

    titleElement = ewp.FirstChildElement("Project")->LastChildElement("ItemGroup");
//    assert(titleElement);
    //buf.clear();
    for (set<string>::iterator i = prjPtr->srcItems.begin(); i != prjPtr->srcItems.end(); i++)
    {
        if (!(-1 != (*i).find(".h")
              || -1 != (*i).find(".c")))
        {
            newElement = ewp.NewElement("None");
            string ibuf(*i);
            newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
            string b = replace_str(ibuf, "/", "\\");
            replace_str(b, "..\\", "");
            if (b.find('\\') != string::npos)
            {
                newElement2 = ewp.NewElement("Filter");
                int a = b.find_last_of('\\');
                b = b.substr(0, a);
                newElement2->SetText(b.c_str());
                newElement->InsertEndChild(newElement2);
            }
            titleElement->InsertEndChild(newElement);
        }
    }
    ewp.SaveFile((prjPtr->path + prjPtr->prj_name + ".vcxproj.filters").c_str());
    return 0;
}
int pgvs::userGenerate() {
    //准备文件
    stringstream ss;
    string buf;
    ofstream sinNew(prjPtr->path + prjPtr->prj_name + ".vcxproj.user");
    ifstream sinTemp(path_exe + resource + "EmbProjectTemplate.vcxproj.user");

    LOG_INFO<<".vcxproj.user path:"<<prjPtr->path + prjPtr->prj_name +".vcxproj.user";
    LOG_INFO<<".vcxproj.user path:"<<path_exe + resource + "EmbProjectTemplate.vcxproj.user";
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
void pgvs::UniqueIdentifier::Refresh() {
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