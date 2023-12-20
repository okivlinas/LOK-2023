#pragma once
#include "stdafx.h"
typedef short GRBALPHABET; // символы алфавита грамматики, терминалы > 0, нетерминалы < 0

namespace GRB
{
	struct Rule // правило в грамматике Грейбах
	{
		GRBALPHABET nn; // нетерминал(левый символ правила) < 0
		int iderror; // идентификатор ошибки
		short size; // количество цепочек - правых частей правила

		struct Chain
		{
			short size; // длина цепочки
			GRBALPHABET* nt; // цепочка трерминалов (>0) и нетерминалов(<0)

			Chain(
				short psize, // количество символов в цепочке
				GRBALPHABET s, // символы (терминал или нетерминал)
				...
			);

			Chain() {}

			char* getCChain(char* b); // получить правую сторону правила

			static GRBALPHABET T(char t) // терминал
			{
				return GRBALPHABET(t);
			}

			static GRBALPHABET N(char n) // нетерминал
			{
				return -1 * GRBALPHABET(n);
			}

			static bool isT(GRBALPHABET s) // терминал?
			{
				return s > 0;
			}

			static bool isN(GRBALPHABET s) // нетерминал?
			{
				return s < 0;
			}

			static char alphabet_to_char(GRBALPHABET s) // GRBALPHABET -> char
			{
				return isT(s) ? char(s) : char(-s);
			}
		} *chains; // массив цепочек - правых частей правила

		Rule()
		{
			nn = 0x00;
			size = 0;
		}

		Rule(
			GRBALPHABET pnn, // нетерминал(<0)
			int iderror, // идентификатор ошибки
			short psize, // количество цепочек - правых частей правила
			Chain c, ... // множество цепочек - правых частей правила
		);

		char* getCRule( // получить правило в виде N->цепочка(для распечатки)
			char* b, // буфер для записи
			short nchain // номер цепочки (правой части) в правиле
		);

		short getNextChain( // получить следующую за j подходящую цепочку, вернуть ее номер или -1
			GRBALPHABET t, // первый символ цепочки
			Rule::Chain& pchain, // возвращаемая цепочка
			short j // номер цепочки
		);
	};

	struct Greibach // грамматика Грейбаха
	{
		short size; // количество правил
		GRBALPHABET startN; // стартовый символ
		GRBALPHABET stbottomT; // дно стека
		Rule* rules; // множество правил

		Greibach()
		{
			size = 0;
			startN = 0;
			stbottomT = 0;
			rules = nullptr;
		}

		Greibach(
			GRBALPHABET pstartN, // стартовый символ
			GRBALPHABET pstbottomT, // дно стека
			short psize, // количество правил
			Rule r, ... // множество правил
		);

		short getRule( // получить правило, возвращается номер правила или -1
			GRBALPHABET pnn, // левый символ правила
			Rule& prule // возвращаемое правило грамматики
		);

		Rule getRule(short n); // получить правило по номеру
	};

	Greibach getGreibach(); // получить грамматику Грейбаха
}