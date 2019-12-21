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

	//Directory���������Ǹ�'/'
	void setPathProjectName(string Directory, string name);
	int generater(void);
protected:
	//Ԥ����������
	emb_virtual void searchDefinitions(void);

	//includeĿ¼
	emb_virtual  void searchIncludePaths(void);

	//Դ�ļ�
	emb_virtual void searchSourseItems(void);

	//����
	int slnGenerate(void);
	int vcxprojGenerate(void);
	int filtersGenerate(void);
	int userGenerate(void);
protected:
	void addPreprocessorDefinitions(string def);

	//����Ҫ��ͷ�Ǹ�"/"
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
	set< string> PreprocessorDefinitions;//Ԥ����������
	set< string> IncludePaths;//includeĿ¼
	set< string> SourseItems;//Դ�ļ�

	set< string> SourseGroup;//Դ�ļ�����//�м����
	string path_exe;
	string ProjectDirectory;//��ĿĿ¼
	string ProjectName;//��Ŀ����
	const string resource = "\\resource\\";
};

class UniqueIdentifier//��������//<UniqueIdentifier>{dc2250e4-e44f-4b53-925e-f71d02f20bba}</UniqueIdentifier>���Ƶ��ַ���
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
