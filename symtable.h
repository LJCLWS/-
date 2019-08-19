#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <stack>

using namespace std;


//0Ϊ��ʶ������
#define IDentifier 0
//1Ϊ�ַ���������
#define character_constant 1
//2Ϊ�ַ�����������
#define string_literal 2
//3Ϊ���ֳ���
#define digit_constant 3
/* �ؼ�������� */
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

/* �������� */
enum PT_Table  //Punctuators
{
	TK_OPENBR = 41,	// [ ��������
	TK_CLOSEBR,		// ] ��������
	TK_OPENPA,		// ( ��Բ����
	TK_CLOSEPA,		// ) ��Բ����
	TK_BEGIN,		// { �������
	TK_END,			// } �Ҵ�����
	TK_DOT,			// . �ṹ���Ա�����
	TK_POINTSTO,	// -> ָ��ṹinit���Ա�����

	TK_PLUS2,       // ++
	TK_MINUS2,      // --
	TK_AND,         // & ��ַ�������
	TK_STAR,		// * �˺�
	TK_PLUS,		// + �Ӻ�
	TK_MINUS,		// - ����
	TK_NOT,         // ~
	TK_NOTEQ,       // !

	TK_DIVIDE,		// / ����
	TK_MOD,			// % ���������
	TK_LSHIFT,      // <<
	TK_RSHIFT,      // >>
	TK_LT,			// < С�ں�
	TK_GT,			// > ���ں�
	TK_LEQ,			// <= С�ڵ��ں�
	TK_GEQ,			// >= ���ڵ��ں�
	TK_EQ,			// == ���ں�
	TK_NEQ,			// != �����ں�
	TK_SQUARE,      // ^
	TK_OR,          // |
	TK_AND2,        // &&
	TK_OR2,         // ||

	TK_ASK,         // ?
	TK_COLON,       // :
	TK_SEMICOLON,	// ; �ֺ�
	TK_ELLIPSIS,	// ... ʡ�Ժ�

	TK_ASSIGN,		// = ��ֵ�����
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

	TK_COMMA,		// , ����
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
	int token_code;   //������
	int sytax_code;    //������
	string spelling;

}TokenElementType;

//�����ܱ�
typedef struct SymbolListCommon_Type
{
	string name;
	int type;
	int cat;
	int addr;
	int is_active;
}SymbolListCommon_Type;

//�����ܱ�
//typedef struct SymbolListCommon_Type
//{
//	string name;
//	int type;
//	int cat;
//	int addr;
//	int is_active;
//}SymbolListCommon_Type;

//������
typedef struct ConstList_Type
{
	string value;
}ConstList_Type;
//���¼��
typedef struct ActRecordList_Type
{
	string value;
}ActRecordList_Type;

//������
typedef struct FuctionList_Type
{
	int level;
	int OFF;
	int argv_number;
	int arg_list_addr;
	int entry;
}FuctionList_Type;
//������
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
	 // bool FucList(TokenElementType ident, int type, int cat, int addr);//�����б�

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
	vector<ConstList_Type> ConstList;       //������

	unordered_map<string, SymbolListCommon_Type>FuctionListEle;   //�洢���������ʱԪ�أ�����
	vector<SymbolListCommon_Type> oredered_FuctionListEle;    //�洢���������ʱԪ�أ�����
	vector<vector<SymbolListCommon_Type>>FuctionList;     //������
	vector<FuctionList_Type>FuctionATList;      //�������Ա�

	unordered_map<string, SymbolListCommon_Type> SymbolList;  //�����ܱ�(����
	vector<SymbolListCommon_Type> oredered_SymbolList;         //�����ܱ�����

	stack<ActRecordList_Type> ActRecordList;   //���
	vector<ArgList_Type> ArgList;
};

