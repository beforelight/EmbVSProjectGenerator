#pragma once
#include "EmbPrj.h"
class EmbPrjKeil :
	public EmbPrj
{
public:
	//.eww������·��
	EmbPrjKeil(string path_uvprojx_, string path_exe_);
	~EmbPrjKeil(void);
	int generater(void);
protected:
	//Ԥ����������
	emb_virtual void searchDefinitions(void);

	//includeĿ¼
	emb_virtual  void searchIncludePaths(void);

	//Դ�ļ�
	emb_virtual void searchSourseItems(void);

private:
	string path_uvprojx;
};

