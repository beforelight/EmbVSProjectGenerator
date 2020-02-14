#pragma once
#include "EmbPrj.h"
class EmbPrjIar :
	public EmbPrj
{
public:
	//.eww的完整路径
	EmbPrjIar(string path_eww_, string path_exe_);
	~EmbPrjIar(void);
	int generater(void);

protected:
	//预处理器定义
	emb_virtual void searchDefinitions(void);

	//include目录
	emb_virtual  void searchIncludePaths(void);

	//源文件
	emb_virtual void searchSourseItems(void);

private:
	string path_eww;
	string path_ewp;


};

