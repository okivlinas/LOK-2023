#include "Semantic.h"

void checkSemantic(
	LT::LexTable& lextable, // таблица лексем
	IT::IdTable& idtable // таблица идентификаторов
)
{
	int right;
	int cntR;
	int left;
	int cntL;
	IT::Entry* ite;
	PN::Polish(lextable, idtable);
	for (int i = 0; i < lextable.size; i++)
	{
		if (lextable.table[i].lexema == 'i' && idtable.table[lextable.table[i].idxTI].iddatatype == IT::ACTION)
		{
			if (lextable.table[i + 1].lexema != 's' && (lextable.table[i - 1].lexema == ',' || lextable.table[i - 1].lexema == '(' || lextable.table[i - 1].lexema == 'v' || lextable.table[i - 1].lexema == '~'))
			{
				throw ERROR_THROW_IN(628, lextable.table[i].sn, 0);
			}
		}
	}
}