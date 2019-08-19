#ifndef _LEXER_H
#define _LEXER_H
#pragma warning(disable:4996)

#define LEXER_OUTPRINT

#include<cstdlib>
#include<iostream>
#include <queue>
#include "error.h"
#include "symtable.h"

using namespace std;

#define is_nondigit(c) ((c >= 'a' && c <= 'z') ||(c >= 'A' && c <= 'Z') ||c == '_'||c==':' ||c=='<'||c=='>')  //�ж�c�Ƿ�Ϊ��ĸ(a-z,A-Z)���»���(-)�ȿ�����ɺ������ķ���
#define is_digit(c)   (c >= '0' && c <= '9')

#define KW_TOKEN_NUM  TK_DBAIMAO+1
//��̬�����ѯ�� �����������ѯ�ؼ���������

/* ���ʴ洢�ṹ���� */
typedef struct TokenTableType
{
    int    token_code;					// ���ʱ���
    string spelling;					// �����ַ���
}TokenTableType;

class lexer {
public:
	vector<TokenElementType> FuctListQueue;
	symtable SymbolTable;
	lexer(char *argv) {
		//Դ�ļ�����
		try { 
			//fin = fopen(argv, "r"); 
			errno_t err = fopen_s(&fin, argv, "rb");
			if(!fin)
			{
				throw string("Can't find source file!\n");
			}
		}
		catch (string e) 
		{
			cout << e.c_str()<<endl;
			exit(0);
		}	
	};
	virtual ~lexer() {};

	int token_scanner(void);//�ʷ������������庯��
	int ch_to_num(char ch);//��ʶ����ַ��������ֱ�ʾ������ֵ��Ӧ�Զ�����״̬ת��������

	int state_change(int sytax_code);//����״̬ת����stateΪ��ǰ״̬��sytax_codeΪת������

	int search(string token);//��ؼ��ֱ�ʶ���Ǳ�ʶ�����ǹؼ���
	int end_state_to_code(int state_before, string token);//���Զ����ս�״ֵ̬�ó���Ӧ�ĵ�������

	void Lexer_output(void);//�ʷ��������Ľ�����
	void color_token(void);//�Թؼ��ָ�����ɫ��ʾ


	//��Դ�ļ��ж�ȡһ���ַ�
	inline void  getch() {
		//ch = getc(fin);
		fscanf(fin, "%c", &ch);
	}  

	void setstr(string str)
	{
		this->str = str;
	}

	string getstr()
	{
		return str;
	}

protected:
	vector<TokenElementType> TokenListQueue;

	//by ljc
//	vector<TokenElementType> FuctListQueue;
	TokenTableType TokenTable[KW_TOKEN_NUM] =
	{
		{ IDentifier,"0" },  //��ʶ��
		{ character_constant,"0" },  //�ַ�����
		{ string_literal,"0" },  //�ַ�������
		{ digit_constant,"0" },  //���ֳ���
		{ KW_void,"void" },
		{ KW_char,"char" },
		{ KW_short,"short" },
		{ KW_int,"int"},
		{ KW_long,"long" },
		{ KW_float,"float" },
		{ KW_double,"double" },
		{ KW_signed,"signed" },
		{ KW_unsigned,"usigned" },
		{ KW__Bool,"_Bool" },
		{ KW__Complex,"_Complex" },
		{ KW__Imaginary,"_Imaginary" },



		{ KW_struct,"struct" },
		{ KW_union,"union" },
		{ KW_enum,"enum" },
		{ KW_typedef,"typedef" },
		{ KW_extern,"extern" },
		{ KW_static,"static" },
		{ KW_auto,"auto" },
		{ KW_register,"register" },

		{ KW_volatile,"volatile" },
		{ KW_const,"const" },
		{ KW_inline,"inline" },
		{ KW_return,"return" },
		{ KW_sizeof,"sizeof" },

		{ KW_restrict,"restrict" },
		{ KW_if,"if" },
		{ KW_else,"else" },
		{ KW_goto,"goto" },
		{ KW_for,"for" },
		{ KW_do,"do" },
		{ KW_while,"while" },
		{ KW_continue,"continue" },
		{ KW_switch,"switch" },
		{ KW_case,"case" },
		{ KW_break,"break" },
		{ KW_default,"default"},

		{ TK_OPENBR,"[" },		// [ ��������
		{ TK_CLOSEBR,"]" },		// ] ��Բ����
		{ TK_OPENPA,"(" },		// ( ��Բ����
		{ TK_CLOSEPA,")" },		// ) ��Բ����
		{ TK_BEGIN,"{" },		// { �������
		{ TK_END,"}" },			// } �Ҵ�����
		{ TK_DOT,"." },			// . �ṹ���Ա�����
		{ TK_POINTSTO,"->" },	// -> ָ��ṹinit���Ա�����

		{ TK_PLUS2,"++" },       // ++
		{ TK_MINUS2,"--" },      // --
		{ TK_AND,"&" },         // & ��ַ�������
		{ TK_STAR,"*" },		// * �˺�
		{ TK_PLUS,"+" },		// + �Ӻ�
		{ TK_MINUS,"-" },		// - ����
		{ TK_NOT,"~" },         // ~
		{ TK_NOTEQ,"!" },       // !

		{ TK_DIVIDE,"/" },		// / ����
		{ TK_MOD,"%" },			// % ���������
		{ TK_LSHIFT,"<<" },      // <<
		{ TK_RSHIFT,">>" },      // >>
		{ TK_LT,"<" },			// < С�ں�
		{ TK_GT,">" },			// > ���ں�
		{ TK_LEQ,"<=" },			// <= С�ڵ��ں�
		{ TK_GEQ,">=" },			// >= ���ڵ��ں�
		{ TK_EQ,"==" },			// == ���ں�
		{ TK_NEQ,"!=" },			// != �����ں�
		{ TK_SQUARE,"^" },      // ^
		{ TK_OR,"|" },          // |
		{ TK_AND2,"&&" },        // &&
		{ TK_OR2,"||" },         // ||

		{ TK_ASK,"?" },         // ?
		{ TK_COLON,":" },       // :
		{ TK_SEMICOLON,";" },	// ; �ֺ�
		{ TK_ELLIPSIS,"..." },	// ... ʡ�Ժ�

		{ TK_ASSIGN,"=" },		// = ��ֵ�����
		{ TK_STAREQ,"*=" },      // *=
		{ TK_DIVIDEEQ,"/=" },    // /=
		{ TK_MODEQ,"%=" },       // %=
		{ TK_PLUSEQ,"+=" },      // +=
		{ TK_MINUSEQ,"-+" },     // -=
		{ TK_LSHIFTEQ,"<<=" },    // <<=
		{ TK_RSHIFTEQ,">>=" },    // >>=
		{ TK_ANDEQ,"&=" },       // &=
		{ TK_SQUAREEQ,"^=" },    // ^=
		{ TK_OREQ,"|=" },        // |=

		{ TK_COMMA,"," },		// , ����
		{ TK_HASH,"#" },        // #
		{ TK_HASH2,"##" },       // ##
		{ TK_LMAO,"<:"},          // <:
		{ TK_MAOG,":>"},           // :>
		{ TK_LBAI,"<%"},          // <%
		{ TK_BAIG,"%>"},          // %>
		{ TK_BAIMAO,"%:"},        // %:
		{ TK_DBAIMAO,"%:%:"}    // %:%:
	};

private:
	TokenElementType TempToken;
	char ch;
	int state=0;  //0
	int state_before=0;  //0
	int code = 0;
	string token="";
	FILE *fin;
	string str = "";

	ExceptionClass error;

};


#endif // _LEXER_H
