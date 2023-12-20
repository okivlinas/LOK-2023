#include "Check.h"

namespace CHECK
{
	Checker::Checker()
	{
		_is = new FST(
			CHECK_IS
		);
		_skip = new FST(
			CHECK_SKIP
		);
		_num = new FST(
			CHECK_NUM
		);
		_break = new FST(
			CHECK_BREAK
		);
		_for = new FST(
			CHECK_FOR
		);
		_symb = new FST(
			CHECK_SYMB
		);
		_float = new FST(
			CHECK_FLOAT
		);
		_action = new FST(
			CHECK_ACTION
		);
		_equalinit = new FST(
			CHECK_EQUALINIT
		);
		_console = new FST(
			CHECK_CONSOLE
		);
		_foo = new FST(
			CHECK_FOO
		);
		_leftscope = new FST(
			CHECK_LEFTSCOPE
		);
		_rightscope = new FST(
			CHECK_RIGHTSCOPE
		);
		_leftbracket = new FST(
			CHECK_LEFTBRACKET
		);
		_rightbracket = new FST(
			CHECK_RIGHTBRACKET
		);
		_comma = new FST(
			CHECK_COMMA
		);
		_allequalsigns = new FST(
			CHECK_EQUALSIGNS
		);

		_boperations = new FST(
			CHECK_OPERATION
		);
		_not = new FST(
			CHECK_NOT
		);
		_minus = new FST(
			CHECK_MINUS
		);
		_main = new FST(
			CHECK_MAIN
		);
		_return = new FST(
			CHECK_RETURN
		);
		_if = new FST(
			CHECK_IF
		);
		_truth = new FST(
			CHECK_TRUTH
		);
		_lie = new FST(
			CHECK_LIE
		);
		_semicomma = new FST(
			CHECK_SEMMICOMMA
		);
	}

	Checker::~Checker()
	{
		delete _is;
		delete _num;
		delete _semicomma;
		delete _skip;
		delete _break;
		delete _for;
		delete _symb;
		delete _float;
		delete _action;
		delete _equalinit;
		delete _console;
		delete _foo;
		delete _leftscope;
		delete _rightscope;
		delete _leftbracket;
		delete _rightbracket;
		delete _comma;
		delete _allequalsigns;
		delete _boperations;
		delete _not;
		delete _minus;
		delete _main;
		delete _return;
		delete _if;
		delete _truth;
		delete _lie;
	}

	bool check(FST* fst, const char* str)
	{
		fst->string = (char*)str;

		return execute(*fst);
	}
}