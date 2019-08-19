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

	//�﷨�������ĳ�ʼ����������β��ʶ����ջ����ʼ��־��ջ
	virtual int pop_terminal();
	//��token���е�Ԫ�ض�Ӧ���ķ���ʾ���ս��

	virtual TokenElementType pop_fuction();
	void print_fuc();
	int check_colon(string str);


	//�﷨�������
	int translation_unit();
	int external_declaration();
	//int declaration();
	int declarator();
	int initializer();
	int parameter_type_list();
	

	

protected:
	vector<FourQtType> TempQT;  //��Ԫʽ��
	bool global = 1;

private:
	int temp_type = 0;        //�������ű�������ʱ�����ʶ������
	TokenElementType temp_id;       //������ʱ���溯����
	TokenElementType temp_terminal; //����ս������ʱ���� 
	ExceptionClass error;        //�쳣�����

	TokenElementType FuctEle;

	stack<TokenElementType> TempSem;     //����ջ


	TokenElementType TempSem_top;
	TokenElementType TempSem_second_top;
	TokenElementType TempSem_temp_;
};
