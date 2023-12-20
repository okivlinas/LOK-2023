#pragma once
#include <vector>
#include <string>
#include <iostream>

#define ID_MAX_SIZE 50 // максимальная длина идентификатора
#define ID_MAXSIZE 450 // максимальное количество символов в идентификаторе
#define TI_MAXSIZE 4096 // максимальное количество записей в таблице идентификаторов
#define TI_INT_DEFAULT 0x00000000 // значение по умолчанию для типа integer
#define TT_STR_DEFAULT 0x00 // значение по умолчанию для типа string
#define TI_NULLIDX 0xffffffff // нет элемента в таблицы идентификаторов
#define TI_STR_MAXSIZE 255 

namespace IT
{
	enum IDDATATYPE // типы данных идентификаторов
	{
		NUM = 1,
		SYMB = 2,
		FLT = 3,
		ACTION = 4,
		STR = 5
	};

	enum IDTYPE // типы идентификаторов
	{
		V = 1, // переменная
		F = 2, // функция
		P = 3, // параметр
		L = 4, // литерал
	};

	struct Entry // запись в таблице идентификаторов
	{
		int idxfirstLE; // индекс первой строки в таблице лексем
		short countParams; // кол-во параметров, если это функция 
		char id[ID_MAXSIZE]; // идентификатор
		IDDATATYPE iddatatype; // тип данных идентификатора
		IDTYPE idtype; // тип идентификатора
		bool hasValue;
		bool isRef;
		bool isFromStatic;

		std::vector<std::string> params;

		bool needToInt;

		union
		{
			int vint; // значение для типа num

			char vsymb; // значение типа symb

			float vflt; // значение типа float

			char vstr[255]; // значение типа string
		} value;

		Entry()
		{
			hasValue = false;
			countParams = -1;
			needToInt = false;
			isRef = false;
			isFromStatic = false;
		}
	};

	struct IdTable // экземпляр таблицы идентификаторов
	{
		int maxsize; // максимальное количество записей в таблице идентификаторов < TI_MAXSIZE
		int size; // текущий размер таблицы идентификаторов < maxsize
		Entry* table; // массив строк таблицы идентификаторов
	};

	IdTable Create(int size); // создание таблицы идентификаторов

	void Add( // добавление записи в таблицу идентификаторов
		IdTable& idtable, // таблица идентификаторов
		Entry entry // строка таблицы идентификаторов
	);

	Entry GetEntry( // получение записи из таблицы идентификаторов
		IdTable& idtable, // таблица идентификаторов
		int n // номер записи в таблице идентификаторов
	);

	int IsId( // поиск идентификатора в таблице идентификаторов вернуть индекс или TI_NULLIDX
		IdTable& idtable, // таблица идентификаторов
		char id[ID_MAXSIZE] // идентификатор
	);

	void Delete(IdTable& idtable); // удаление таблицы идентификаторов

	int GetIndexByLTIndex( // найти индекс элемента в таблице идентификаторов по индексу в таблице лексем
		IdTable& idtable, // таблица идентификаторов
		int index); // индекс в таблице лексем

	void Output(IdTable& idtable); // вывод таблицы идентификаторов
}