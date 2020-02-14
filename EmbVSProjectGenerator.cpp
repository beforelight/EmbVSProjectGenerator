// EmbVSProjectGenerater.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "EmbPrjIar.h"
#include "EmbPrjKeil.h"
#include "EmbPrjMcux.h"
using namespace std;
using namespace tinyxml2;
int main(int argc, char* argv[], char* envp[])
{
	system("chcp 65001");//修改控制台编码为utf8
	string rec_path;
	//cout << "中文测试" << endl;
	printf("Total %d arguments\n", argc);
	if (argc <= 1)
	{
		cout << "输入路径" << endl;
		cout << "例如 C:\\Users\\17616\\Documents\\SmartCar\\hello_emb\\hello_world.eww" << endl
			<< "C:\\Users\\17616\\Documents\\Tencent Files\\1761690868\\FileRecv\\stm32_can\\USER\\LED.uvprojx" << endl;
		cin >> rec_path;
	}
	else
	{
		rec_path = argv[1];
	}
	cout << rec_path << endl;
	if (rec_path.find(".eww") != string::npos)
	{
		EmbPrjIar prj_iar(rec_path, argv[0]);
		return prj_iar.generater();
	}
	else if (rec_path.find(".uvprojx") != string::npos)
	{
		EmbPrjKeil prj_mdk(rec_path, argv[0]);
		return prj_mdk.generater();
	}
	else if (rec_path.find(".project") != string::npos || rec_path.find(".cproject") != string::npos)
	{
		EmbPrjMcux prj_mcux(rec_path, argv[0]);
		return prj_mcux.generater();
	}
	else
	{
		cout << "输入正确路径" << endl;
	}
	system("pause");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
void generate_XML(string path);
void generate_XML(string path)
{
	XMLDocument doc;
	XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	doc.InsertFirstChild(dec);

	//创建算法主节点
	XMLElement* ALgorithm = doc.NewElement("Algorithm");
	doc.InsertEndChild(ALgorithm);

	//创建算法分支点
	XMLElement* D1 = doc.NewElement("D1");
	ALgorithm->InsertEndChild(D1);

	//创建滤波算法分支点
	XMLElement* D1_filter = doc.NewElement("D1_filter");
	D1->InsertEndChild(D1_filter);

	//创建中值滤波算法分支点
	XMLElement* median_blur = doc.NewElement("median_blur");
	median_blur->SetAttribute("width", "3");
	median_blur->SetAttribute("cut_boundary", "3");
	D1_filter->InsertEndChild(median_blur);

	//创建均值滤波算法分支点
	XMLElement* aver_blur = doc.NewElement("aver_blur");
	aver_blur->SetAttribute("width", "3");
	D1_filter->InsertEndChild(aver_blur);

	const char* path_save = path.c_str();
	doc.SaveFile(path_save, false);
}
