#include "GRB.h"

namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)
#define GRB_ERROR_SERIES 650


#define GLOBAL 'A'
#define EXPR 'B'
#define IPARAMS 'C'
#define IPARAMS_ARGS 'D'
#define CALLFUNC 'E'
#define ARGS_CFUNC 'F'
#define BODY_FUNC 'G'
#define INIT 'H'
#define IF 'I'
#define RETURN 'J'
#define TYPES_VALUES 'K'
#define IFBODY 'M'
#define CONSOLE 'L'
#define VISAREA 'N'
#define FOR_PARAM 'O'
#define FOR_BODY 'P'
#define INIT_FUNC 'T'
#define DEF_TYPE 'U'
#define BODY_ACTION 'V'
#define INIT_ACTION 'W'
#define SEMICOLON 'X'
#define RIGHTSCOPE 'Y'

	GRB::Greibach greibach(
		NS(GLOBAL),
		TS('$'),
		22, // edit

		Rule(NS(GLOBAL), GRB_ERROR_SERIES + 0, // глобальное пространство
			12,

			Rule::Chain(3, TS('i'), NS(INIT_FUNC), NS(GLOBAL)),
			Rule::Chain(2, TS('i'), NS(INIT_FUNC)),

			Rule::Chain(3, TS('i'), NS(INIT_ACTION), NS(GLOBAL)),
			Rule::Chain(2, TS('i'), NS(INIT_ACTION)),

			Rule::Chain(6, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON), NS(GLOBAL)),
			Rule::Chain(5, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON)),

			Rule::Chain(4, TS('i'), NS(DEF_TYPE), NS(SEMICOLON), NS(GLOBAL)),
			Rule::Chain(3, TS('i'), NS(DEF_TYPE), NS(SEMICOLON)),

			Rule::Chain(5, TS('m'), TS('{'), NS(BODY_FUNC), TS('}'), NS(GLOBAL)),
			Rule::Chain(4, TS('m'), TS('{'), NS(BODY_FUNC), TS('}')),

			Rule::Chain(4, TS('m'), TS('{'), NS(RETURN), TS('}')),
			Rule::Chain(5, TS('m'), TS('{'), NS(RETURN), TS('}'), NS(GLOBAL))
		),

		Rule(NS(INIT_FUNC), GRB_ERROR_SERIES + 16, // ошибка при инициализации функции
			2,
			Rule::Chain(7, TS('s'), TS('f'), NS(IPARAMS), NS(DEF_TYPE), TS('{'), NS(BODY_FUNC), TS('}')),
			Rule::Chain(7, TS('s'), TS('f'), NS(IPARAMS), NS(DEF_TYPE), TS('{'), NS(RETURN), TS('}'))
		),

		Rule(NS(DEF_TYPE), GRB_ERROR_SERIES + 17, // ошибка при определении типа
			1,
			Rule::Chain(2, TS('s'), TS('t'))
		),

		/////////////////////////

		Rule(NS(BODY_ACTION), GRB_ERROR_SERIES + 18, // ошибка в функции типа action
			24,

			Rule::Chain(4, TS('i'), NS(DEF_TYPE), NS(SEMICOLON), NS(BODY_ACTION)),
			Rule::Chain(3, TS('i'), NS(DEF_TYPE), NS(SEMICOLON)),

			Rule::Chain(6, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON), NS(BODY_ACTION)),
			Rule::Chain(5, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON)),

			Rule::Chain(5, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON), NS(BODY_ACTION)),
			Rule::Chain(4, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON)),

			Rule::Chain(6, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON), NS(BODY_ACTION)),
			Rule::Chain(5, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON)),

			Rule::Chain(5, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON), NS(BODY_ACTION)),
			Rule::Chain(4, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON)),

			Rule::Chain(4, TS('{'), NS(BODY_ACTION), TS('}'), NS(BODY_ACTION)),
			Rule::Chain(3, TS('{'), NS(BODY_ACTION), TS('}')),

			Rule::Chain(3, TS('{'), TS('}'), NS(BODY_FUNC)),
			Rule::Chain(2, TS('{'), TS('}')),

			Rule::Chain(4, TS('c'), NS(CONSOLE), NS(SEMICOLON), NS(BODY_ACTION)),
			Rule::Chain(3, TS('c'), NS(CONSOLE), NS(SEMICOLON)),

			Rule::Chain(3, TS(FOR), NS(FOR_PARAM), NS(BODY_ACTION)),
			Rule::Chain(2, TS(FOR), NS(FOR_PARAM)),

			Rule::Chain(8, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY), NS(BODY_ACTION)),
			Rule::Chain(7, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY)),

			Rule::Chain(4, TS('i'), NS(INIT_FUNC), NS(SEMICOLON), NS(BODY_ACTION)),
			Rule::Chain(3, TS('i'), NS(INIT_FUNC), NS(SEMICOLON)),

			Rule::Chain(2, TS('r'), NS(SEMICOLON)),
			Rule::Chain(3, TS('r'), NS(SEMICOLON), NS(BODY_ACTION))
		),

		Rule(NS(INIT_ACTION), GRB_ERROR_SERIES + 19,
			2,
			Rule::Chain(8, TS('s'), TS('f'), NS(IPARAMS), TS('s'), TS('a'), TS('{'), NS(BODY_ACTION), TS('}')),
			Rule::Chain(7, TS('s'), TS('f'), NS(IPARAMS), TS('s'), TS('a'), TS('{'), TS('}'))
		),

		Rule(NS(SEMICOLON), GRB_ERROR_SERIES + 20,
			1,
			Rule::Chain(1, TS(';'))
		),

		Rule(NS(RIGHTSCOPE), GRB_ERROR_SERIES + 21,
			1,
			Rule::Chain(1, TS(')'))
		),

		/////////////////////////

		Rule(NS(EXPR), GRB_ERROR_SERIES + 1, // выражение
			11,

			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(4, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE)),
			Rule::Chain(3, TS('i'), TS('('), NS(RIGHTSCOPE)),

			Rule::Chain(3, TS('i'), TS('v'), NS(EXPR)),
			Rule::Chain(3, TS('l'), TS('v'), NS(EXPR)),
			Rule::Chain(6, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), TS('v'), NS(EXPR)),

			Rule::Chain(5, TS('i'), TS('('), NS(RIGHTSCOPE), TS('v'), NS(EXPR)),

			Rule::Chain(3, TS('('), NS(EXPR), NS(RIGHTSCOPE)),
			Rule::Chain(5, TS('('), NS(EXPR), NS(RIGHTSCOPE), TS('v'), NS(EXPR)),

			Rule::Chain(2, TS('~'), NS(EXPR))
		),

		Rule(NS(IPARAMS), GRB_ERROR_SERIES + 2, // параметры
			2,
			Rule::Chain(2, TS('('), NS(RIGHTSCOPE)),
			Rule::Chain(3, TS('('), NS(IPARAMS_ARGS), NS(RIGHTSCOPE))
		),

		Rule(NS(IPARAMS_ARGS), GRB_ERROR_SERIES + 3, // инициализация параметров
			2,
			Rule::Chain(2, TS('i'), NS(DEF_TYPE)),
			Rule::Chain(4, TS('i'), NS(DEF_TYPE), TS(','), NS(IPARAMS_ARGS))
		),

		Rule(NS(CALLFUNC), GRB_ERROR_SERIES + 4, // вызов функции
			2,
			Rule::Chain(3, TS('i'), TS('('), NS(RIGHTSCOPE)),
			Rule::Chain(4, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE))
		),

		Rule(NS(ARGS_CFUNC), GRB_ERROR_SERIES + 5, // параметры вызываемой функции
			8,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(4, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE)),
			Rule::Chain(3, TS('i'), TS('('), NS(RIGHTSCOPE)),

			Rule::Chain(3, TS('i'), TS(','), NS(ARGS_CFUNC)),
			Rule::Chain(3, TS('l'), TS(','), NS(ARGS_CFUNC)),
			Rule::Chain(6, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), TS(','), NS(ARGS_CFUNC)),
			Rule::Chain(5, TS('i'), TS('('), NS(RIGHTSCOPE), TS(','), NS(ARGS_CFUNC))
		),

		Rule(NS(BODY_FUNC), GRB_ERROR_SERIES + 6, // тело функции
			24,

			Rule::Chain(6, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON), NS(BODY_FUNC)),
			Rule::Chain(6, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON), NS(RETURN)),

			Rule::Chain(5, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON), NS(BODY_FUNC)),
			Rule::Chain(5, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON), NS(RETURN)),

			Rule::Chain(4, TS('i'), NS(DEF_TYPE), NS(SEMICOLON), NS(BODY_FUNC)),
			Rule::Chain(4, TS('i'), NS(DEF_TYPE), NS(SEMICOLON), NS(RETURN)),

			Rule::Chain(6, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON), NS(BODY_FUNC)),
			Rule::Chain(6, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON), NS(RETURN)),

			Rule::Chain(5, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON), NS(BODY_FUNC)),
			Rule::Chain(5, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON), NS(RETURN)),

			Rule::Chain(4, TS('{'), NS(VISAREA), TS('}'), NS(BODY_FUNC)),
			Rule::Chain(4, TS('{'), NS(VISAREA), TS('}'), NS(RETURN)),

			Rule::Chain(3, TS('{'), TS('}'), NS(BODY_FUNC)),
			Rule::Chain(3, TS('{'), TS('}'), NS(RETURN)),

			Rule::Chain(4, TS('c'), NS(CONSOLE), NS(SEMICOLON), NS(BODY_FUNC)),
			Rule::Chain(4, TS('c'), NS(CONSOLE), NS(SEMICOLON), NS(RETURN)),

			Rule::Chain(3, TS(FOR), NS(FOR_PARAM), NS(BODY_FUNC)),
			Rule::Chain(3, TS(FOR), NS(FOR_PARAM), NS(RETURN)),

			Rule::Chain(8, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY), NS(BODY_FUNC)),
			Rule::Chain(8, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY), NS(RETURN)),

			Rule::Chain(4, TS('i'), NS(INIT_FUNC), NS(SEMICOLON), NS(BODY_FUNC)),
			Rule::Chain(4, TS('i'), NS(INIT_FUNC), NS(SEMICOLON), NS(RETURN)),

			Rule::Chain(4, TS('r'), NS(EXPR), NS(SEMICOLON), NS(BODY_FUNC)),
			Rule::Chain(3, TS('r'), NS(EXPR), NS(SEMICOLON))
		),

		Rule(NS(INIT), GRB_ERROR_SERIES + 7, // инициализация переменных
			1,
			Rule::Chain(3, TS('i'), TS('s'), TS('t'))
		),

		Rule(NS(IF), GRB_ERROR_SERIES + 8, // условие
			38,
			Rule::Chain(6, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(6, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON), NS(IF)),
			Rule::Chain(5, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON)),

			Rule::Chain(5, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(5, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON), NS(IF)),
			Rule::Chain(4, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON)),

			Rule::Chain(4, TS('i'), NS(DEF_TYPE), NS(SEMICOLON), NS(IF)),
			Rule::Chain(4, TS('i'), NS(DEF_TYPE), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(3, TS('i'), NS(DEF_TYPE), NS(SEMICOLON)),

			Rule::Chain(6, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON), NS(IF)),
			Rule::Chain(6, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(5, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON)),

			Rule::Chain(5, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON), NS(IF)),
			Rule::Chain(5, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(4, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON)),

			Rule::Chain(4, TS('{'), NS(IF), TS('}'), NS(IF)),
			Rule::Chain(4, TS('{'), NS(IF), TS('}'), NS(RETURN)),
			Rule::Chain(3, TS('{'), NS(IF), TS('}')),

			Rule::Chain(2, TS('{'), TS('}')),
			Rule::Chain(3, TS('{'), TS('}'), NS(IF)),
			Rule::Chain(3, TS('{'), TS('}'), NS(RETURN)),

			Rule::Chain(4, TS('i'), NS(INIT_FUNC), NS(SEMICOLON), NS(IF)),
			Rule::Chain(4, TS('i'), NS(INIT_FUNC), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(3, TS('i'), NS(INIT_FUNC), NS(SEMICOLON)),

			Rule::Chain(4, TS('c'), NS(CONSOLE), NS(SEMICOLON), NS(IF)),
			Rule::Chain(4, TS('c'), NS(CONSOLE), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(3, TS('c'), NS(CONSOLE), NS(SEMICOLON)),

			Rule::Chain(8, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY), NS(IF)),
			Rule::Chain(8, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY), NS(RETURN)),
			Rule::Chain(7, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY)),

			Rule::Chain(3, TS('r'), NS(EXPR), NS(SEMICOLON)),
			Rule::Chain(2, TS('r'), NS(SEMICOLON)),

			Rule::Chain(4, TS('r'), NS(EXPR), NS(SEMICOLON), NS(IF)),
			Rule::Chain(3, TS('r'), NS(SEMICOLON), NS(IF)),

			Rule::Chain(2, TS('b'), NS(SEMICOLON)),
			Rule::Chain(3, TS('b'), NS(SEMICOLON), NS(IF)),

			Rule::Chain(2, TS(SKIP), NS(SEMICOLON)),
			Rule::Chain(3, TS(SKIP), NS(SEMICOLON), NS(IF))
		),

		Rule(NS(RETURN), GRB_ERROR_SERIES + 9, // return
			2,
			Rule::Chain(3, TS('r'), NS(EXPR), NS(SEMICOLON)),
			Rule::Chain(2, TS('r'), NS(SEMICOLON))
		),

		Rule(NS(TYPES_VALUES), GRB_ERROR_SERIES + 10, // типы значений
			4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS('('), NS(RIGHTSCOPE)),
			Rule::Chain(4, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE))
		),

		Rule(NS(IFBODY), GRB_ERROR_SERIES + 11, // тело if
			8,
			Rule::Chain(4, TS('T'), TS('{'), NS(IF), TS('}')),
			Rule::Chain(8, TS('T'), TS('{'), NS(IF), TS('}'), TS('L'), TS('{'), NS(IF), TS('}')),
			Rule::Chain(4, TS('L'), TS('{'), NS(IF), TS('}')),

			Rule::Chain(3, TS('T'), TS('{'), TS('}')),
			Rule::Chain(3, TS('L'), TS('{'), TS('}')),

			Rule::Chain(7, TS('T'), TS('{'), TS('}'), TS('L'), TS('{'), NS(IF), TS('}')),
			Rule::Chain(7, TS('T'), TS('{'), NS(IF), TS('}'), TS('L'), TS('{'), TS('}')),
			Rule::Chain(6, TS('T'), TS('{'), TS('}'), TS('L'), TS('{'), TS('}'))
		),

		Rule(NS(CONSOLE), GRB_ERROR_SERIES + 12, // вызов потока вывода
			2,
			Rule::Chain(3, TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE)),
			Rule::Chain(2, TS('('), NS(RIGHTSCOPE))
		),

		Rule(NS(VISAREA), GRB_ERROR_SERIES + 13,
			38,
			Rule::Chain(6, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON), NS(VISAREA)),
			Rule::Chain(6, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(5, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON)),

			Rule::Chain(5, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON), NS(VISAREA)),
			Rule::Chain(5, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(4, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON)),

			Rule::Chain(4, TS('i'), NS(DEF_TYPE), NS(SEMICOLON), NS(VISAREA)),
			Rule::Chain(4, TS('i'), NS(DEF_TYPE), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(3, TS('i'), NS(DEF_TYPE), NS(SEMICOLON)),

			Rule::Chain(6, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON), NS(VISAREA)),
			Rule::Chain(6, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(5, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON)),

			Rule::Chain(5, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON), NS(VISAREA)),
			Rule::Chain(5, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(4, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON)),

			Rule::Chain(4, TS('{'), NS(VISAREA), TS('}'), NS(VISAREA)),
			Rule::Chain(4, TS('{'), NS(VISAREA), TS('}'), NS(RETURN)),
			Rule::Chain(3, TS('{'), NS(VISAREA), TS('}')),

			Rule::Chain(2, TS('{'), TS('}')),
			Rule::Chain(3, TS('{'), TS('}'), NS(VISAREA)),
			Rule::Chain(3, TS('{'), TS('}'), NS(RETURN)),

			Rule::Chain(4, TS('i'), NS(INIT_FUNC), NS(SEMICOLON), NS(VISAREA)),
			Rule::Chain(4, TS('i'), NS(INIT_FUNC), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(3, TS('i'), NS(INIT_FUNC), NS(SEMICOLON)),

			Rule::Chain(4, TS('c'), NS(CONSOLE), NS(SEMICOLON), NS(VISAREA)),
			Rule::Chain(4, TS('c'), NS(CONSOLE), NS(SEMICOLON), NS(RETURN)),
			Rule::Chain(3, TS('c'), NS(CONSOLE), NS(SEMICOLON)),

			Rule::Chain(8, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY), NS(VISAREA)),
			Rule::Chain(8, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY), NS(RETURN)),
			Rule::Chain(7, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY)),

			Rule::Chain(2, TS('b'), NS(SEMICOLON)),
			Rule::Chain(3, TS('b'), NS(SEMICOLON), NS(VISAREA)),

			Rule::Chain(4, TS('r'), NS(EXPR), NS(SEMICOLON), NS(VISAREA)),
			Rule::Chain(3, TS('r'), NS(SEMICOLON), NS(VISAREA)),

			Rule::Chain(3, TS('r'), NS(EXPR), NS(SEMICOLON)),
			Rule::Chain(2, TS('r'), NS(SEMICOLON)),

			Rule::Chain(2, TS(SKIP), NS(SEMICOLON)),
			Rule::Chain(3, TS(SKIP), NS(SEMICOLON), NS(VISAREA))
		),

		Rule(NS(FOR_PARAM), GRB_ERROR_SERIES + 14, // Ошибка в параметрах цикла
			1,
			Rule::Chain(13, TS('('), NS(TYPES_VALUES), TS(','), NS(TYPES_VALUES), TS(','), NS(TYPES_VALUES), TS(','), TS('i'), TS('{'), NS(FOR_BODY), TS('}'), NS(RIGHTSCOPE), NS(SEMICOLON))
		),

		Rule(NS(FOR_BODY), GRB_ERROR_SERIES + 15, // Ошибка в теле цикла For
			26,
			Rule::Chain(6, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON), NS(FOR_BODY)),
			Rule::Chain(5, TS('i'), TS('('), NS(ARGS_CFUNC), NS(RIGHTSCOPE), NS(SEMICOLON)),

			Rule::Chain(5, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON), NS(FOR_BODY)),
			Rule::Chain(4, TS('i'), TS('('), NS(RIGHTSCOPE), NS(SEMICOLON)),

			Rule::Chain(4, TS('i'), NS(DEF_TYPE), NS(SEMICOLON), NS(FOR_BODY)),
			Rule::Chain(3, TS('i'), NS(DEF_TYPE), NS(SEMICOLON)),

			Rule::Chain(6, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON), NS(FOR_BODY)),
			Rule::Chain(5, TS('i'), NS(DEF_TYPE), TS('='), NS(EXPR), NS(SEMICOLON)),

			Rule::Chain(5, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON), NS(FOR_BODY)),
			Rule::Chain(4, TS('i'), TS('='), NS(EXPR), NS(SEMICOLON)),

			Rule::Chain(4, TS('{'), NS(FOR_BODY), TS('}'), NS(FOR_BODY)),
			Rule::Chain(3, TS('{'), NS(FOR_BODY), TS('}')),
			Rule::Chain(2, TS('{'), TS('}')),
			Rule::Chain(3, TS('{'), TS('}'), NS(FOR_BODY)),

			Rule::Chain(4, TS('i'), NS(INIT_FUNC), NS(SEMICOLON), NS(FOR_BODY)),
			Rule::Chain(3, TS('i'), NS(INIT_FUNC), NS(SEMICOLON)),

			Rule::Chain(4, TS('c'), NS(CONSOLE), NS(SEMICOLON), NS(FOR_BODY)),
			Rule::Chain(3, TS('c'), NS(CONSOLE), NS(SEMICOLON)),

			Rule::Chain(3, TS(FOR), NS(FOR_PARAM), NS(FOR_BODY)),
			Rule::Chain(2, TS(FOR), NS(FOR_PARAM)),

			Rule::Chain(8, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY), NS(FOR_BODY)),
			Rule::Chain(7, TS('('), NS(TYPES_VALUES), TS('V'), NS(TYPES_VALUES), NS(RIGHTSCOPE), TS('?'), NS(IFBODY)),

			Rule::Chain(2, TS('b'), NS(SEMICOLON)),
			Rule::Chain(3, TS('b'), NS(SEMICOLON), NS(FOR_BODY)),

			Rule::Chain(2, TS(SKIP), NS(SEMICOLON)),
			Rule::Chain(3, TS(SKIP), NS(SEMICOLON), NS(FOR_BODY))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)	///I?aano. oaii?ee (i?aaay noi?iia i?aaeea)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i)
		{
			nt[i] = (GRBALPHABET)p[i];
		}
	};
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)	///I?aaeei a a?aiiaoeea A?aeaao
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	};
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	};
	Greibach getGreibach()
	{
		return greibach;
	};
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)	
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
		{
			k++;
		}
		if (k < size) prule = rules[rc = k];
		return rc;
	};
	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	};
	char* Rule::getCRule(char* b, short nchain)
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)
		{
			++j;
		}
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	};
	char* Rule::Chain::getCChain(char* b)	///iieo?eou oaii?eo a neia. aeaa
	{
		for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};
}
