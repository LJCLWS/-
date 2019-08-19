#include "lexer.h"
#include <iostream>
#include <windows.h>
#include <iomanip>
#include "parser_.h"


using namespace std;


//词法分析器
int lexer::  token_scanner(void)
{
	state = 1;
	int i = -1;
	while (!feof(fin))
	//cout << str.length() << endl;
	//cout << strlen(str.c_str()) << endl;
	//while ((++i) < str.length())
	{
		//cout << "读入一个字符" << endl;
		getch();
		//ch = str[i];

		//cout << "ch=" << ch <<endl;

		state_before = state;

		//cout << "  state_before=" << state ;  //测试state

		try {
			state = state_change(ch_to_num(ch));
			
			//cout << "    state=" << state  << endl; //测试state_change
		}

		catch (string a) {
			cout << a.c_str() << endl;
			exit(0);
		}
		/*if (ch == '?')
		{
			cout << "此处" << "  state_before=" << state_before<<"ch_to_num(ch)= "<< ch_to_num(ch) << "    state=" << state  <<endl;
		}*/

		if (state) {
			if (state > 1)
				token += ch;// 记录字符串;


#ifdef LEXER_OUTPRINT
			//else cout << ch;//输出空格与换行


#endif
		}
		else
		{
			TempToken.token_code = end_state_to_code(state_before, token);
			//类型关键字
			if (TempToken.token_code >= KW_void && TempToken.token_code <= KW__Imaginary)TempToken.sytax_code = type_specifier;
			//修饰关键字
			else if (TempToken.token_code >= KW_typedef && TempToken.token_code <= KW_register)TempToken.sytax_code = storage_class_specifier;

			else TempToken.sytax_code = 0;
			TempToken.spelling = token;
			if (TempToken.token_code >= 0)TokenListQueue.push_back(TempToken);

			//color_token();
			
			state = 1;

			//if(ch!='?')
			ungetc(ch, fin);
			token = "";
		}
	}
	fclose(fin);		// 关闭文件
	TempToken.token_code = EOF;
	TempToken.spelling = "EOF";
	TokenListQueue.push_back(TempToken);
	return 0;
}


int lexer::state_change(int sytax_code)
{
	switch (state)
	{
	case 1: //标识符
		if (sytax_code == 0)state = 1;     //空格
		else if (sytax_code == 1)state = 2;     //字母
		else if (sytax_code == 2)state = 3;     //数字

		else if (ch == '[')state = 24;    //[
		else if (ch == ']')state = 25;    //]
		else if (ch == '(')state = 18;    //(
		else if (ch == ')')state = 19;    //)
		else if (ch == '{')state = 20;    //{
		else if (ch == '}')state = 21;    //}
		else if (ch == '.')state = 40;     //.
		else if (ch == '&')state = 58;        //&

		else if (ch == '\'')state = 9;     //'
		else if (ch == '\"')state = 11;     //"
		else if (ch == '>')state = 2;     //>
		else if (ch == '<')state = 2;	    //<
		else if (ch == '=')state = 6;     //=

		else if (ch == '+')state = 26;    //+
		else if (ch == '-')state = 27;    //-
		else if (ch == '*')state = 28;    //*
		else if (ch == '/')state = 29;    // /
		else if (ch == '%')state = 50;            //%
		else if (ch == '~')state = 67;             //~
		else if (ch == '!')state = 41;             //!
		else if (ch == '^')state = 61;              //^
		else if (ch == '|')state = 62;               //|
		else if (ch == '#')state = 30;           // #
		else if (ch == '?')state = 64;            //?
		else if (ch == ':')state = 2;             //:
		else if (ch == ',')state = 22;    //,
		else if (ch == ';')state = 23;    //;

		else if (sytax_code == -1)state = -1;    //非法输入

		else state = 0;

		break;
	case 2: //标识符
		if (sytax_code == 1)state = 2;        //字母
		else if (sytax_code == 2)state = 2;        //数字
		else
			state = 0;
		break;
	case 3: //数字
		if (sytax_code == 2)state = 3;         //数字
		else if (sytax_code == 1)state = 2;     //-2修正
		else if (ch == '.')state = 8;         //.
		else
			state = 0;
		break;
	case 4:  //>
		if (sytax_code == 1)state = 2;        //字母
		else if (sytax_code == 2)state = 2;

		else if (ch == '=')state = 13;
		else if (ch == '>')state = 46;
		else state = 0;
		break;
	case 5:  //<
		if (sytax_code == 1)state = 2;        //字母
		else if (sytax_code == 2)state = 2;

		else if (ch == '=')state = 14;
		else if (ch == ':')state = 56;
		else if (ch == '<')state = 48;
		else if (ch == '%')state = 57;

		else state = 0;
		break;
	case 6:  //=
		if (ch == '=')state = 15;
		else state = 0;
		break;
	case 7:
		if (sytax_code == 2)state = 7;        //数字
		else state = 0;
		break;
	case 8://.
		if (sytax_code == 2)state = 7;        //数字
		else;//error
		break;
	case 9://'
		if (sytax_code == 1)state = 10;        //字母
		else;//error;
		break;
	case 10:
		if (ch == '\'')state = 16;       //'
		else;//error;
		break;
	case 11://"
		state = 12;
		break;
	case 12:
		if (ch == '\"')state = 17;      //"
		else state = 12;   //字母

		break;

	case 16:
		if (ch == '\'')state = 0;       //'
		else state = 0;
		break;
	case 17://两个“
		if (ch == '\"')state = 17;      //"
		else state = 0;

		break;
	case 13:
	case 14:
	case 15:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
		state = 0;
		break;

	case 26:  //+
		if (ch == '=')state = 31;     //=
		else if (ch == '+')state = 43;
		else state = 0;
		break;
	case 27: //-
		if (ch == '=')state = 32;     //=
		else if (ch == '-')state = 44;
		else if (ch == '>')state = 45;
		else state = 0;
		break;
	case 28: // *
		if (ch == '=')state = 33;     //=
		else state = 0;
		break;
	case 29: // /
		if (ch == '=')state = 34;     //=
		else if (ch == '/')state = 35;  // /
		else if (ch == '*')state = 36;  // *

		else state = 0;
		break;
	case 30:// #
		if (ch == '#')state = 39;    // #
		else state = 0;
		break;

	case 35:// /
		if (ch == '\n')state = 0;
		else state = 35;
		break;

	case 36:// *
		if (ch == '*')state = 37;
		else state = 36;
		break;

	case 37:// *
		if (ch == '/')state = 38;
		else state = 36;
		break;
	case 38:
		state = 0;
		break;
	case 40:
		if (sytax_code == 2)state = 7;
		else if (ch == '.')state = 68;

		else state = 0;
		break;
	case 41:
		if (ch == '=')state = 42;
		else state = 0;
		break;
	case 42:
		state = 0;
		break;
	case 43:
		state = 0;
		break;
	case 44:
		state = 0;
		break;
	case 45:
		state = 0;
		break;
	case 46:
		if (ch == '=')state = 47;
		else state = 0;
		break;
	case 48:
		if (ch == '=')state = 49;
		else state = 0;
		break;
	case 50:
		if (ch == '>')state = 51;
		else if (ch == '=')state = 52;
		else if (ch == ':')state = 53;
		break;
	case 51:
		state = 0;
		break;
	case 52:
		state = 0;
		break;
	case 53:
		if (ch == '%')state = 54;
		else state = 0;
		break;
	case 54:
		if (ch == ':')state = 55;
		else;//error
		break;
	case 55:
		state = 0;
		break;
	case 56:
		state = 0;
		break;
	case 57:
		state = 0;
		break;
	case 58:
		if (ch == '=')state = 59;
		else if (ch == '&')state = 60;
		else state = 0;
		break;
	case 59:
	case 60:
		state = 0;
		break;
	case 61:
		if (ch == '=')state = 69;
		else	 state = 0;
		break;
	case 62:
		if (ch == '=')state = 70;
		else if (ch == '|')state = 63;
		else state = 0;
		break;
	case 63:
		state = 0;
		break;
	case 64:
		state = 0;
		break;
	case 65:
		if (sytax_code == 1)state = 2;        //字母
		else if (sytax_code == 2)state = 2;

		else if (ch == '>')state = 66;
		else state = 0;
		break;
	case 68:
		if (ch == '.')state = 71;
		else;//error
		break;
	case 69:
		state = 0;
		break;
	case 71:
		state = 0;
		break;
	default:
		state = 0;//error
		break;

	}
	if (state < 0)error.token_error(state);

	return state;
}

int lexer::search(string token)
{
	for (int i = KW_void; i <= KW_default; i++)
		if (token == TokenTable[i].spelling) return TokenTable[i].token_code;

	return 0;//标识符
}

int lexer::end_state_to_code(int state_before, string token)
{
	switch (state_before)
	{
	case 2:
		code = search(token);
		break;
	case 3:
		code = 3;//数字类码
		break;
	case 4:
		code = TK_GT;
		break;
	case 5:
		code = TK_LT;
		break;
	case 6:
		code = TK_ASSIGN;
		break;
	case 7:
		code = 3;//数字类码
		break;
	case 13:
		code = TK_GEQ;
		break;

	case 14:
		code = TK_LEQ;
		break;

	case 15:
		code = TK_EQ;
		break;

	case 16:
		code = 1;
		break;
	case 17:
		code = 2;
		break;

	case 18:
		code = TK_OPENPA;
		break;
	case 19:
		code = TK_CLOSEPA;
		break;
	case 20:
		code = TK_BEGIN;
		break;
	case 21:
		code = TK_END;
		break;
	case 22:
		code = TK_COMMA;
		break;
	case 23:
		code = TK_SEMICOLON;
		break;
	case 24:
		code = TK_OPENBR;
		break;
	case 25:
		code = TK_CLOSEBR;
		break;
	case 26:
		code = TK_PLUS;
		break;
	case 27:
		code = TK_MINUS;
		break;
	case 28:
		code = TK_STAR;
		break;
	case 29:
		code = TK_DIVIDE;
		break;
	case 30:
		code = TK_HASH;
		break;
	case 31:
		code = TK_PLUSEQ;
		break;
	case 32:
		code = TK_MINUSEQ;
		break;
	case 33:
		code = TK_STAREQ;
		break;
	case 34:
		code = TK_DIVIDEEQ;
		break;

	case 39:
		code = TK_HASH2;
		break;
	case 40:
		code = TK_DOT;
		break;
	case 42:
		code = TK_NEQ;
		break;
	case 43:
		code = TK_PLUS2;
		break;
	case 44:
		code = TK_MINUS2;
		break;
	case 45:
		code = TK_POINTSTO;
		break;

	case 47:
		code = TK_RSHIFTEQ;
		break;
	case 49:
		code = TK_LSHIFTEQ;
		break;
	case 51:
		code = TK_BAIG;
		break;
	case 52:
		code = TK_MODEQ;
		break;
	case 55:
		code = TK_DBAIMAO;
		break;

	case 56:
		code = TK_LMAO;
		break;
	case 57:
		code = TK_LBAI;
		break;
	case 59:
		code = TK_ANDEQ;
		break;
	case 60:
		code = TK_AND2;
		break;
	case 61:
		code = TK_SQUARE;
		break;
	case 63:
		code = TK_OR2;
		break;
	case 64:
		code = TK_ASK;
		break;
	case 65:
		code = TK_COLON;
		break;
	case 66:
		code = TK_MAOG;
		break;
	case 67:
		code = TK_NOT;
		break;
	case 69:
		code = TK_SQUAREEQ;
		break;
	case 70:
		code = TK_OREQ;
		break;
	case 71:
		code = TK_ELLIPSIS;
		break;

	default:
		code = -1;//error;
		break;

	}

	return code;
}

int lexer::ch_to_num(char ch)
{
	if (is_nondigit(ch))    return 1; //字母或下划线
	else if (is_digit(ch))       return 2; //数字
	else {
		switch (ch)
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			return 0; //空格

		default:
			if (ch != EOF);//没有返回值
			else return 0;
			break;
		}
	}
}

//token序列输出
void lexer::Lexer_output(void)
{
	cout << endl << "token list:" << endl;
	for (int i = 0; i < TokenListQueue.size(); i++)
	{
		static int j = 1;
		cout << setw(4) << "<" << TokenListQueue[i].token_code << "," << TokenListQueue[i].sytax_code << "," << TokenListQueue[i].spelling.c_str() << ">" << "	" << right;
		if (j++ % 5 == 0)cout << endl;
	}

	/*while (TokenListQueue.size())
	{
		static int i = 1;
		TokenElementType t = TokenListQueue.front();
		TokenListQueue.pop();
		cout <<setw(4) << "<"<< t.token_code<< "," << t.sytax_code << "," << t.spelling.c_str() << ">"<<"	" << right;
		if (i++ % 5 == 0)cout << endl;
	} */
	cout << endl;
}


//词法着色
void lexer::color_token(void)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (TempToken.token_code == 0)     //标识符
		SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	else if (TempToken.token_code == 1)  //字符常量
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (TempToken.token_code == 2)  //字符串常量
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (TempToken.token_code == 3)  //数字常量
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (TempToken.token_code <= KW__Imaginary && TempToken.token_code >= KW_auto)//关键字
		SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	else if (TempToken.token_code <= TK_HASH2 && TempToken.token_code >= TK_OPENBR)//界符
		SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	else
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);

#ifdef LEXER_OUTPRINT
	cout << TempToken.spelling.c_str();
#endif // LEXER_OUTPRINT


}
