#pragma once
#include "stdafx.h"

namespace PN
{
	char* PolishNotation(char* expression);

	bool Polish(LT::LexTable& lextable, IT::IdTable& idtable);

	void goThrowStr(char* expr, char* polExprs);

	bool isOperand(char symb);

	bool isOperation(char symb);

	bool isBrackets(char symb);

	void writeTo(char* expr, stack<char> _stack);

	bool checkExpression(char* expression);

	bool checkOperations(char* expression);

	bool checkBrackets(char* expression);
}

