#include "IT.h"
#include "stdafx.h"

namespace IT
{
	IdTable Create(int size)
	{
		if (size > TI_MAXSIZE)
		{
			throw ERROR_THROW(116);
		}

		IdTable idtable;

		idtable.maxsize = size;
		idtable.size = 0;
		idtable.table = new Entry[size];


		Entry* pEn = new Entry();

		pEn->hasValue = true;
		strcpy_s(pEn->id, "sum");
		pEn->iddatatype = IT::FLT;
		pEn->idtype = IT::F;
		pEn->countParams = 2;
		pEn->isFromStatic = true;

		Add(idtable, *pEn);

		strcpy_s(pEn->id, "minus");
		Add(idtable, *pEn);

		strcpy_s(pEn->id, "mult");
		Add(idtable, *pEn);

		strcpy_s(pEn->id, "division");
		Add(idtable, *pEn);

		delete pEn;

		return idtable;
	}

	void Add(
		IdTable& idtable,
		Entry entry)
	{
		if (idtable.size >= idtable.maxsize)
		{
			throw ERROR_THROW(117);
		}

		idtable.table[idtable.size] = entry;
		idtable.size++;
	}

	Entry GetEntry(
		IdTable& idtable,
		int n)
	{
		if (
			n >= idtable.maxsize ||
			n < 0)
		{
			throw ERROR_THROW(118);
		}

		return idtable.table[n];
	}

	int IsId(
		IdTable& idtable,
		char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0)
			{
				return i;
			}
		}

		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
		idtable.size = 0;
		idtable.maxsize = 0;
	}

	int GetIndexByLTIndex(IdTable& idtable, int index)
	{
		int ind = -1;

		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idxfirstLE == index)
			{
				ind = i;
				break;
			}
		}

		return ind;
	}

	void Output(IdTable& idtable)
	{
		int dist = 30;
		int di = 10;
		int dtd = -10;
		int dt = -5;
		int dind = 0;
		int dv = 0;

		fstream fout("IT.txt", fstream::out);
		Entry e;
		fout << "Таблица идентификаторов";
		fout << endl;

		string word;

		word = "Идентификатор";
		fout << word << setw(dist - word.size() + di) << setfill(' ') << "| ";

		word = "Тип данных";
		fout << word << setw(dist - word.size() + dtd) << setfill(' ') << "| ";

		word = "Тип идентификатора";
		fout << word << setw(dist - word.size() + dt) << setfill(' ') << "| ";


		word = "Индекс в таблице лексем";
		fout << word << setw(dist - word.size() + dind) << setfill(' ') << "| ";

		word = "Значение";
		fout << word << endl;

		for (ushort i = 0; i < idtable.size; i++)
		{
			e = idtable.table[i];
			fout << e.id << setw(dist - strlen(e.id) + di) << setfill(' ') << "| ";

			word = "";
			if (e.isRef)
			{
				word += "ref ";
			}
			switch (e.iddatatype)
			{
			case IT::NUM:
				word += "num";
				break;
			case IT::FLT:
				word += "float";
				break;
			case IT::STR:
				word += "string";
				break;
			case IT::ACTION:
				word += "action";
				break;
			case IT::SYMB:
				word += "symb";
				break;
			}
			fout << word << setw(dist - word.size() + dtd) << setfill(' ') << "| ";

			word = "";
			switch (e.idtype)
			{
			case IT::V:
				word += "переменная";
				break;
			case IT::P:
				word += "параметр";
				break;
			case IT::F:
				word += "функция";
				break;
			case IT::L:
				word += "литерал";
				break;
			}

			fout << word << setw(dist - word.size() + dt) << setfill(' ') << "| ";
			if (!e.isFromStatic || i > 3)
			{
				word = to_string(e.idxfirstLE);
			}
			else
			{
				word = "Из статической библиотеки";
			}

			fout << word << setw(dist - word.size() + dind) << setfill(' ') << "| ";

			word = "";
			if (e.idtype == IT::L)
			{
				switch (e.iddatatype)
				{
				case IT::NUM:
					word += to_string(e.value.vint);
					break;
				case IT::FLT:
					word += to_string(e.value.vflt);
					break;
				case IT::STR:
					word += e.value.vstr;
					break;
				case IT::SYMB:
					word += e.value.vsymb;
					break;
				}

				fout << word;
			}
			else
			{
				word = "Не определено";
				fout << word;
			}
			fout << endl;
		}

		fout << endl;
		fout.close();
	}
}