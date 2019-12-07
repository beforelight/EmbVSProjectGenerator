#include "pch.h"
#include "EmbPrjIar.h"


EmbPrjIar::EmbPrjIar(string path_eww_, string path_exe_)
{
	setPathExe(path_exe_);
	addPreprocessorDefinitions("__ICCARM__");
	addPreprocessorDefinitions("__ARMVFP__");
	int a = path_eww_.find_last_of('\\');
	int b = path_eww_.find_last_of('/');
	int c = path_eww_.find_last_of('.');
	a = _Max_value(a, b);
	string path = path_eww_.substr(0, a + 1);//���������Ǹ�'/'
	string name = path_eww_.substr(a + 1, c - a - 1);
	setPathProjectName(path, name);

	path_eww = path_eww_;
	//����path_ewp
	XMLDocument eww;
	eww.LoadFile(path_eww.c_str());
	XMLElement* titleElement = eww.FirstChildElement("workspace")
		->FirstChildElement("project")
		->FirstChildElement("path");
	const char* title = titleElement->GetText();
#define Text "$WS_DIR$/"
	string ewp_path(title);
	path_ewp = path + ewp_path.substr(strlen(Text), ewp_path.size() - strlen(Text));
#undef Text
}

EmbPrjIar::~EmbPrjIar(void)
{

}

void EmbPrjIar::searchDefinitions(void)
{
	XMLDocument ewp;
	XMLElement* titleElement;
	XMLElement* subElement;
	ewp.LoadFile(path_ewp.c_str());
	titleElement = ewp.FirstChildElement("project");
	emb_assert(titleElement, "��ʽ����");
	//�ҵ�debug��һ��
	titleElement = titleElement->FirstChildElement("configuration");
	emb_assert(titleElement, "��ʽ����");
	subElement = titleElement->FirstChildElement("name");
	// cout << subElement->GetText() <<  endl;
	while (("debug" != string(subElement->GetText()))&& ("Debug" != string(subElement->GetText())))
	{
		titleElement = titleElement->NextSiblingElement("configuration");
		//emb_assert(titleElement, "��ʽ����");
		if (titleElement == nullptr)
		{
			titleElement = ewp.FirstChildElement("project")->FirstChildElement("configuration");
			break;
		}
		subElement = titleElement->FirstChildElement("name");
	}

	//�ҵ��궨����һ��
	titleElement = titleElement->FirstChildElement("settings");
	emb_assert(titleElement, "��ʽ����");
	subElement = titleElement->FirstChildElement("name");
	while ("ICCARM" != string(subElement->GetText()))
	{
		titleElement = titleElement->NextSiblingElement("settings");
		emb_assert(titleElement, "��ʽ����");
		subElement = titleElement->FirstChildElement("name");
	}

	//�ҵ�options��һ��
	titleElement = titleElement->FirstChildElement("data");
	emb_assert(titleElement, "��ʽ����");
	titleElement = titleElement->FirstChildElement("option");
	emb_assert(titleElement, "��ʽ����");
	subElement = titleElement->FirstChildElement("name");
	while ("CCDefines" != string(subElement->GetText()))
	{
		titleElement = titleElement->NextSiblingElement("option");
		emb_assert(titleElement, "��ʽ����");
		subElement = titleElement->FirstChildElement("name");
	}

	//�ҵ�def��
	subElement = titleElement->FirstChildElement("state");
	emb_assert(subElement, "��ʽ����");
	while (subElement)
	{
		// cout << subElement->GetText() <<  endl;
		addPreprocessorDefinitions(subElement->GetText());
		subElement = subElement->NextSiblingElement("state");
	}
}

void EmbPrjIar::searchIncludePaths(void)
{
	XMLDocument ewp;
	XMLElement* titleElement;
	XMLElement* subElement;
	ewp.LoadFile(path_ewp.c_str());
	titleElement = ewp.FirstChildElement("project");
	emb_assert(titleElement, "��ʽ����");
	//�ҵ�debug��һ��
	titleElement = titleElement->FirstChildElement("configuration");
	emb_assert(titleElement, "��ʽ����");
	subElement = titleElement->FirstChildElement("name");
	// cout << subElement->GetText() <<  endl;
	while (("debug" != string(subElement->GetText())) && ("Debug" != string(subElement->GetText())))
	{
		titleElement = titleElement->NextSiblingElement("configuration");
		//emb_assert(titleElement, "��ʽ����");
		if (titleElement == nullptr)
		{
			titleElement = ewp.FirstChildElement("project")->FirstChildElement("configuration");
			break;
		}
		subElement = titleElement->FirstChildElement("name");
	}

	//�ҵ��궨����һ��
	titleElement = titleElement->FirstChildElement("settings");
	emb_assert(titleElement, "��ʽ����");
	subElement = titleElement->FirstChildElement("name");
	while ("ICCARM" != string(subElement->GetText()))
	{
		titleElement = titleElement->NextSiblingElement("settings");
		emb_assert(titleElement, "��ʽ����");
		subElement = titleElement->FirstChildElement("name");
	}

	//�ҵ�options��һ��
	titleElement = titleElement->FirstChildElement("data");
	emb_assert(titleElement, "��ʽ����");
	titleElement = titleElement->FirstChildElement("option");
	emb_assert(titleElement, "��ʽ����");
	subElement = titleElement->FirstChildElement("name");
	while ("CCIncludePath2" != string(subElement->GetText()))
	{
		titleElement = titleElement->NextSiblingElement("option");
		emb_assert(titleElement, "��ʽ����");
		subElement = titleElement->FirstChildElement("name");
	}

	//�ҵ�include��
	subElement = titleElement->FirstChildElement("state");
	emb_assert(subElement, "��ʽ����");

#define Text "$PROJ_DIR$/"
	while (subElement)
	{
		// cout << subElement->GetText() <<  endl;
		string buf(subElement->GetText());
		buf = buf.substr(strlen(Text), buf.size() - strlen(Text));
		// cout << buf <<  endl;
		addIncludePaths(buf);
		subElement = subElement->NextSiblingElement("state");
	}
#undef Text
}

void EmbPrjIar::searchSourseItems(void)
{
	XMLDocument ewp;
	XMLElement* titleElement;
	XMLNode* titleNode;
	XMLElement* subElement;
	//XMLNode* subNode;
	ewp.LoadFile(path_ewp.c_str());
	titleElement = ewp.FirstChildElement("project");
	emb_assert(titleElement, "��ʽ����");
	titleElement = titleElement->FirstChildElement("group");
	emb_assert(titleElement, "��ʽ����");
	titleElement->Parent();

	titleNode = ewp.FirstChildElement("project")->FirstChild();
	while (strcmp(titleNode->Value(), "group"))//��ͬʱ�Ƿ���0
	{
		//printf(titleNode->Value());
		//printf("\r\n");
		titleNode = titleNode->NextSibling();
	}
	//����titleNode�Ѿ��ƶ�����һ��group������
	//��������
	//1.������������
	//2.child���������սڵ㣬next�������սڵ�
	int next = 0;//��Ҫnext
	string source_name;
	source_name.clear();
#define Text "$PROJ_DIR$\\"
	while (1)
	{
		next = 0;
		if (titleNode == nullptr)
		{
			break;
		}
		else if (strcmp(titleNode->Value(), "group") == 0)
		{
			titleNode = titleNode->FirstChild();
		}
		else if (strcmp(titleNode->Value(), "name") == 0)
		{
			next = 1;
		}
		else if (strcmp(titleNode->Value(), "file") == 0)
		{
			source_name = titleNode->FirstChildElement("name")->GetText();
			source_name = source_name.substr(strlen(Text), source_name.size() - strlen(Text));
			// cout << source_name <<  endl;
			addSourseItems(source_name);
			next = 1;
		}
		else
		{
			break;
		}

		if (next)
		{
			while (1)
			{
				if (titleNode->NextSibling())//���next����
				{
					titleNode = titleNode->NextSibling();//��ת�ɹ�
					break;
				}
				else//������
				{
					titleNode = titleNode->Parent();
					if (titleNode == nullptr) { break; }
				}
			}

		}
	}
#undef Text
}

int EmbPrjIar::generater(void)
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
