#include <iostream>
#include <cstdlib>
#include<iterator>
#include "lexer.h"
#include "parser_.h"
#include "error.h"
#include <queue>
#include "symtable.h"
#include "object.h"
#include <string>
#include <string.h>
#include "auto_insert.h"
#include<fstream>
#include <Windows.h>
#include<io.h>
#include <shlobj.h>
#include "CommDlg.h"

#pragma warning(disable:4996)

using namespace std;


#define PARSER_ANALYZE

extern int terminal_count;
extern int fuction_count;

string UTF8ToGB(const char* str)
{
	string result;
	WCHAR* strSrc;
	LPSTR szRes;

	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//�����ʱ�����Ĵ�С
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

string readfile(char* src)
{
	char txt[1024];	// �������
	string msg;
	ifstream infile;
	infile.open(src);

	if (!infile.is_open())
	{
		cout << "" << endl;
		exit(0);
	}
	string s;
	while (!infile.eof())
	{
		infile.getline(txt, sizeof txt);
		msg = UTF8ToGB(txt);
		// cout << msg << endl;
		s += msg + "\n";
	}
	infile.close();
	return s;
}

void print_path(string f)
{

	int i = 0;
	while (i < f.length())
	{
		cout << f[i];
		i++;
		if (f[i-1] == '\\'&&f[i]=='\\')
		{
			i++;
		}
		
	}
}

void getFiles1(string path, vector<string>& files)
{
	//�ļ����  
	intptr_t hFile = 0;   //win10
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	WIN32_FIND_DATAA FindFileData;
	FindFirstFileA(path.c_str(),&FindFileData);
	if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		
		 if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	     {
			do
			{
				//�����Ŀ¼,����֮  
				//�������,�����б�  
				if ((fileinfo.attrib & _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getFiles1(p.assign(path).append("\\").append(fileinfo.name), files);
				}
				else
				{
					files.push_back(path + "\\" + fileinfo.name);
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
			//_findclose(hFile);
	     }
	}else files.push_back(path);
	
}

void exec(string file)
{

	//object o((char*)str);
	object * result =  new object((char*)file.c_str());
	//�ʷ������
	result->setstr(file);
	if (result->token_scanner() == EXCEPTION)
		exit(0);
#ifdef  LEXER_OUTPRINT
	//result->Lexer_output();//token�������
#endif

	//�﷨������

#ifdef  PARSER_ANALYZE

	try {
		//cout << "�﷨����" << endl;
		result->translation_unit();
		
	}
	catch (int str)
	{
		cout << str << endl;
	}
	
#endif


	//�Զ���װ

	
	char* f = const_cast<char*>(file.c_str());
	char* file_insert = auto_insert(f, result->FuctListQueue,*result);
	cout << file_insert << endl;


	//// д��Դ�ļ�  
	ofstream fout;
	fout.open(file);//����
	fout << file_insert << endl;
	fout.close();



	//result->print_fuc();
	cout << "���ļ���װ���" << endl;
	delete result;
}

//�ļ�ѡ��Ի���
char* folder_choose()
{
	TCHAR szBuffer[1024] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pidlRoot = CSIDL_DESKTOP;//�ļ��еĸ�Ŀ¼���˴�Ϊ����
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = "������ѡ���ļ����ļ���:";
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);


	if (NULL == idl)
	{

		return bi.pszDisplayName;
	}

	SHGetPathFromIDList(idl, szBuffer);
	cout << bi.pszDisplayName << endl;//���·����
	return bi.pszDisplayName;
}

int main(int argc, char** argv)
{
	//�Զ���ȡ�ļ������ļ�·��
	char* get_f=new char[100];
	char* filePath=new char[100];
	char* str;
	int i = 0, j = 0, p = 0;
	vector<string> files;
	//cout << "���������Զ���װ���ļ���·����";
	//char* filePath = (char*)"C:\\Users\\User\\Desktop\\3";
	//cin >> get_f;
	get_f = folder_choose();
	while (get_f[i] != '\0')
	{
		filePath[j] = get_f[i];
		if (get_f[i] == '\\')
		{
			j++;
			filePath[j] = '\\';
		}
		i++;
		j++;
	}
	filePath[j] = '\0';

	//��ȡ��·���µ������ļ�·��  
	getFiles1(filePath, files);

	for (int i = 0; i < files.size(); i++)
	{
		if (files[i].find(".cu") != files[i].npos)
		{
			
			cout << endl << endl ;
			print_path(files[i]) ;
			cout << endl;
			terminal_count = 0;
			exec(files[i]);
			
		}
	}
return 0;
}