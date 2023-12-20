#pragma once

#define LEXEMA_FIXSIZE 1   // фиксированный размер лексемы
#define LT_MAXSIZE  4096 // максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX 0xffffffff // нет элемента таблицы индентификаторов
#define LEX_INTEGER 't' // лексема для integer
#define LEX_STRING 't' // лексема для string
#define LEX_ID 'i' // лексема для идентификатора
#define LEX_LITERAL 'l' // лексема для литерала
#define LEX_FUNCTION 'f' // лексема для функции
#define LEX_DECLARE 'd' // лексема для declare
#define LEX_RETURN 'r' // лексема для return
#define LEX_PRINT 'p' // лексема для print
#define LEX_SEMICOLON ';' // лексема для ;
#define LEX_COMMA ',' // лексема для ,
#define LEX_LEFTBRACE '{' // лексема для {
#define LEX_BRACELET '}' // лексема для }
#define LEX_LEFTHESIS '(' // лексема для (
#define LEX_RIGHTHESIS ')' // лексема для )
#define LEX_PLUS '+' // лексема для +
#define LEX_MINUS '-' // лексема для -
#define LEX_STAR '*' // лексема для *
#define LEX_DIRSLASH '/' // лексема для /

namespace LT // таблица лексем
{
	struct Entry
	{
		char lexema; // лексема
		char view; // вид лексемы
		int sn; // номер строки
		int idxTI; // индекс в таблице идентификаторов

		Entry()
		{
			view = 0;
		}
	};

	struct LexTable
	{
		int maxsize; // максимальный размер таблицы
		int size; // текущий размер таблицы
		Entry* table; // таблица лексем
	};

	LexTable Create( // создание таблицы лексем
		int size // максимальный размер таблицы
	);

	void Add( // добавление лексемы в таблицу
		LexTable& lextable, // таблица лексем
		Entry entry // лексема
	);

	Entry GetEntry( // получение лексемы из таблицы
		LexTable& lextable, // таблица лексем
		int n // номер лексемы
	);

	void Delete( // удаление таблицы лексем
		LexTable& lextable // таблица лексем
	);

	void Output(LexTable& lextable); // вывод таблицы лексем

}
