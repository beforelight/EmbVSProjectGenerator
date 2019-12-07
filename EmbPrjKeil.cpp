#include "pch.h"
#include "EmbPrjKeil.h"

EmbPrjKeil::EmbPrjKeil(string path_uvprojx_, string path_exe_)
{
	setPathExe(path_exe_);
	int a = path_uvprojx_.find_last_of('\\');
	int b = path_uvprojx_.find_last_of('/');
	int c = path_uvprojx_.find_last_of('.');
	a = _Max_value(a, b);
	string path = path_uvprojx_.substr(0, a + 1);//包含最后的那个'/'
	string name = path_uvprojx_.substr(a + 1, c - a - 1);
	setPathProjectName(path, name);

	path_uvprojx = path_uvprojx_;



	XMLDocument ewp;
	XMLElement* titleElement;
	XMLElement* subElement;
	ewp.LoadFile(path_uvprojx.c_str());
	titleElement = ewp.FirstChildElement("Project");
	emb_assert(titleElement, "格式错误");
	//找到debug那一栏
	titleElement = titleElement->FirstChildElement("Targets");
	emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("Target");
	emb_assert(titleElement, "格式错误");
	subElement = titleElement->FirstChildElement("pCCUsed");
	string aaaa(subElement->GetText());
	a = aaaa.find_first_of(":");
	addPreprocessorDefinitions("__CC_ARM");
	addPreprocessorDefinitions("__ARMCC_VERSION=" + aaaa.substr(0, a));
	
}

EmbPrjKeil::~EmbPrjKeil(void)
{
}

int EmbPrjKeil::generater(void)
{
	searchDefinitions();
	searchIncludePaths();
	searchSourseItems();
	slnGenerate();
	vcxprojGenerate();
	filtersGenerate();
	userGenerate();
	return 0;
}

emb_virtual void EmbPrjKeil::searchDefinitions(void)
{
	XMLDocument ewp;
	XMLElement* titleElement;
	XMLElement* subElement;
	ewp.LoadFile(path_uvprojx.c_str());
	titleElement = ewp.FirstChildElement("Project"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("Targets"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("Target"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("TargetOption"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("TargetArmAds"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("Cads"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("VariousControls"); emb_assert(titleElement, "格式错误");
	subElement = titleElement->FirstChildElement("Define"); emb_assert(subElement, "格式错误");
	string str(subElement->GetText());
	replace_str(str, ",", " ");
	stringstream strr(str);
	while (strr)
	{
		strr >> str;
		addPreprocessorDefinitions(str);
	}

	//void EmbPrjKeil::searchIncludePaths(void)
	subElement = titleElement->FirstChildElement("IncludePath"); emb_assert(subElement, "格式错误");
	string str2(subElement->GetText());
	replace_str(str2, ";", " ");
	stringstream strr2(str2);
	while (strr2)
	{
		strr2 >> str2;
		//cout << str2 << endl;
		addIncludePaths(str2);
	}
}

void EmbPrjKeil::searchIncludePaths(void)
{
	//已经在searchDefinitions实现了
}

void EmbPrjKeil::searchSourseItems(void)
{
	XMLDocument ewp;
	XMLElement* titleElement;
	XMLElement* subElement;
	ewp.LoadFile(path_uvprojx.c_str());
	titleElement = ewp.FirstChildElement("Project"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("Targets"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("Target"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("Groups"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("Group"); emb_assert(titleElement, "格式错误");
	while (titleElement!=nullptr)
	{
		if (titleElement->FirstChildElement("Files"))
		{
			subElement = titleElement->FirstChildElement("Files")->FirstChildElement("File"); emb_assert(subElement, "格式错误");
			while (subElement != nullptr)
			{
				string str(subElement->FirstChildElement("FilePath")->GetText());
				if (str.find_first_of('\\') == 1)
				{
					replace_str(str, ".\\", "");
				}
				addSourseItems(str);
				subElement = subElement->NextSiblingElement("File");
			}
		}
		titleElement = titleElement->NextSiblingElement("Group");
	}

}
