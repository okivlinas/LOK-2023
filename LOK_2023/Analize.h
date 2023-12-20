#pragma once
#include "stdafx.h"
using namespace std;
#define PARAMETR_OF_LYMBDA 'o'
#define FOR 'p'
#define SKIP 'z'

void LexAnalize( // функция для выполнения лексического анализа
	In::IN in, // входной файл
	LT::LexTable& lextable, // таблица лексем
	IT::IdTable& idtable); // таблица идентификаторов

string toString(char* str); // перевод строки char в строку string

void check_id_size(string& id); // проверка на длину идентификатора

bool isArtScopes(// Функция проверка на ниличие искусственных областей видимости
	stack<string> scope, // стек областей видимости
	int i = 2); // i - до какого элемента проверять

bool isFloat(string& word); // проверка на float

void insertToStr( // вставить в строку текст
	char* str, // строка, в которую будем вставлять строку
	string insertedStr, // строка, которая будет вставлена
	int index); // индекс, куда вставлять

bool isStopSymbol(char symbol); // проверка на символ конца строки. Возвращает N, если это не стоп-символ, иначе возвращает переданный символ

void setLexemsAndIds( // разбиение строки на лексемы и идентификаторы
	vector<string>& words, // все слова в тектовом документе
	LT::LexTable& lexems, // таблица лексем
	IT::IdTable& idtable); // таблица идентификаторов

bool isLiteral(string& word); // проверяет слово, является ли оно литералом

bool isStr(string word); // проверяет слово, является ли оно строкой

bool isId(string word); // проверяет слово, является ли оно идентификатором

bool is_id_in_table( // проверяет, есть ли идентификатор в таблице идентификаторов
	IT::IdTable& idtable, // таблица идентификаторов
	string word, // слово, которое нужно проверить
	stack<string> scope,
	string* fullWord = nullptr); // стек областей видимости

bool is_id_in_this_scope( // проверить переменную в данной области видимости
	IT::IdTable& idtable, // таблица идентификаторов
	string word, // слово
	stack<string> scope // область видимости
);

string getNewWord( // получить новое слово учитывая область видимости
	string word, // имя переменной
	stack<string> scope // область видимости
);

bool nextIs(string word, vector<string>& words, int i); // проверка следующее слово ялвляется word