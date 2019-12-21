#pragma once
#include"pch.h"
using namespace std;
using namespace tinyxml2;
#define emb_assert(expression,...) if(!expression){printf(__VA_ARGS__);printf("\r\n");}assert(expression)
#define emb_virtual
#define MSBuildProjectDirectory "$(MSBuildProjectDirectory)"
class EmbPrj
{
public:
	EmbPrj(string path, string name);
	EmbPrj();
	~EmbPrj();
	void setPathExe(string path);

	//Directory包含最后的那个'/'
	void setPathProjectName(string Directory, string name);
	int generater(void);
protected:
	//预处理器定义
	emb_virtual void searchDefinitions(void);

	//include目录
	emb_virtual  void searchIncludePaths(void);

	//源文件
	emb_virtual void searchSourseItems(void);

	//生成
	int slnGenerate(void);
	int vcxprojGenerate(void);
	int filtersGenerate(void);
	int userGenerate(void);
protected:
	void addPreprocessorDefinitions(string def);

	//不需要开头那个"/"
	void addIncludePaths(string inc);
	void addSourseItems(string src);
	string& getProjectDirectory(void);
	string& getProjectName(void);
	string& getPath_exe(void);
	set< string>& getPreprocessorDefinitions(void);
	set< string>& getIncludePaths(void);
	set< string>* pgetIncludePaths(void);
	set< string>& getSourseItems(void);
private:
	set< string> PreprocessorDefinitions;//预处理器定义
	set< string> IncludePaths;//include目录
	set< string> SourseItems;//源文件

	set< string> SourseGroup;//源文件分组//中间变量
	string path_exe;
	string ProjectDirectory;//项目目录
	string ProjectName;//项目名字
	const string resource = "\\resource\\";
};

class UniqueIdentifier//用来生成//<UniqueIdentifier>{dc2250e4-e44f-4b53-925e-f71d02f20bba}</UniqueIdentifier>类似的字符串
	:public  string
{
public:
	UniqueIdentifier();
	~UniqueIdentifier();
	void Refresh(void);
private:
	static int timeSrandSeed;
};

string& replace_str(string& str, const string& to_replaced, const string& newchars);
void getFiles(string path, vector<string>& files);
