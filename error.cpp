#include <iostream>
#include "error.h"

using namespace std;


int ExceptionClass::token_error(int state)
{
	switch (state)
	{
	case IN_ERROR:
		throw string("∑«∑® ‰»Î");
		break;
	case IDENTICAL_ERROR:
		throw string("IDENTICAL_ERROR");
		break;
	}
}

int ExceptionClass::syntax_error(int choose_way)
{
	switch (choose_way)
	{
	case LOOKUP_ERROR0:
		throw string("LOOKUP_ERROR0");
		break;
	case LOOKUP_ERROR1:
		throw string("LOOKUP_ERROR1");
		break;
	case LOOKUP_ERROR2:
		throw string("LOOKUP_ERROR2");
		break;
	case LOOKUP_ERROR3:
		throw string("LOOKUP_ERROR3");
		break;
	case LOOKUP_ERROR4:
		throw string("LOOKUP_ERROR4");
		break;
	case LOOKUP_ERROR5:
		throw string("LOOKUP_ERROR5");
		break;
	case ANALYZE_ERROR:
		throw string("ANALYZE_ERROR");
		break;
	case MATCH_ERROR:
		throw string("MATCH_ERROR");
		break;
	case WRONG_END:
		throw string("WRONG_END");
		break;
	}
	return EXCEPTION;
}
