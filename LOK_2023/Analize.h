#pragma once
#include "stdafx.h"
using namespace std;
#define PARAMETR_OF_LYMBDA 'o'
#define FOR 'p'
#define SKIP 'z'

void LexAnalize( // ������� ��� ���������� ������������ �������
	In::IN in, // ������� ����
	LT::LexTable& lextable, // ������� ������
	IT::IdTable& idtable); // ������� ���������������

string toString(char* str); // ������� ������ char � ������ string

void check_id_size(string& id); // �������� �� ����� ��������������

bool isArtScopes(// ������� �������� �� ������� ������������� �������� ���������
	stack<string> scope, // ���� �������� ���������
	int i = 2); // i - �� ������ �������� ���������

bool isFloat(string& word); // �������� �� float

void insertToStr( // �������� � ������ �����
	char* str, // ������, � ������� ����� ��������� ������
	string insertedStr, // ������, ������� ����� ���������
	int index); // ������, ���� ���������

bool isStopSymbol(char symbol); // �������� �� ������ ����� ������. ���������� N, ���� ��� �� ����-������, ����� ���������� ���������� ������

void setLexemsAndIds( // ��������� ������ �� ������� � ��������������
	vector<string>& words, // ��� ����� � �������� ���������
	LT::LexTable& lexems, // ������� ������
	IT::IdTable& idtable); // ������� ���������������

bool isLiteral(string& word); // ��������� �����, �������� �� ��� ���������

bool isStr(string word); // ��������� �����, �������� �� ��� �������

bool isId(string word); // ��������� �����, �������� �� ��� ���������������

bool is_id_in_table( // ���������, ���� �� ������������� � ������� ���������������
	IT::IdTable& idtable, // ������� ���������������
	string word, // �����, ������� ����� ���������
	stack<string> scope,
	string* fullWord = nullptr); // ���� �������� ���������

bool is_id_in_this_scope( // ��������� ���������� � ������ ������� ���������
	IT::IdTable& idtable, // ������� ���������������
	string word, // �����
	stack<string> scope // ������� ���������
);

string getNewWord( // �������� ����� ����� �������� ������� ���������
	string word, // ��� ����������
	stack<string> scope // ������� ���������
);

bool nextIs(string word, vector<string>& words, int i); // �������� ��������� ����� ��������� word