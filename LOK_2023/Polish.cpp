#include "Polish.h"

namespace PN
{
	typedef char operation;
	void go_to_pol(char* expr, map<char, IT::Entry*> dict, LT::LexTable& lextable, IT::IdTable& idtable);
	bool Polish(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		auto expr = new char[200];
		auto cls = (char*)"";
		int size;
		int index;
		char* polExprs;
		char el;
		map<char, IT::Entry*> dict;
		IT::Entry* ide;
		LT::Entry* lte;
		auto table = lextable.table;

		bool isFunc;
		int cntScopes;

		for (int i = 0; i < lextable.size; i++)
		{
			if (table[i].lexema == '=' || table[i].lexema == 'r')
			{
				i++;
				strcpy_s(expr, cls);
				dict.clear();
				size = 0;
				isFunc = false;
				cntScopes = 0;
				el = 'a';

				while (table[i].lexema != ';')
				{
					if (table[i].lexema == '(' && table[i - 1].lexema == 'i')
					{
						isFunc = true;
						cntScopes = 0;
					}
					if (!isFunc)
					{
						if (table[i].lexema != 'i' && table[i].lexema != 'l')
						{
							expr[size++] = (table[i].lexema == 'v' ? table[i].view : table[i].lexema);
						}
						else
						{
							expr[size++] = el++;
							dict[el - 1] = &idtable.table[table[i].idxTI];
						}
					}

					if (table[i].lexema == '(' && isFunc)
					{
						cntScopes++;
					}
					else if (table[i].lexema == ')' && isFunc)
					{
						cntScopes--;

						if (cntScopes == 0)
						{
							isFunc = false;
						}
					}
					i++;
				}
				expr[size] = 0;

				polExprs = PolishNotation(expr);

				go_to_pol(polExprs, dict, lextable, idtable);
			}
		}


		delete[] expr;

		return true;
	}

	map<operation, int> priorities =
	{
		{'(', 1},
		{')', 1},
		{'|', 2},
		{'&', 3},
		{'~', 4}
	};

	char* PolishNotation(char* expression)
	{
		char* polishExpression = new char[strlen(expression)];
		strcpy_s(polishExpression, strlen(expression), "");

		goThrowStr(expression, polishExpression);

		return polishExpression;
	}

	void go_to_pol(char* expr, map<char, IT::Entry*> dict, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		ushort len = strlen(expr);
		IT::Entry* edi;
		for (ushort i = 0; i < len; i++)
		{
			if (!isOperation(expr[i]))
			{
				edi = dict[expr[i]];

				if (edi->iddatatype == IT::FLT && len > 1)
				{
					edi->needToInt = true;
					cout << "| Предупреждение: переменная/функция/литерал ";
					if (edi->idtype == IT::L)
					{
						cout << edi->value.vflt;
					}
					else
					{
						cout << edi->id;
					}
					cout << " приведена к типу num при выполение побитовых операций! Строка: " << lextable.table[edi->idxfirstLE].sn << " |\n";
				}
				else if (edi->iddatatype == IT::ACTION)
				{
					throw ERROR_THROW_IN(628, lextable.table[edi->idxfirstLE].sn, -1);
				}
			}
		}
	}

	bool checkExpression(char* expression)
	{
		if (
			!checkBrackets(expression) ||
			!checkOperations(expression)
			)
		{
			return false;
		}
		return true;
	}

	bool checkBrackets(char* expression)
	{
		stack<char> brackets;

		for (int i = 0; i < strlen(expression); i++)
		{
			if (expression[i] == '(')
			{
				brackets.push(expression[i]);
			}
			else if (expression[i] == ')')
			{
				if (brackets.empty())
				{
					return false;
				}
				else
				{
					brackets.pop();
				}
			}
		}

		if (brackets.empty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool checkOperations(char* expression)
	{
		for (int i = 0; i < strlen(expression); i++)
		{
			if (isOperation(expression[i]) && !isBrackets(expression[i]))
			{
				if (i == 0)
				{
					return false;
				}
				else if (i == strlen(expression) - 1 && !isBrackets(expression[i]))
				{
					return false;
				}
				else if (isOperation(expression[i - 1]) && !isBrackets(expression[i - 1]))
				{
					return false;
				}
				else if (isOperation(expression[i + 1]) && !isBrackets(expression[i + 1]))
				{
					return false;
				}
			}
		}
		return true;
	}

	void goThrowStr(char* expr, char* polExprs)
	{
		stack<operation> _stack;
		int	polExprIndex = 0;

		for (auto i = 0; i < strlen(expr); i++)
		{
			if (isOperand(expr[i]))
			{
				polExprs[polExprIndex++] = expr[i];
			}
			else if (isOperation(expr[i]))
			{
				if (_stack.empty() ||
					expr[i] == '(' ||
					priorities[expr[i]] > priorities[_stack.top()])
				{
					_stack.push(expr[i]);
				}
				else
				{
					while (
						!_stack.empty() &&
						priorities[expr[i]] <= priorities[_stack.top()])
					{
						if (!isBrackets(_stack.top()))
						{
							polExprs[polExprIndex++] = _stack.top();
						}

						_stack.pop();
					}
					_stack.push(expr[i]);
				}
			}
		}

		polExprs[polExprIndex] = '\0';

		if (!_stack.empty())
		{
			writeTo(polExprs, _stack);
		}

	}

	void writeTo(char* expr, stack<operation> _stack)
	{
		int exprIndex = strlen(expr);

		while (!_stack.empty())
		{
			if (!isBrackets(_stack.top()))
			{
				expr[exprIndex++] = _stack.top();
			}

			_stack.pop();
		}

		expr[exprIndex] = '\0';
	}

	bool isOperand(char symb)
	{
		return
			(symb >= '0' && symb <= '9') ||
			(symb >= 'a' && symb <= 'z') ||
			(symb >= 'A' && symb <= 'Z');
	}

	bool isOperation(char symb)
	{
		return
			symb == '&' ||
			symb == '|' ||
			symb == '~' ||
			isBrackets(symb);
	}

	bool isBrackets(char symb)
	{
		return symb == '(' ||
			symb == ')';
	}
}