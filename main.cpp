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

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
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
	char txt[1024];	// 随便设置
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
	//文件句柄  
	intptr_t hFile = 0;   //win10
	//文件信息  
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
				//如果是目录,迭代之  
				//如果不是,加入列表  
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
	//词法处理机
	result->setstr(file);
	if (result->token_scanner() == EXCEPTION)
		exit(0);
#ifdef  LEXER_OUTPRINT
	//result->Lexer_output();//token序列输出
#endif

	//语法分析器

#ifdef  PARSER_ANALYZE

	try {
		//cout << "语法分析" << endl;
		result->translation_unit();
		
	}
	catch (int str)
	{
		cout << str << endl;
	}
	
#endif


	//自动插装

	
	char* f = const_cast<char*>(file.c_str());
	char* file_insert = auto_insert(f, result->FuctListQueue,*result);
	cout << file_insert << endl;


	//// 写回源文件  
	ofstream fout;
	fout.open(file);//待定
	fout << file_insert << endl;
	fout.close();



	//result->print_fuc();
	cout << "该文件插装完毕" << endl;
	delete result;
}

//文件选择对话框
char* folder_choose()
{
	TCHAR szBuffer[1024] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pidlRoot = CSIDL_DESKTOP;//文件夹的根目录，此处为桌面
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = "从下面选择文件或文件夹:";
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);


	if (NULL == idl)
	{

		return bi.pszDisplayName;
	}

	SHGetPathFromIDList(idl, szBuffer);
	cout << bi.pszDisplayName << endl;//输出路径名
	return bi.pszDisplayName;
}

int main(int argc, char** argv)
{
	//自动获取文件夹下文件路径
	char* get_f=new char[100];
	char* filePath=new char[100];
	char* str;
	int i = 0, j = 0, p = 0;
	vector<string> files;
	//cout << "请输入需自动插装的文件夹路径：";
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

	//获取该路径下的所有文件路径  
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