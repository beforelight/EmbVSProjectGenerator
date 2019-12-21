#pragma once
#include "EmbPrj.h"
class EmbPrjMcux :
	public EmbPrj
{
public:

	EmbPrjMcux(string path_project_, string path_exe_);
	~EmbPrjMcux(void);
	int generater(void);
	int cmakeGenner(void);
	static int file_exists(const char* filename);
protected:
	void mcux11_1_0_LaterVersion(void);

	//预处理器定义
	emb_virtual void searchDefinitions(void);

	//include目录
	emb_virtual  void searchIncludePaths(void);

	//源文件
	emb_virtual void searchSourseItems(void);

	void searchCmake(void);
	void searchDefDebugRelease(void);
private:
	set<string> sourceEntries;
	string path_project;
	string path_cproject;


	set< string> DefDebug;//预处理器定义
	set< string> DefRelease;//预处理器定义


	const string cmakexml = "\\resource\\CmakeConfig.xml";
	set< string> cmake_asm_flags_debug;
	set< string> cmake_asm_flags_release;
	set< string> cmake_c_flags_debug;
	set< string> cmake_c_flags_release;
	set< string> cmake_cxx_flags_debug;
	set< string> cmake_cxx_flags_release;
	list< string> cmake_exe_linker_flags_debug;
	list< string> cmake_exe_linker_flags_release;
	set< string> target_link_libraries;


};

