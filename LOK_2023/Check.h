#pragma once
#include "FST.h"
using namespace fst;

#define CHECK_IS (char*)"",\
				 3,\
				 NODE(1, RELATION('i', 1)),\
				 NODE(1, RELATION('s', 2)),\
				 NODE()

#define CHECK_NUM (char*)"",\
				   4,\
				   NODE(1, RELATION('n', 1)),\
				   NODE(1, RELATION('u', 2)),\
				   NODE(1, RELATION('m', 3)),\
				   NODE()

#define CHECK_BREAK (char*)"",\
					6,\
					NODE(1, RELATION('b', 1)),\
					NODE(1, RELATION('r', 2)),\
					NODE(1, RELATION('e', 3)),\
					NODE(1, RELATION('a', 4)),\
					NODE(1, RELATION('k', 5)),\
					NODE()

#define CHECK_FOR (char*)"",\
				   4,\
				   NODE(1, RELATION('F', 1)),\
				   NODE(1, RELATION('o', 2)),\
				   NODE(1, RELATION('r', 3)),\
				   NODE()

#define CHECK_SYMB (char*)"",\
					5,\
					NODE(1, RELATION('s', 1)),\
					NODE(1, RELATION('y', 2)),\
					NODE(1, RELATION('m', 3)),\
					NODE(1, RELATION('b', 4)),\
					NODE()

#define CHECK_FLOAT (char*)"",\
					6,\
					NODE(1, RELATION('f', 1)),\
					NODE(1, RELATION('l', 2)),\
					NODE(1, RELATION('o', 3)),\
					NODE(1, RELATION('a', 4)),\
					NODE(1, RELATION('t', 5)),\
					NODE()

#define CHECK_ACTION (char*)"",\
					 7,\
					 NODE(1, RELATION('a', 1)),\
					 NODE(1, RELATION('c', 2)),\
					 NODE(1, RELATION('t', 3)),\
					 NODE(1, RELATION('i', 4)),\
					 NODE(1, RELATION('o', 5)),\
					 NODE(1, RELATION('n', 6)),\
					 NODE()

#define CHECK_EQUALINIT (char*)"",\
						2,\
						NODE(1, RELATION('=', 1)),\
						NODE()

#define CHECK_CONSOLE (char*)"",\
					  8,\
					  NODE(1, RELATION('c', 1)),\
					  NODE(1, RELATION('o', 2)),\
					  NODE(1, RELATION('n', 3)),\
					  NODE(1, RELATION('s', 4)),\
					  NODE(1, RELATION('o', 5)),\
					  NODE(1, RELATION('l', 6)),\
					  NODE(1, RELATION('e', 7)),\
					  NODE()

#define CHECK_FOO (char*)"",\
				  4,\
				  NODE(1, RELATION('f', 1)),\
				  NODE(1, RELATION('o', 2)),\
				  NODE(1, RELATION('o', 3)),\
				  NODE()

#define CHECK_LEFTSCOPE (char*)"",\
						2,\
						NODE(1, RELATION('(', 1)),\
						NODE()

#define CHECK_RIGHTSCOPE (char*)"",\
						 2,\
						 NODE(1, RELATION(')', 1)),\
						 NODE()

#define CHECK_LEFTBRACKET (char*)"",\
						   2,\
						   NODE(1, RELATION('{', 1)),\
						   NODE()

#define CHECK_RIGHTBRACKET (char*)"",\
							2,\
							NODE(1, RELATION('}', 1)),\
							NODE()

#define CHECK_COMMA (char*)"",\
					2,\
					NODE(1, RELATION(',', 1)),\
					NODE()

#define CHECK_EQUALSIGNS (char*)"",\
						  3,\
						  NODE(4, RELATION('>', 2), RELATION('<', 2), RELATION('=', 1), RELATION('!', 1)),\
						  NODE(1, RELATION('=', 2)),\
						  NODE()

#define CHECK_OPERATION (char*)"",\
						2,\
						NODE(2, RELATION('&', 1), RELATION('|', 1)),\
						NODE()

#define CHECK_MAIN (char*)"",\
					5,\
					NODE(1, RELATION('m', 1)),\
					NODE(1, RELATION('a', 2)),\
					NODE(1, RELATION('i', 3)),\
					NODE(1, RELATION('n', 4)),\
					NODE()

#define CHECK_NOT (char*)"",\
				  2,\
				  NODE(1, RELATION('~', 1)),\
				  NODE()

#define CHECK_MINUS (char*)"",\
					2,\
					NODE(1, RELATION('-', 1)),\
					NODE()

#define CHECK_RETURN (char*)"",\
					 7,\
					 NODE(1, RELATION('r', 1)),\
					 NODE(1, RELATION('e', 2)),\
					 NODE(1, RELATION('t', 3)),\
					 NODE(1, RELATION('u', 4)),\
					 NODE(1, RELATION('r', 5)),\
					 NODE(1, RELATION('n', 6)),\
					 NODE()

#define CHECK_IF (char*)"",\
				 2,\
				 NODE(1, RELATION('?', 1)),\
			     NODE()

#define CHECK_TRUTH (char*)"",\
					6,\
					NODE(1, RELATION('T', 1)),\
					NODE(1, RELATION('r', 2)),\
					NODE(1, RELATION('u', 3)),\
					NODE(1, RELATION('t', 4)),\
					NODE(1, RELATION('h', 5)),\
					NODE()

#define CHECK_LIE (char*)"",\
				  4,\
				  NODE(1, RELATION('L', 1)),\
				  NODE(1, RELATION('i', 2)),\
				  NODE(1, RELATION('e', 3)),\
				  NODE()

#define CHECK_SKIP (char*)"",\
				   5,\
				   NODE(1, RELATION('s', 1)),\
				   NODE(1, RELATION('k', 2)),\
				   NODE(1, RELATION('i', 3)),\
				   NODE(1, RELATION('p', 4)),\
				   NODE()

#define CHECK_SEMMICOMMA (char*)"",\
						 2,\
						 NODE(1, RELATION(';', 1)),\
						 NODE()

namespace CHECK
{
	struct Checker
	{
		FST* _is;
		FST* _num;
		FST* _break;
		FST* _skip;
		FST* _for;
		FST* _symb;
		FST* _float;
		FST* _action;
		FST* _equalinit;
		FST* _console;
		FST* _foo;
		FST* _leftscope;
		FST* _rightscope;
		FST* _leftbracket;
		FST* _rightbracket;
		FST* _comma;
		FST* _allequalsigns;
		FST* _boperations;
		FST* _semicomma;
		FST* _not;
		FST* _minus;
		FST* _main;
		FST* _return;
		FST* _if;
		FST* _truth;
		FST* _lie;

		Checker();

		~Checker();
	};

	bool check(FST* fst, const char* str);
}

