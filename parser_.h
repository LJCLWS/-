#pragma once

#include <stack>
#include <vector>
#include "lexer.h"
#include "error.h"
#include "symtable.h"
#include <string>

using namespace std;

#define IF_END -1
#define WHILE_END -2

extern int terminal_count;
extern int fuction_count ;


typedef struct FourQtType
{
	string operation;
	SymbolListCommon_Type aaa;
	SymbolListCommon_Type bbb;
	SymbolListCommon_Type ttt;
}FourQtType;


class parser :public lexer
{
public:
	int count;
	parser(char* fin) :lexer(fin), count(0) {};
	virtual ~parser() {};

	//语法分析器的初始化，包括结尾标识符入栈和起始标志入栈
	virtual int pop_terminal();
	//把token序列的元素对应到文法表示的终结符

	virtual TokenElementType pop_fuction();
	void print_fuc();
	int check_colon(string str);


	//语法分析入口
	int translation_unit();
	int external_declaration();
	//int declaration();
	int declarator();
	int initializer();
	int parameter_type_list();
	

	

protected:
	vector<FourQtType> TempQT;  //四元式区
	bool global = 1;

private:
	int temp_type = 0;        //构建符号表用来临时保存标识符类型
	TokenElementType temp_id;       //用来临时保存函数名
	TokenElementType temp_terminal; //存放终结符的临时变量 
	ExceptionClass error;        //异常类对象

	TokenElementType FuctEle;

	stack<TokenElementType> TempSem;     //语义栈


	TokenElementType TempSem_top;
	TokenElementType TempSem_second_top;
	TokenElementType TempSem_temp_;
};
