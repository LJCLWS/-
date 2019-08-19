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

//表示每一句X86汇编指令的数据结构
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
	ofstream objectFile;//用于存储生成的目标代码

	int MEM_num=0;
	vector<SymbolListCommon_Type> MainSymbolList;//带有活跃信息的符号表
	unordered_map<string, int> unordere_MainSymbolList;   //存储函数表的临时元素（无序）
	vector<string> RDLMEM;//存储放到内存中的变量名
	map<string, string> RDL;//存储寄存器与变量或常量的映射关系

	vector<objectFile_Type> objectFileTemp;//用于暂存目标代码的数组（主要是处理跳转指令的回填地址）

};


