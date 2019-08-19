#include "lexer.h"
#include "parser_.h"
#include <stack>
#include <queue>
#include <vector>
#include <iostream>
#include "object.h"
#include "error.h"

using namespace std;

int terminal_count = 0;
int fuction_count = 0;

int parser::pop_terminal()
{
	//token序号出队列
	//temp_terminal = TokenListQueue.front();
	//static int j = 0;
	//cout << terminal_count << endl;
	//cout << TokenListQueue.size() << endl;
	//if (j < TokenListQueue.size())
	if (terminal_count < TokenListQueue.size())
	{
		temp_terminal = TokenListQueue[terminal_count];
		terminal_count++;
		//cout <<"********************"<< temp_terminal.spelling << endl;
	}
	//TokenListQueue.pop();

	return temp_terminal.token_code;
}

//by ljc
TokenElementType parser::pop_fuction()
{
	//static int j = FuctListQueue.size();
	if (fuction_count > 0)
	{
		FuctEle = FuctListQueue[fuction_count - 1];
		//cout <<fuction_count<< endl;
		fuction_count--;
	}

	return FuctEle;
}

void parser::print_fuc()
{
	int k = 0;
	while (k < FuctListQueue.size())
	{

		cout << pop_fuction().spelling << endl;
		k++;
	}
}


int parser::check_colon(string str)
{
	int i=0;
	int position_f = 0,position_b = 0;
	while ((i < str.length()) && (str[i]!=':'))
	{
		i++;
	}
	i++;
	while(i < str.length() && ((str[i]==':')||(str[i]==' ')))
	{
		i++;
	}
	position_f = str.find("Forward_gpu");
	position_b = str.find("Backward_gpu");
	if (position_f != str.npos)
	{
		if (position_f == i)
		{
			return 1;
		}
		else return 0;
	}
	else if (position_b != str.npos)
	{
		if (position_b == i)
		{
			return 1;
		}
		else return 0;
	}
	else 
	return 0;
}

//递归下降子程序法

//递归下降主程序
int parser::translation_unit()
{
	static int is_first = 0;
	if (is_first == 0)
	{
		//cout << "pop_terminal()" << endl;
		pop_terminal();
		is_first++;
	}
	//cout << "external_declaration()" << endl;
	external_declaration();
	//cout << "pop_terminal()" << endl;
	pop_terminal();
	//cout<<"******************"<< temp_terminal.token_code << "********"<<temp_terminal.spelling<<endl;
	if (temp_terminal.token_code == EOF)
	{
		cout << endl << "syntax alnayze successful" << endl;
		fuction_count = FuctListQueue.size();
		return 0;//结束
	}
	else
	{
		translation_unit();
	}
}

int parser::external_declaration()
{
	/*if (temp_terminal.token_code == KW_const) //常量
	{
		temp_type = temp_terminal.token_code;
		pop_terminal();
		if (temp_terminal.sytax_code == type_specifier)
		{
			pop_terminal();
			if (temp_terminal.token_code == IDentifier)
			{
				temp_id = temp_terminal;

				try
				{
					//插入公共总表
					if (SymbolTable.CommonListElement_insert(temp_id,temp_type, const_value,0));
					else throw false;
				}
				catch (bool error)
				{
					if (error == false)
					{
						cout << endl << "变量重复定义" << endl;
						exit(0);
					}
					else;
				}

				pop_terminal();
				if (temp_terminal.token_code == TK_ASSIGN)pop_terminal();
				else throw START_ERROR;
				if (temp_terminal.token_code == digit_constant || temp_terminal.token_code == character_constant
					|| temp_terminal.token_code == string_literal)
				{
					//插入常量表
					SymbolTable.ConstElement_insert(temp_terminal);
					pop_terminal();
				}
				else throw START_ERROR;
				if (temp_terminal.token_code == TK_SEMICOLON)pop_terminal();
				else throw START_ERROR;
			}
			else throw START_ERROR;
		}
		else throw START_ERROR;
	}
	else          //函数有关语法
	{*/
	//declaration();
	//by ljc
	//char* spellingtmp = new char[1024];
	//cout << temp_terminal.print() << endl;
	//cout << temp_terminal.sytax_code << "\t" << temp_terminal.token_code << endl;
	//if (temp_terminal.sytax_code != 0 && temp_terminal.token_code != 0)
	char* spellingtmp = (char*)(temp_terminal.spelling.c_str());
	if (spellingtmp=="__global__")
	{
		global = 0;
		pop_terminal();
	}
	if (temp_terminal.sytax_code == type_specifier)
	{
		//cout << "分类" << endl;
		temp_type = temp_terminal.token_code;
		pop_terminal();
		if (temp_terminal.token_code == IDentifier)
		{
			//cout << "标识符" << endl;
			temp_id = temp_terminal;

			/*****************添加******************/
			pop_terminal();

			//去空格
			//cout << "去空格" << endl;
			while (temp_terminal.token_code == IDentifier)
			{
				temp_id.spelling.append(" ");
				temp_id.spelling.append(temp_terminal.spelling);
				pop_terminal();
			}


			//插入函数值表

			if (temp_terminal.token_code == TK_OPENPA)
			{
				//cout << "左括号" << endl;
				pop_terminal();
				if (temp_terminal.token_code != TK_CLOSEPA)
				{
					//cout << "参数" << endl;
					parameter_type_list();
					if (temp_terminal.token_code == TK_CLOSEPA)
					{
						//cout << "右括号" << endl;
						pop_terminal();
						if (temp_terminal.token_code == TK_BEGIN)//确定为函数
						{
							//cout << "函数" << endl;
							//cout << temp_id.spelling << endl;
							//by ljc
							//SymbolTable.FuctionListEle_insert(temp_id, temp_type, function_name, 0);

							//未被__global__修饰的函数入栈
							if (global == 1)
							{
								//cout << "未修饰" << endl;
								//::后直接跟forward或backward的函数入栈
								if (check_colon(temp_id.spelling))
								{
									//cout << "入栈" << endl;
									FuctListQueue.push_back(temp_id);
									cout <<"需插装函数  "<< temp_id.spelling << endl;
								}
									

							}
							pop_terminal();
							global = 1;
							
						}
						//else throw PARSER_ERROR2;
					}
				}
				else if (temp_terminal.token_code == TK_CLOSEPA)
				{
					//cout << "右括号" << endl;
					pop_terminal();
					if (temp_terminal.token_code == TK_BEGIN)
					{
						//cout << "函数" << endl;
						//SymbolTable.FuctionListEle_insert(temp_id, temp_type, function_name, 0);
						if (global == 1)
						{
							if (check_colon(temp_id.spelling))
							{
								//cout << "入栈" << endl;
								FuctListQueue.push_back(temp_id);
								cout << "需插装函数  " << temp_id.spelling << endl;
							}
						}

						pop_terminal();
						global = 1;
						/*block_item_list();
						
						*/
					}
					//else throw PARSER_ERROR2;
				}
				//else throw PARSER_ERROR3;
			}
			
		}
	}
	return 0;
}


int parser::declarator()
{
	//一句
	while (temp_terminal.token_code != TK_SEMICOLON)
	{
		switch (temp_terminal.token_code)
		{
		case TK_ASSIGN:        //赋值
			pop_terminal();
			initializer();
			if (temp_terminal.token_code == TK_SEMICOLON); //;结束					
			else if (temp_terminal.token_code == TK_COMMA) //,再来
			{
				pop_terminal();
				if (temp_terminal.token_code == IDentifier)
				{
					try
					{
						//插入公共总表
						if (SymbolTable.is_fuction_entry)
						{
							if (SymbolTable.FuctionListEle_insert(temp_terminal, temp_type, var_name, temp_type));
							else throw false;
						}
						else
						{
							if (SymbolTable.CommonListElement_insert(temp_terminal, temp_type, var_name, temp_type));
							else throw false;
						}
					}
					catch (bool error)
					{
						cout << endl << "变量重复定义" << endl;
						exit(0);
					}

					pop_terminal();
				}
				else throw PARSER_ERROR5;
			}
			else throw PARSER_ERROR6;
			break;

		case TK_COMMA:
			pop_terminal();
			if (temp_terminal.token_code == IDentifier)
			{
				try
				{
					if (SymbolTable.CommonListElement_insert(temp_terminal, temp_type, var_name, temp_type));
					else throw false;
				}
				catch (bool error)
				{
					cout << endl << "变量重复定义：" << endl;
					exit(0);
				}
				pop_terminal();
			}
			else throw PARSER_ERROR7;
			break;

		default:
			throw PARSER_ERROR8;
			break;
		}
	}
	pop_terminal();
}

int parser::initializer()
{
	if (temp_terminal.token_code == digit_constant || temp_terminal.token_code == character_constant
		|| temp_terminal.token_code == string_literal)
	{
		temp_id = temp_terminal;
		pop_terminal();
	}
	else throw PARSER_ERROR9;
	return 0;
}

int parser::parameter_type_list()
{
	//declaration();
	pop_terminal();//
	//SymbolTable.arg_number++;  //参数个数计数
	//while (temp_terminal.token_code == TK_COMMA)
	while (temp_terminal.token_code != TK_CLOSEPA)
	{
		pop_terminal();
		//declaration();
		//SymbolTable.arg_number++;
	}

	return 0;
}


