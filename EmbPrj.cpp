#include "pch.h"
#include "EmbPrj.h"

EmbPrj::EmbPrj(string path, string name) :ProjectDirectory(path), ProjectName(name)
{
	addPreprocessorDefinitions("__ARMVFP__");
}

EmbPrj::EmbPrj()
{
	//addPreprocessorDefinitions("__ARMVFP__");
}

EmbPrj::~EmbPrj()
{
	PreprocessorDefinitions.clear();
	IncludePaths.clear();
	SourseGroup.clear();
	SourseItems.clear();
}


void EmbPrj::setPathExe(string path)
{
	int a = replace_str(path, "/", "\\").find_last_of("\\");
	path_exe = path.substr(0, a);
}

void EmbPrj::setPathProjectName(string Directory, string name)
{
	ProjectDirectory = Directory;
	ProjectName = name;
}

int EmbPrj::generater(void)
{
	return 0;
}

void EmbPrj::addPreprocessorDefinitions(string def)
{
	PreprocessorDefinitions.insert(def);
}

void EmbPrj::addIncludePaths(string inc)
{
	IncludePaths.insert(inc);
}

void EmbPrj::addSourseItems(string src)
{
	SourseItems.insert(src);
}

string& EmbPrj::getProjectDirectory(void)
{
	return ProjectDirectory;
}

string& EmbPrj::getProjectName(void)
{
	return ProjectName;
	// TODO: 在此处插入 return 语句
}

string& EmbPrj::getPath_exe(void)
{
	return path_exe;
	// TODO: 在此处插入 return 语句
}

set<string>& EmbPrj::getPreprocessorDefinitions(void)
{
	return PreprocessorDefinitions;
	// TODO: 在此处插入 return 语句
}

set<string>& EmbPrj::getIncludePaths(void)
{
	return IncludePaths;
	// TODO: 在此处插入 return 语句
}

set<string>& EmbPrj::getSourseItems(void)
{
	return SourseItems;
	// TODO: 在此处插入 return 语句
}

int EmbPrj::slnGenerate(void)
{
	//准备文件
	stringstream ss;
	string buf;
	//char buff[1000];
	ofstream sinNew(ProjectDirectory + ProjectName + ".sln");
	ifstream sinTemp(path_exe + resource + "EmbProjectTemplate.sln");
	emb_assert(sinNew.is_open(), "文件打不开");
	emb_assert(sinTemp.is_open(), "文件打不开");
	ss << sinTemp.rdbuf();
	buf = ss.str();
	replace_str(buf, "EmbProjectTemplate", ProjectName);
	//cout << endl << endl << buf;
	//stringstream sss(buf);
	sinNew << buf;
	sinTemp.close();
	sinNew.close();
	return 0;
}

int EmbPrj::vcxprojGenerate(void)
{
	//准备文件
	stringstream ss;
	string buf;
	//char buff[1000];
	ofstream sinNew(ProjectDirectory + ProjectName + ".vcxproj");
	ifstream sinTemp(path_exe + resource + "EmbProjectTemplate.vcxproj");
	emb_assert(sinNew.is_open(), "文件打不开");
	emb_assert(sinTemp.is_open(), "文件打不开");
	ss << sinTemp.rdbuf();
	buf = ss.str();
	replace_str(buf, "EmbProjectTemplate", ProjectName);
	//cout << endl << endl << buf;
	//stringstream sss(buf);
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
	ewp.LoadFile((ProjectDirectory + ProjectName + ".vcxproj").c_str());

	titleElement = ewp.FirstChildElement("Project")->LastChildElement("PropertyGroup");
	assert(titleElement);
	for (size_t i = 0; i < 4; i++)
	{
		subElement = titleElement->FirstChildElement("NMakePreprocessorDefinitions");
		buf = subElement->GetText();
		for (set<string>::iterator i = PreprocessorDefinitions.begin(); i != PreprocessorDefinitions.end(); i++)
		{
			buf += ";";
			buf += *i;
		}
		subElement->SetText(buf.c_str());
		newElement = ewp.NewElement("IncludePath");
		buf.clear();
		for (set<string>::iterator i = IncludePaths.begin(); i != IncludePaths.end(); i++)
		{
			buf += "$(MSBuildProjectDirectory)/";
			string ibuf(*i);
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
	assert(titleElement);
	//buf.clear();
	for (set<string>::iterator i = SourseItems.begin(); i != SourseItems.end(); i++)
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
	assert(titleElement);
	//buf.clear();
	for (set<string>::iterator i = SourseItems.begin(); i != SourseItems.end(); i++)
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
	assert(titleElement);
	//buf.clear();
	for (set<string>::iterator i = SourseItems.begin(); i != SourseItems.end(); i++)
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

	ewp.SaveFile((ProjectDirectory + ProjectName + ".vcxproj").c_str());
	return 0;
}

int EmbPrj::filtersGenerate(void)
{
	//准备文件
	stringstream ss;
	string buf;
	//char buff[1000];
	ofstream sinNew(ProjectDirectory + ProjectName + ".vcxproj.filters");
	ifstream sinTemp(path_exe + resource + "EmbProjectTemplate.vcxproj.filters");
	emb_assert(sinNew.is_open(), "文件打不开");
	emb_assert(sinTemp.is_open(), "文件打不开");
	ss << sinTemp.rdbuf();
	buf = ss.str();
	replace_str(buf, "EmbProjectTemplate", ProjectName);
	//cout << endl << endl << buf;
	//stringstream sss(buf);
	sinNew << buf;
	sinTemp.close();
	sinNew.close();


	//准备筛选器
	int a = 0;
	for (set<string>::iterator i = SourseItems.begin(); i != SourseItems.end(); i++)
	{
		string ibuf(*i);
		string b = replace_str(ibuf, "/", "\\");
		b = replace_str(b, "..\\", "");
		if (b.size())
		{
			while ((a = b.find("\\", a + 1)) != string::npos)
			{
				SourseGroup.insert(b.substr(0, a));
			}
		}

		//cout <<  << endl;
	}

	//for (set<string>::iterator i = SourseGroup.begin(); i != SourseGroup.end(); i++)
	//{
	//	cout << (*i) << endl;
	//}

	//进行xml修改
	XMLDocument ewp;
	XMLElement* titleElement;
	XMLElement* subElement;
	XMLElement* newElement;
	XMLElement* newElement2;
	ewp.LoadFile((ProjectDirectory + ProjectName + ".vcxproj.filters").c_str());

	//1.
	titleElement = ewp.FirstChildElement("Project")->LastChildElement("ItemGroup");
	assert(titleElement);
	titleElement->DeleteChildren();
	for (set<string>::iterator i = SourseGroup.begin(); i != SourseGroup.end(); i++)
	{
		newElement = ewp.NewElement("Filter");
		string ibuf(*i);
		replace_str(ibuf, "..\\", "");
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
	assert(titleElement);
	//buf.clear();
	for (set<string>::iterator i = SourseItems.begin(); i != SourseItems.end(); i++)
	{
		if (-1 != (*i).find(".c"))
		{
			newElement = ewp.NewElement("ClCompile");
			string ibuf(*i);
			newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
			string b = replace_str(ibuf, "/", "\\");
			replace_str(b, "..\\", "");
			if (b.find("\\") != string::npos)
			{
				newElement2 = ewp.NewElement("Filter");
				int a = b.find_last_of("\\");
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
	assert(titleElement);
	//buf.clear();
	for (set<string>::iterator i = SourseItems.begin(); i != SourseItems.end(); i++)
	{
		if (-1 != (*i).find(".h"))
		{
			newElement = ewp.NewElement("ClInclude");
			string ibuf(*i);
			newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
			string b = replace_str(ibuf, "/", "\\");
			replace_str(b, "..\\", "");
			if (b.find("\\") != string::npos)
			{
				newElement2 = ewp.NewElement("Filter");
				int a = b.find_last_of("\\");
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
	assert(titleElement);
	//buf.clear();
	for (set<string>::iterator i = SourseItems.begin(); i != SourseItems.end(); i++)
	{
		if (!(-1 != (*i).find(".h")
			|| -1 != (*i).find(".c")))
		{
			newElement = ewp.NewElement("None");
			string ibuf(*i);
			newElement->SetAttribute("Include", replace_str(ibuf, "/", "\\").c_str());
			string b = replace_str(ibuf, "/", "\\");
			replace_str(b, "..\\", "");
			if (b.find("\\") != string::npos)
			{
				newElement2 = ewp.NewElement("Filter");
				int a = b.find_last_of("\\");
				b = b.substr(0, a);
				newElement2->SetText(b.c_str());
				newElement->InsertEndChild(newElement2);
			}
			titleElement->InsertEndChild(newElement);
		}
	}

	ewp.SaveFile((ProjectDirectory + ProjectName + ".vcxproj.filters").c_str());
	return 0;
}

int EmbPrj::userGenerate(void)
{
	//准备文件
	stringstream ss;
	string buf;
	//char buff[1000];
	ofstream sinNew(ProjectDirectory + ProjectName + ".vcxproj.user");
	ifstream sinTemp(path_exe + resource + "EmbProjectTemplate.vcxproj.user");
	emb_assert(sinNew.is_open(), "文件打不开");
	emb_assert(sinTemp.is_open(), "文件打不开");
	ss << sinTemp.rdbuf();
	buf = ss.str();
	replace_str(buf, "EmbProjectTemplate", ProjectName);
	//cout << endl << endl << buf;
	//stringstream sss(buf);
	sinNew << buf;
	sinTemp.close();
	sinNew.close();
	return 0;
}

UniqueIdentifier::UniqueIdentifier()
{
	if (timeSrandSeed == 0)
	{
		srand(time(NULL));
		timeSrandSeed = 1;
	}
	Refresh();
}

UniqueIdentifier::~UniqueIdentifier()
{
}

const char randchar[] = { "0123456789abcdef" };
void UniqueIdentifier::Refresh(void)
{
	string::clear();
	//<UniqueIdentifier>{dc2250e4-e44f-4b53-925e-f71d02f20bba}</UniqueIdentifier>
	string::append(1, '{');
	for (size_t i = 0; i < 8; i++)
	{
		int a = rand() % strlen(randchar);
		string::append(1, randchar[a]);
	}

	string::append("-");
	for (size_t i = 0; i < 4; i++)
	{
		int a = rand() % strlen(randchar);
		string::append(1, randchar[a]);
	}

	string::append("-");
	for (size_t i = 0; i < 4; i++)
	{
		int a = rand() % strlen(randchar);
		string::append(1, randchar[a]);
	}

	string::append("-");
	for (size_t i = 0; i < 4; i++)
	{
		int a = rand() % strlen(randchar);
		string::append(1, randchar[a]);
	}

	string::append("-");
	for (size_t i = 0; i < 12; i++)
	{
		int a = rand() % strlen(randchar);
		string::append(1, randchar[a]);
	}

	string::append(1, '}');
}
int UniqueIdentifier::timeSrandSeed = 0;


string& replace_str(string& str, const string& to_replaced, const string& newchars)
{
	for (string::size_type pos(0); pos != string::npos; pos += newchars.length())
	{
		pos = str.find(to_replaced, pos);
		if (pos != string::npos)
			str.replace(pos, to_replaced.length(), newchars);
		else
			break;
	}
	return   str;
}

void getFiles(string path, vector<string>& files)
{
	intptr_t   hFile = 0;//文件句柄，过会儿用来查找
	struct _finddata_t fileinfo;//文件信息
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		//如果查找到第一个文件
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//如果是文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else//如果是文件
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0); //能寻找到其他文件

		_findclose(hFile);  //结束查找，关闭句柄
	}
}