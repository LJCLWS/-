#include <iostream>
#include <cstdlib>
#include <iterator>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include "symtable.h"
#include <sstream>
#include "parser_.h"
#include "auto_insert.h"
#include "object.h"

using namespace std;

char start_fir[] = "LOG(INFO) << \"start the function:";
char start_mid[] = "func_enter(\"enter ";
char line[] = "/***********************************************************************/";
char suffix[] = "()\";";
char end_fir[] = "LOG(INFO)<<\"end the function : ";
char end_mid[] = "func_exit(\"exit ";
char suffix2[] = "<<<1,1>>>();";
char suffix3[] = "()\");";
int name_def = 0;//函数定义位置
int name_start = 0;//函数开头位置
int name_end = 0;//函数结束位置
char line_break[] = " ";


//读取文件到字符串中
char* read_str(char* filename)
{
	char* data;
	string str;
	ifstream ifile(filename);
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	str = buf.str();
	//cout << str << endl;
	data = change_type(str);
	//data = (char*)str.c_str();
	return data;
}

//将string类型转化为char*
char* change_type(string s)
{
	//cout << sizeof s << endl;
	//cout << s.length() << endl;
	char* data = new char[s.length()+1];
	//cout << sizeof(s.c_str()) << endl;
	//strcpy(data, s.c_str());
	for (int i = 0; i < s.length(); ++i)
	{
		data[i] = s[i];
	}
	//int len = s.length();
	
	//data = (char*)malloc((len) * sizeof(char));
	//s.copy(data, len, 0);
	data[s.length()] = '\0';

	//cout << data << endl;
	//cout << sizeof data << endl;
	return data;
}


int auto_match(char* s, char* t)
{
	//cout << "auto_match" << endl;
	//cout << s << endl;		
	//cout << t << endl<<endl;
	int i = 0, j=0,k=0;
	//cout << sizeof(t) << endl;
	while (s[i] != '\0')
	{
		j = 0;
		k = 0;
		while (s[i + j] && t[k] && (s[i + j] == t[k]||s[i+j]==' '||t[k]==' '))
		{
			if (s[i + j] == t[k])
			{
				j++; k++;
			}
			else if (s[i + j] == ' ')
			{
				j++;
			}	
			else if(t[k]==' ')
			{
				k++;
			}
			
		}
		//if (t[k] == '\0')//匹配成功
		if(k == strlen(t))
		{
			//cout << "匹配位置" << i + j << endl;
			return i + j;
		}
		i++;
	}
	//cout << "匹配失败" << i << endl;
	return 0;//匹配失败
}

//自动插装
char* auto_insert(char* filename, vector<TokenElementType> FuctListQueue, object& par)
{

	char* name_ide =(char*)"namespace";
	char* t;
	char* s;
	s = read_str(filename);
	//TokenElementType fuc_union;
	string fuc_name;
	int k = 0;
	/// cout << s << endl;
	k = auto_match(s, name_ide);
	if (k)
	{
		while (s[k] && s[k] != '{')
		{
			if (s[k] == ';')
				break;
			k++;
		}
		if (s[k] == '{')//定位空间开头
		{
			//cout << "找到namespace" << endl;
			name_def = k;
			s = insert_str(s, line_break, name_def);
			//cout << s << endl;
		}
	}
	else cout << "未找到namespace"<<endl;
	k = 0;
	  
	while (k < FuctListQueue.size())
	{
		//cout << "开始查找函数" << k<<endl;
		fuc_name = par.pop_fuction().spelling;
		//cout << "fuc_name=" << fuc_name << endl;
		//cout << sizeof(fuc_name) << endl;
		//t = change_type(fuc_name);
		t = (char*)fuc_name.c_str();
		//cout <<strlen(t) << endl;
		s = insert_log(s, t);
		k++;
	}
	return s;
}


//插入log
char* insert_log(char* s, char* t)
{
	//cout << "开始执行insert_log" << endl;
	int i = 0, j = 0, k = 0;
	char* fuc = (char*)t;// = new char[100];
	char* str;// = new char[100];
	char* enter;
	char* exit_;
	//memset(fuc, 0, sizeof(char) * 100);auto_insert
	string str_start, str_end;
	string str_ns("__global__ void (){}//xwj"), str_ne("__global__ void (){}//xwj");
	//strcpy(fuc, t);
	/*while (t[i] != '\0')
	{
		fuc[i] = t[i];
		i++;
	}*/
	//cout << "开始匹配" << endl;
	//cout << sizeof fuc << endl;
	//cout << sizeof(t) << endl;
	j = auto_match(s, fuc);
	/*cout << s << endl;
	cout << fuc << endl;
	cout << strcmp(s, fuc) << endl;*/
	//j = strcmp(s, fuc);
	//cout << "匹配结束" << endl;
	//cout << j << endl;
	if (j)
	{
		//cout << "匹配到函数" << fuc << endl;
		while (s[j] && s[j] != '{')
		{
			if (s[j] == ';')
				break;
			j++;
		}
		if (s[j] == '{')//定位函数体开头
		{
			//cout << "定位函数头"<<endl;
			//cout << "j=" << j << endl;
			name_start = j;
			//暂时显示start_log
			//cout << line << endl;
			str = connect(start_fir, t, suffix);//插入第一行log
			enter = connect(start_mid, t, suffix3);
			//cout << "插入第一行log" << endl;
			//cout << str << endl;
			//cout << enter << endl;
			string temp_start(go_to_space((char*)(start_sco(t).c_str())));
			//第二行log
			str_start = temp_start;
			k = 0;
			while (*(suffix2 + k) != '\0')
			{
				str_start.push_back(suffix2[k]);
				k++;
			}
			//cout << "第二行log" << endl;
			//cout << str_start << endl;
			//cout << line << endl;

			//函数首插入字符串
			//cout << "函数首插入字符串" << endl;
			if (auto_match(s, str) == 0)
			{
				s = insert_str(s, line_break, name_start);
				s = insert_str(s, line, name_start);
				s = insert_str(s, go_to_space((char*)(str_start.c_str())), name_start);
				s = insert_str(s, enter, name_start);
				s = insert_str(s, str, name_start);
				s = insert_str(s, line, name_start);
			}
			



			//定位函数体结尾
			//cout << "定位函数体结尾" << endl;
			int count = 1;//函数体最后一个"}"的位置
			j++;
			while (count != 0)
			{
				if (j < strlen(s) && s[j] == '{')
				{
					count++;
					//cout <<s[j-5]<<s[j-4]<<s[j-3]<<s[j-2]<<s[j-1]<<"count="<< count <<"   j="<<j<< endl;
				}
				else if (j < strlen(s) && s[j] == '}')
				{
					count--;
					name_end = j - 1;
					//cout << s[j - 5] << s[j - 4] << s[j - 3] << s[j - 2] << s[j - 1] << "count=" << count << "   j=" << j << endl;
				}
				j++;
			}

			/*//暂时显示end_log
			cout << line << endl;
			str = connect(end_fir, t, suffix);//插入start
			cout << str << endl;
			temp_end = end_sco(t);
			cout << temp_end << endl;
			cout << line << endl;*/

			//暂时显示end_log
			//cout << "end_log" << endl;
			//cout << line << endl;
			str = connect(end_fir, t, suffix);//插入第一行log
			exit_ = connect(end_mid, t, suffix3);
			//cout << str << endl;
			//cout << exit_ << endl;
			string temp_end(go_to_space((char*)(end_sco(t).c_str())));//去空格
			//第二行log
			//cout << "第二行log" << endl;
			str_end = temp_end;
			k = 0;
			while (*(suffix2 + k) != '\0')
			{
				str_end.push_back(suffix2[k]);
				k++;
			}
			//cout << str_end << endl;
			//cout << line << endl;

			//函数尾插入字符串
			//cout << "函数尾插入字符串" << endl;
			if (auto_match(s, str) == 0)
			{
				s = insert_str(s,line_break,name_end);     //插入换行
				s = insert_str(s, line, name_end);
				s = insert_str(s, go_to_space((char*)(str_end.c_str())), name_end);
				s = insert_str(s, exit_, name_end);
				s = insert_str(s, str, name_end);
				s = insert_str(s, line, name_end);
			}
			

			//开头定义结束函数
			//cout << "开头定义结束函数" << endl;
			str_ne.insert(16, temp_end);
			if (auto_match(s, (char*)(str_ne.c_str())) == 0)//若源字符串没有定义过则定义
				s = insert_str(s, (char*)(str_ne.c_str()), name_def);

			//开头定义开始函数；
			//cout << "开头定义开始函数" << endl;
			str_ns.insert(16, temp_start);
			if (auto_match(s, (char*)(str_ns.c_str())) == 0)//若源字符串没有定义过则定义
				s = insert_str(s, (char*)(str_ns.c_str()), name_def);
		}
	}
	else cout <<"fuc=  "<< fuc <<endl<< "匹配失败";                  //fuc有问题
	return s;
}

//插入字符串
char* insert_str(char* s1, char* s2, int n)
{
	int i = 0, j = 0, k = 1;
	char* temp;
	temp = new char[strlen(s1) + strlen(s2) + 2];//动态分配内存问题
	
	while (j <= n)
	{
		temp[j] = s1[j];
		j++;

	}
	temp[j] = '\n';
	while (*(s2 + i) != '\0')
	{
		temp[j + 1] = s2[i];
		j++;
		i++;
	}
	while (*(s1 + n + k) != '\0')
	{
		temp[j + 1] = s1[n + k];
		j++;
		k++;
	}
	temp[j + 1] = '\0';
	//temp[j + 2] = '\0';
	return temp;
}

char* connect(char* a, char* b, char* c)
{
	char* temp = new char[100];
	strcpy(temp, a);
	strcat(temp, b);
	strcat(temp, c);
	return temp;
}


string start_sco(char* t)
{
	int i = 0, j = 0, k = 0;
	string str("start__");
	while (*(t + i) != '\0')
	{
		if (t[i] == '<')
		{
			while (t[i] != '>')
				i++;
			str.push_back('_');
		}
		else if (t[i] == ':')
		{
			i++;
			if (t[i] == ':')
			{
				str.push_back('_');
			}

		}
		else
		{
			str.push_back(t[i]);
		}
		i++;
	}
	/*while (*(suffix2 + j) != '\0')
	{
		str.push_back(suffix2[j]);
		j++;
	}*/
	return str;
}


string end_sco(char* t)
{
	int i = 0, j = 0, k = 0;
	string str("end__");
	while (*(t + i) != '\0')
	{
		if (t[i] == '<')
		{
			while (t[i] != '>')
				i++;
			str.push_back('_');
		}
		else if (t[i] == ':')
		{
			i++;
			if (t[i] == ':')
			{
				str.push_back('_');
			}

		}
		else
		{
			str.push_back(t[i]);
		}
		i++;
	}
	/*while (*(suffix2 + j) != '\0')
	{
		str.push_back(suffix2[j]);
		j++;
	}*/
	return str;
}

//去字符串空格
char* go_to_space(char* v)
{
	char* temp=(char*)v;
	char* re = new char[100];
	int i=0,j=0;
	while (*(temp+i)!='\0')
	{
		if (*(temp + i) != ' ')
		{
			re[j] = temp[i];
			j++;
		}
		i++;
	}
	re[j] = '\0';
	return re;
		
}