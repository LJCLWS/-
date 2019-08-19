#pragma once
#include <stack>
#include <queue>
#include "parser_.h"
#include "symtable.h"
#include "error.h"
#include <fstream>

using namespace std;


#define ACTIVE     0
#define UNACTIVE   -1
#define A_NULL      -2

//��ʾÿһ��X86���ָ������ݽṹ
typedef struct objectFile_Type
{
	string op;
	string r1;
	string r2;
}objectFile_Type;

class object :public parser
{
public:
	object(char * src) :parser(src) {};
	virtual ~object() {};

	void init_active_inf();
	void writeFile();
	int object_CODE(string Reg, string Reg1);

	//int BACK(pi, pk);
	string LOCR(string var);
	string is_EMPTY();
	void GETR(FourQtType qt_Ele, string &R1, string &B1, string &C1);
	void Result_Code();

	void objectFileTemp_insert(string op,string r1,string r2);

	void MainSymbolList_print(void);

	//void FourQT_print(void);
private:
	ofstream objectFile;//���ڴ洢���ɵ�Ŀ�����

	int MEM_num=0;
	vector<SymbolListCommon_Type> MainSymbolList;//���л�Ծ��Ϣ�ķ��ű�
	unordered_map<string, int> unordere_MainSymbolList;   //�洢���������ʱԪ�أ�����
	vector<string> RDLMEM;//�洢�ŵ��ڴ��еı�����
	map<string, string> RDL;//�洢�Ĵ��������������ӳ���ϵ

	vector<objectFile_Type> objectFileTemp;//�����ݴ�Ŀ���������飨��Ҫ�Ǵ�����תָ��Ļ����ַ��

};


