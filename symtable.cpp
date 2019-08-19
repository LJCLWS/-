#include <iostream>
#include "symtable.h"


using namespace std;

bool symtable::CommonListElement_insert(TokenElementType ident,int type,int cat,int addr)
{
	static int address = 0;
	SymbolListCommon_Type SymbolListEle;

	SymbolListEle.name = ident.spelling;
	SymbolListEle.type = type;
	SymbolListEle.cat = cat;

	if (type == KW_char)address += 1;
	else if (type == KW_short)address += 2;
	else if (type == KW_int)address += 4;
	else if (type == KW_long)address += 8;
	else if (type == KW_float)address += 4;
	else if (type == KW_double)address += 4;
	else address = addr;
	SymbolListEle.addr = address;

	unordered_map<string, SymbolListCommon_Type>::iterator  iter;
	pair<unordered_map<string, SymbolListCommon_Type>::iterator, bool > ret;
	ret = SymbolList.insert(pair<string, SymbolListCommon_Type>(SymbolListEle.name, SymbolListEle));
	if (ret.second) oredered_SymbolList.push_back(SymbolListEle);
	return ret.second;
}

bool symtable::FuctionListEle_insert(TokenElementType ident, int type, int cat, int addr)
{
	SymbolListCommon_Type FuctionListEleEle;     //函数表临时元素的临时单元

	FuctionListEleEle.name = ident.spelling;
	FuctionListEleEle.type = type;
	FuctionListEleEle.cat = cat;
	FuctionListEleEle.addr = addr;

	unordered_map<string, SymbolListCommon_Type>::iterator  iter;
	pair<unordered_map<string, SymbolListCommon_Type>::iterator, bool > ret;
	ret = FuctionListEle.insert(pair<string, SymbolListCommon_Type>(FuctionListEleEle.name, FuctionListEleEle));
	if (ret.second) 
	oredered_FuctionListEle.push_back(FuctionListEleEle);

	return ret.second;

}

bool symtable::FuctionList_insert()
{
	FuctionList.push_back(oredered_FuctionListEle);
	oredered_FuctionListEle.clear();
	FuctionListEle.clear();
	return true;
}

bool symtable::ConstElement_insert(TokenElementType ident)
{
	//ConstList.begin();
	ConstList_Type TempConstListElement;
	int type;
	TempConstListElement.value = ident.spelling;
	
	//if (ident.token_code == character_constant)type = character_constant;
	//else if (ident.token_code == digit_constant)type = digit_constant;
	//else if (ident.token_code == string_literal)type = string_literal;
	//else type = type_in;
	//SymbolList[ident.spelling].type = type;

	ConstList.push_back(TempConstListElement);
	return true;
}

bool symtable::FunctionATList_insert(int level, int OFF, int arg_number, int arg_list_addr,int entry)
{
	FuctionList_Type temp;
	temp.level = level;
	temp.OFF = OFF;
	temp.argv_number = arg_number;
	temp.arg_list_addr = arg_list_addr;
	temp.entry = entry;
	FuctionATList.push_back(temp);
	return true;
}


bool symtable::ActRecordList_insert(TokenElementType ident)
{
	ActRecordList_Type ActRecordListEle;
	ActRecordListEle.value  = ident.spelling;
	ActRecordList.push(ActRecordListEle);
	return true;
}

bool symtable::ArgList_insert(TokenElementType ident, int type, int cat, int addr)
{
	ArgList_Type ArgListEle;
	ArgListEle.name = ident.spelling;
	ArgListEle.type = type;
	ArgListEle.cat = cat;
	ArgListEle.addr = addr;
	ArgList.push_back(ArgListEle);
	return true;
}
//输出符号表
void symtable::SymbolList_print(void)
{
	cout << endl << "总符号表如下：" << endl;
	for (int i = 0; i < oredered_SymbolList.size(); i++)
	cout << oredered_SymbolList[i].name << "," << oredered_SymbolList[i].type << "," << oredered_SymbolList[i].cat << /*"," << oredered_SymbolList[i].addr <<*/ endl;
}

void symtable::FuctionList_print(void)   //需改
{
	cout << endl << "函数表：" << endl;
		for (int i = 0; i < FuctionList.size(); i++)
		{
			vector<SymbolListCommon_Type> temp;
			temp = FuctionList[i];
				for (int j = 0; j < temp.size(); j++)
				{
					SymbolListCommon_Type t = temp[j];
					cout << t.name << "," << t.type << "," << t.cat /*<< "," << t.addr*/ << endl;
				}
			cout << endl;
		}    
}
void symtable::FuctionListAT_print(void)
{
	cout << endl << "函数属性表：" << endl;
	for (int i = 0; i <  FuctionATList.size(); i++)
		cout << FuctionATList[i].level<<","<< FuctionATList[i].OFF <<","<< FuctionATList[i].argv_number /*<<","
		     << FuctionATList[i].arg_list_addr */<<","<< FuctionATList[i].entry << endl;
}
void symtable::ConstList_print(void)
{
	cout << endl << "常量表：" << endl;
	for (int i = 0; i < ConstList.size();i++)
		cout << ConstList[i].value.c_str() << endl;
}
void symtable::ActRecordList_print(void)
{
	cout << endl << "活动记录表：" << endl;
	ActRecordList_Type Temp;
	while (ActRecordList.size())
	{
		Temp = ActRecordList.top();
		ActRecordList.pop();
		cout << Temp.value << endl;
	}
}

