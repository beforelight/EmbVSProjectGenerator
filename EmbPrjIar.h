#pragma once
#include "EmbPrj.h"
class EmbPrjIar :
	public EmbPrj
{
public:
	//.eww������·��
	EmbPrjIar(string path_eww_, string path_exe_);
	~EmbPrjIar(void);
	int generater(void);

protected:
	//Ԥ����������
	emb_virtual void searchDefinitions(void);

	//includeĿ¼
	emb_virtual  void searchIncludePaths(void);

	//Դ�ļ�
	emb_virtual void searchSourseItems(void);

private:
	string path_eww;
	string path_ewp;


};

