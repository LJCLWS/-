#pragma once

#define EXCEPTION        -1
          

#define IN_ERROR          -1
#define IDENTICAL_ERROR   -2

#define ANALYZE_ERROR    0

#define LOOKUP_ERROR0   -1
#define LOOKUP_ERROR1   -2
#define LOOKUP_ERROR2   -3
#define LOOKUP_ERROR3   -4
#define LOOKUP_ERROR4   -5
#define LOOKUP_ERROR5   -6

#define MATCH_ERROR     -7

#define WRONG_END       -8

#define START_ERROR     -9
#define PARSER_ERROR1   -10
#define PARSER_ERROR2   -11
#define PARSER_ERROR3   -12
#define PARSER_ERROR4   -13
#define PARSER_ERROR5   -14
#define PARSER_ERROR6   -15
#define PARSER_ERROR7   -16
#define PARSER_ERROR8   -17

#define TK_END_MATCH_ERROR -18

#define PARSER_ERROR9   -19
#define PARSER_ERROR10   -20
#define PARSER_ERROR11   -21
#define PARSER_ERROR12   -22
#define PARSER_ERROR13   -23
#define PARSER_ERROR14   -24
#define PARSER_ERROR15   -25
#define PARSER_ERROR16   -26
#define PARSER_ERROR17   -27
#define PARSER_ERROR18   -28
#define PARSER_ERROR19   -29
#define PARSER_ERROR20   -30
#define PARAMETER_ERROR21  -31


class ExceptionClass
{
public:
	ExceptionClass() {};
	~ExceptionClass() {};
	int token_error(int state);
	int syntax_error(int error_type);

private:


};

