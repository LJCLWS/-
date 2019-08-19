#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <stack>

using namespace std;


//0为标识符类码
#define IDentifier 0
//1为字符常数类码
#define character_constant 1
//2为字符串常量类码
#define string_literal 2
//3为数字常量
#define digit_constant 3
/* 关键字类码表 */
enum KW_Table //KeyWords
{
	KW_void = 4,
	KW_char,
	KW_short,
	KW_int,
	KW_long,
	KW_float,
	KW_double,
	KW_signed,
	KW_unsigned,
	KW__Bool,
	KW__Complex,
	KW__Imaginary,

	KW_struct,
	KW_union,
	KW_enum,

	KW_typedef,
	KW_extern,
	KW_static,
	KW_auto,
	KW_register,

	KW_volatile,
	KW_const,
	KW_inline,
	KW_return,
	KW_sizeof,

	KW_restrict,

	KW_if,
	KW_else,
	KW_goto,
	KW_for,
	KW_do,
	KW_while,
	KW_continue,
	KW_switch,
	KW_case,
	KW_break,
	KW_default,
};

/* 界符类码表 */
enum PT_Table  //Punctuators
{
	TK_OPENBR = 41,	// [ 左中括号
	TK_CLOSEBR,		// ] 右中括号
	TK_OPENPA,		// ( 左圆括号
	TK_CLOSEPA,		// ) 右圆括号
	TK_BEGIN,		// { 左大括号
	TK_END,			// } 右大括号
	TK_DOT,			// . 结构体成员运算符
	TK_POINTSTO,	// -> 指向结构init体成员运算符

	TK_PLUS2,       // ++
	TK_MINUS2,      // --
	TK_AND,         // & 地址与运算符
	TK_STAR,		// * 乘号
	TK_PLUS,		// + 加号
	TK_MINUS,		// - 减号
	TK_NOT,         // ~
	TK_NOTEQ,       // !

	TK_DIVIDE,		// / 除号
	TK_MOD,			// % 求余运算符
	TK_LSHIFT,      // <<
	TK_RSHIFT,      // >>
	TK_LT,			// < 小于号
	TK_GT,			// > 大于号
	TK_LEQ,			// <= 小于等于号
	TK_GEQ,			// >= 大于等于号
	TK_EQ,			// == 等于号
	TK_NEQ,			// != 不等于号
	TK_SQUARE,      // ^
	TK_OR,          // |
	TK_AND2,        // &&
	TK_OR2,         // ||

	TK_ASK,         // ?
	TK_COLON,       // :
	TK_SEMICOLON,	// ; 分号
	TK_ELLIPSIS,	// ... 省略号

	TK_ASSIGN,		// = 赋值运算符
	TK_STAREQ,      // *=
	TK_DIVIDEEQ,    // /=
	TK_MODEQ,       // %=
	TK_PLUSEQ,      // +=
	TK_MINUSEQ,     // -=
	TK_LSHIFTEQ,    // <<=
	TK_RSHIFTEQ,    // >>=
	TK_ANDEQ,       // &=
	TK_SQUAREEQ,    // ^=
	TK_OREQ,        // |=

	TK_COMMA,		// , 逗号
	TK_HASH,        // #
	TK_HASH2,       // ##

	TK_LMAO,                // <:
	TK_MAOG,                // :>
	TK_LBAI,                // <%
	TK_BAIG,                // %>
	TK_BAIMAO,               // %:
	TK_DBAIMAO              // %:%:
};


enum syntax_express {
	type_specifier = -100,
	storage_class_specifier,
	function_name,
	var_name,
	temp_var_name,
	argv_name,
	const_value,

};

typedef struct TokenElementType
{
	int token_code;   //分类码
	int sytax_code;    //语义码
	string spelling;

}TokenElementType;

//符号总表
typedef struct SymbolListCommon_Type
{
	string name;
	int type;
	int cat;
	int addr;
	int is_active;
}SymbolListCommon_Type;

//符号总表
//typedef struct SymbolListCommon_Type
//{
//	string name;
//	int type;
//	int cat;
//	int addr;
//	int is_active;
//}SymbolListCommon_Type;

//常量表
typedef struct ConstList_Type
{
	string value;
}ConstList_Type;
//活动记录表
typedef struct ActRecordList_Type
{
	string value;
}ActRecordList_Type;

//函数表
typedef struct FuctionList_Type
{
	int level;
	int OFF;
	int argv_number;
	int arg_list_addr;
	int entry;
}FuctionList_Type;
//参数表
typedef struct ArgList_Type
{
	string name;
	int type;
	int cat;
	int addr;
}ArgList_Type;

class symtable
{
	
  public:
	  int is_fuction_entry = 0;
	  int arg_number = 0;

	  symtable() {};
	  ~symtable() {};

	  bool CommonListElement_insert(TokenElementType ident, int type, int cat, int addr);

	  //by ljc
	 // bool FucList(TokenElementType ident, int type, int cat, int addr);//函数列表

	  bool FuctionListEle_insert(TokenElementType ident, int type, int cat, int addr);
	  bool FuctionList_insert();
	  bool FunctionATList_insert(int level, int OFF, int argv_number, int arg_list_addr, int entry);
	  bool ActRecordList_insert(TokenElementType ident);
	  bool ConstElement_insert(TokenElementType ident);
	  bool ArgList_insert(TokenElementType ident, int type, int cat, int addr);
	 
	  void SymbolList_print(void);
	  void FuctionListAT_print(void);
	  void FuctionList_print(void);
	  void ConstList_print(void);
	  void ActRecordList_print(void);

	  vector<vector<SymbolListCommon_Type>> get_FunctionList() { return FuctionList;}
	  vector<SymbolListCommon_Type> get_SymbolList() { return oredered_SymbolList; }
  private:
	vector<ConstList_Type> ConstList;       //常量表

	unordered_map<string, SymbolListCommon_Type>FuctionListEle;   //存储函数表的临时元素（无序）
	vector<SymbolListCommon_Type> oredered_FuctionListEle;    //存储函数表的临时元素（有序）
	vector<vector<SymbolListCommon_Type>>FuctionList;     //函数表
	vector<FuctionList_Type>FuctionATList;      //函数属性表

	unordered_map<string, SymbolListCommon_Type> SymbolList;  //符号总表(无序）
	vector<SymbolListCommon_Type> oredered_SymbolList;         //符号总表（有序）

	stack<ActRecordList_Type> ActRecordList;   //活动表
	vector<ArgList_Type> ArgList;
};

