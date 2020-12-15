#include "pgVS.h"
#include <sstream>
#include <fstream>
#include "error.h"
#include <cstdlib>
#include <ctime>
#include "tinyxml2.h"

pg::login loginVS("vs",
                     [](prj_ptr &ptr, const std::string &exe_path) {
                         return new pgVS(ptr, exe_path);
                     }
);
using namespace std;
using namespace tinyxml2;
int pgVS::Generate() {
    slnGenerate();
    vcxprojGenerate();
    filtersGenerate();
    userGenerate();
    return 0;
}
int pgVS::slnGenerate() {
    //准备文件
    stringstream ss;
    string buf;
    ofstream sinNew(prjPtr->pathPrj + prjPtr->prjName + ".sln");
    ifstream sinTemp(pathExe + resource + "EmbProjectTemplate.sln");

    LOG_INFO<<".sin path:"<< prjPtr->pathPrj + prjPtr->prjName + ".sln";
    LOG_INFO << ".sin path:" << pathExe + resource + "EmbProjectTemplate.sln";
    if (!sinNew.is_open()
        || !sinTemp.is_open()) {
        throw ERROR("fail to open the file");
    }
    ss << sinTemp.rdbuf();
    buf = ss.str();
    replace_str(buf, "EmbProjectTemplate", prjPtr->prjName);
    sinNew << buf;
    sinTemp.close();
    sinNew.close();
    return 0;
}
int pgVS::vcxprojGenerate() {
    //准备文件
    stringstream ss;
    string buf;
    ofstream sinNew(prjPtr->pathPrj + prjPtr->prjName + ".vcxproj");
    ifstream sinTemp(pathExe + resource + "EmbProjectTemplate.vcxproj");
    LOG_INFO<<".vcxproj path:"<< prjPtr->pathPrj + prjPtr->prjName + ".vcxproj";
    LOG_INFO << ".vcxproj path:" << pathExe + resource + "EmbProjectTemplate.vcxproj";
    if (!sinNew.is_open()
        || !sinTemp.is_open()) {
        throw ERROR("fail to open the file");
    }
    ss << sinTemp.rdbuf();
    buf = ss.str();
    replace_str(buf, "EmbProjectTemplate", prjPtr->prjName);
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
    ewp.LoadFile((prjPtr->pathPrj + prjPtr->prjName + ".vcxproj").c_str());

    titleElement = ewp.FirstChildElement("Project")->LastChildElement("PropertyGroup");
    for (size_t i = 0; i < 4; i++)
    {
        subElement = titleElement->FirstChildElement("NMakePreprocessorDefinitions");
        buf = subElement->GetText();
        for (const auto & definedsymbol : prjPtr->definedsymbols)
        {
            buf += ";";
            buf += definedsymbol;
        }
        subElement->SetText(buf.c_str());
        newElement = ewp.NewElement("IncludePath");
        buf.clear();
        for (auto ibuf : prjPtr->includePaths)
        {
            buf += "$(MSBuildProjectDirectory)/";
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
    for (const auto & srcItem : prjPtr->srcItems)
    {
        if (-1 != srcItem.find(".c"))
        {
            newElement = ewp.NewElement("ClCompile");
            string ibuf(srcItem);
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
    for (const auto & srcItem : prjPtr->srcItems)
    {
        if (-1 != srcItem.find(".h"))
        {
            newElement = ewp.NewElement("ClInclude");
            string ibuf(srcItem);
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
    for (const auto & srcItem : prjPtr->srcItems)
    {
        if (!(-1 != srcItem.find(".h")
              || -1 != srcItem.find(".c")))
        {
            newElement = ewp.NewElement("None");
            string ibuf(srcItem);
            newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
            titleElement->InsertEndChild(newElement);
        }
    }

    ewp.SaveFile((prjPtr->pathPrj + prjPtr->prjName + ".vcxproj").c_str());
    return 0;
}
int pgVS::filtersGenerate() {
    //准备文件
    stringstream ss;
    string buf;
    ofstream sinNew(prjPtr->pathPrj + prjPtr->prjName + ".vcxproj.filters");
    ifstream sinTemp(pathExe + resource + "EmbProjectTemplate.vcxproj.filters");
    LOG_INFO<<".vcxproj.filters path:"<< prjPtr->pathPrj + prjPtr->prjName + ".vcxproj.filters";
    LOG_INFO << ".vcxproj.filters path:" << pathExe + resource + "EmbProjectTemplate.vcxproj.filters";
    if (!sinNew.is_open()
        || !sinTemp.is_open()) {
        throw ERROR("fail to open the file");
    }
    ss << sinTemp.rdbuf();
    buf = ss.str();
    replace_str(buf, "EmbProjectTemplate", prjPtr->prjName);
    sinNew << buf;
    sinTemp.close();
    sinNew.close();

    //进行xml修改
    XMLDocument ewp;
    XMLElement* titleElement;
//    XMLElement* subElement;
    XMLElement* newElement;
    XMLElement* newElement2;
    ewp.LoadFile((prjPtr->pathPrj + prjPtr->prjName + ".vcxproj.filters").c_str());

    //1.
    titleElement = ewp.FirstChildElement("Project")->LastChildElement("ItemGroup");
    titleElement->DeleteChildren();
    for (auto ibuf : prjPtr->srcGroups)
    {
        newElement = ewp.NewElement("Filter");
        replace_str(ibuf, "..\\", "");
        if(ibuf==".."){
            continue;
        }
        newElement->SetAttribute("Include", ibuf.c_str());
        newElement2 = ewp.NewElement("UniqueIdentifier");
        UniqueIdentifier uniqueIdentifier;
        newElement2->SetText(uniqueIdentifier.c_str());
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
    for (const auto & srcItem : prjPtr->srcItems)
    {
        if (-1 != srcItem.find(".c"))
        {
            newElement = ewp.NewElement("ClCompile");
            string ibuf(srcItem);
            newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
            string str = replace_str(ibuf, "/", "\\");
            replace_str(str, "..\\", "");
            if (str.find('\\') != string::npos)
            {
                newElement2 = ewp.NewElement("Filter");
                int length_a = str.find_last_of('\\');
                str = str.substr(0, length_a);
                newElement2->SetText(str.c_str());
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
    for (const auto & srcItem : prjPtr->srcItems)
    {
        if (-1 != srcItem.find(".h"))
        {
            newElement = ewp.NewElement("ClInclude");
            string ibuf(srcItem);
            newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
            string str = replace_str(ibuf, "/", "\\");
            replace_str(str, "..\\", "");
            if (str.find('\\') != string::npos)
            {
                newElement2 = ewp.NewElement("Filter");
                int length_a = str.find_last_of('\\');
                str = str.substr(0, length_a);
                newElement2->SetText(str.c_str());
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
    for (const auto & srcItem : prjPtr->srcItems)
    {
        if (!(-1 != srcItem.find(".h")
              || -1 != srcItem.find(".c")))
        {
            newElement = ewp.NewElement("None");
            string ibuf(srcItem);
            newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
            string str = replace_str(ibuf, "/", "\\");
            replace_str(str, "..\\", "");
            if (str.find('\\') != string::npos)
            {
                newElement2 = ewp.NewElement("Filter");
                int length_a = str.find_last_of('\\');
                str = str.substr(0, length_a);
                newElement2->SetText(str.c_str());
                newElement->InsertEndChild(newElement2);
            }
            titleElement->InsertEndChild(newElement);
        }
    }
    ewp.SaveFile((prjPtr->pathPrj + prjPtr->prjName + ".vcxproj.filters").c_str());
    return 0;
}
int pgVS::userGenerate() {
    //准备文件
    stringstream ss;
    string buf;
    ofstream sinNew(prjPtr->pathPrj + prjPtr->prjName + ".vcxproj.user");
    ifstream sinTemp(pathExe + resource + "EmbProjectTemplate.vcxproj.user");

    LOG_INFO<<".vcxproj.user path:"<< prjPtr->pathPrj + prjPtr->prjName + ".vcxproj.user";
    LOG_INFO << ".vcxproj.user path:" << pathExe + resource + "EmbProjectTemplate.vcxproj.user";
    if (!sinNew.is_open()
        || !sinTemp.is_open()) {
        throw ERROR("fail to open the file");
    }
    ss << sinTemp.rdbuf();
    buf = ss.str();
    replace_str(buf, "EmbProjectTemplate", prjPtr->prjName);
    sinNew << buf;
    sinTemp.close();
    sinNew.close();
    return 0;
}


pgVS::UniqueIdentifier::UniqueIdentifier() {
    if (timeSrandSeed == 0) {
        srand(time(NULL));
        timeSrandSeed = 1;
    }
    Refresh();
}

pgVS::UniqueIdentifier::~UniqueIdentifier() {
}

const char randCharSet[] = {"0123456789abcdef"};
void pgVS::UniqueIdentifier::Refresh() {
    std::string::clear();
    //<UniqueIdentifier>{dc2250e4-e44f-4b53-925e-f71d02f20bba}</UniqueIdentifier>
    std::string::append(1, '{');
    for (size_t i = 0; i < 8; i++) {
        int a = rand() % strlen(randCharSet);
        std::string::append(1, randCharSet[a]);
    }

    std::string::append("-");
    for (size_t i = 0; i < 4; i++) {
        int a = rand() % strlen(randCharSet);
        std::string::append(1, randCharSet[a]);
    }

    std::string::append("-");
    for (size_t i = 0; i < 4; i++) {
        int a = rand() % strlen(randCharSet);
        std::string::append(1, randCharSet[a]);
    }

    std::string::append("-");
    for (size_t i = 0; i < 4; i++) {
        int a = rand() % strlen(randCharSet);
        std::string::append(1, randCharSet[a]);
    }

    std::string::append("-");
    for (size_t i = 0; i < 12; i++) {
        int a = rand() % strlen(randCharSet);
        std::string::append(1, randCharSet[a]);
    }

    std::string::append(1, '}');
}
int pgVS::UniqueIdentifier::timeSrandSeed = 0;