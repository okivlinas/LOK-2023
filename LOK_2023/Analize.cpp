#include "Analize.h"
#include "Parm.h"
#include "Check.h"
#include <random>
using namespace fst;

using CHECK::check;

#define BLOCK_SLASH_N
#define PARAMETR_OF_LYMBDA 'o'
#define FOR 'p'

void LexAnalize(
	In::IN in,
	LT::LexTable& lextable,
	IT::IdTable& idtable)
{
	map<string, string> funcsGDV =
	{
		{"pow",
		" pow is foo(number is num, power is num) is num { result => 1; For(1, power, 1, i => { result = mult(result, number); }); return result;\}"},

		{"symb_to_num",
		"symb_to_num is foo(s is symb) is num {(s == '0') ?Truth{return 0;}(s == '1') ? Truth{return 1;}(s == '2') ?Truth{return 2;}(s == '3') ?Truth{return 3;}(s == '4') ?Truth{return 4;}(s == '5') ?Truth{return 5;}(s == '6') ?Truth{return 6;}(s == '7') ?Truth{return 7;}(s == '8') ?Truth{return 8;}(s == '9') ?Truth{return 9;}return s;}"},

		{"abs",
		"abs is foo(number is num) is num {(number < 0) ? Truth { return mult(number, -1);}return number;}"},

		{"round",
		 "round is foo(a is float) is float { numa is num = a; (minus(a, numa) > .5) ? { return sum(numa, 1); } return numa; }"
		}
	};

	char* text = (char*)in.text;
	vector<string> words;
	string word = "";
	int line = 1;
	char symb;
	bool isComment = false;
	bool usedNM = false;

	srand(time(NULL));

	for (int i = 0; i < strlen(text); i++)
	{
		symb = text[i];

		if (symb == '\n')
		{
			line++;

			if (isComment)
			{
				words.push_back("\n");
				isComment = false;
				continue;
			}
		}

		if (!isComment)
		{
			if (symb == '*')
			{
				isComment = true;
				continue;
			}

			if (word == "...")
			{
				words.push_back("main");
				words.push_back("{");
				word = "";
				usedNM = true;
			}

			else if (word == "$R")
			{
				int num = rand() % 1000000 * pow(2, rand() % 4);
				word = to_string(num);
			}

			if (isStopSymbol(symb))
			{
				if (
					word != "" &&
					word != " " &&
					word != "\t"
#if !defined(BLOCK_SLASH_N)
					&& word != "\n"
#endif
					)
				{
					words.push_back(word);
					word = "";
				}

				if (
					symb == '=' &&
					text[i + 1] == '='
					)
				{
					word += symb;
					word += text[i + 1];

					i++;

					words.push_back(word);
					word = "";
				}
				else if (symb == '!')
				{
					word += symb;
					word += text[i + 1];

					i++;

					words.push_back(word);

					word = "";
				}
				else if (symb == '`')
				{
					word += symb;
					int j = i + 1;
					while (text[j] != '`')
					{
						if ((text[j] == '"' || text[j] == '\\') && j + 1 < strlen(text) && (text[j + 1] != 't' && text[j + 1] != 'n'))
						{
							word += '\\';
						}
						if (text[j] == '\n')
						{
							word += "\\n";
						}
						else
						{
							word += text[j];
						}
						j++;

						if (j == strlen(text))
						{
							throw ERROR_THROW(622);
						}
					}
					word += text[j];
					words.push_back(word);
					i = j;
				}
				else if (
					symb == '=' &&
					text[i + 1] == '>'
					)
				{
					word += symb;
					word += text[i + 1];

					i++;

					words.push_back(word);
				}
				else if (symb == '\'' && i + 3 < strlen(text))
				{
					word += symb;

					if (text[i + 1] != '\\')
					{
						if (text[i + 2] != '\'')
						{
							throw ERROR_THROW_IN(600, line, -1);
						}

						word += text[i + 1];

						word += text[i + 2];

						i += 2;
					}
					else
					{
						if (text[i + 2] == '\'')
						{
							word += text[i + 1];
							word += text[i + 2];
							i += 2;
						}
						else
						{
							if (text[i + 3] != '\'')
							{
								throw ERROR_THROW_IN(600, line, -1);
							}

							switch (text[i + 2])
							{
							case 'n':
								word += '\n';
								break;
							case 't':
								word += '\t';
								break;
							case 'r':
								word += '\r';
								break;
							default:
								throw ERROR_THROW_IN(621, line, -1);
							}

							word += text[i + 3];
							i += 3;
						}
					}
					words.push_back(word);
				}
				else if (
					symb != ' ' &&
					symb != '\t'
#if !defined(BLOCK_SLASH_N)
					&& symb != "\n"
#endif
					)
				{
					word += symb;

					words.push_back(word);
				}

				word = "";

				if (words.size() >= 3 && words[words.size() - 3] == "@import")
				{
					if (funcsGDV.find(words[words.size() - 2]) != funcsGDV.end())
					{
						insertToStr(text, funcsGDV[words[words.size() - 2]], i);
					}
					else
					{
						throw ERROR_THROW_IN(119, line, -1);
					}
					words.pop_back();
					words.pop_back();
					words.pop_back();
					i--;
				}
			}
			else
			{
				word += symb;
			}
		}
	}

	if (usedNM)
	{
		words.push_back("}");
	}

	setLexemsAndIds(words, lextable, idtable);
}

void setLexemsAndIds(
	vector<string>& words,
	LT::LexTable& lextable,
	IT::IdTable& idtable)
{
	string word;
	LT::Entry lexe;
	IT::Entry* ide;
	ushort line = 1;
	ushort countScopes = 0;

	ushort countActionScopes = 0;
	bool isAction = false;

	string* fullWord = new string;
	stack<bool> isFor;
	isFor.push(false);

	stack<string> scope;
	stack<string> tempScope;
	IT::Entry* tempId;

	bool isMain = false;
	int cntScopesRetInt = 0;

	bool* willBeTrue = nullptr;

	bool mainWas = false;

	int index;
	int indexOfScope = 0;
	scope.push("$");
	time(NULL);

	CHECK::Checker checker;

	for (int i = 0; i < words.size(); i++)
	{
		lexe.view = 0;
		word = words[i];

		if (word == ":")
		{
			word = "Lie";
		}

		if (word == "?")
		{
			if (nextIs("{", words, i + 1))
			{
				words.insert(words.begin() + i + 1, "Truth");
			}
		}

		if (word == "Lie" && lextable.table[lextable.size - 1].lexema == ')')
		{
			lexe.idxTI = -1;
			lexe.lexema = '?';
			lexe.sn = line;
			lexe.view = '?';

			LT::Add(lextable, lexe);
		}

		if (
			word == "}" &&
			isMain &&
			cntScopesRetInt == 1 &&
			lextable.size - 2 > 0 &&
			(lextable.table[lextable.size - 1].lexema == ';' || lextable.table[lextable.size - 1].lexema == '}'))
		{
			int j = lextable.size - 2;
			isMain = false;
			while (
				j >= 0 &&
				(
					lextable.table[j].lexema != '}' &&
					lextable.table[j].lexema != ';' &&
					lextable.table[j].lexema != '{'
					)
				)
			{
				j--;

				if (j == 0)
				{
					break;
				}
			}
			if (j > 0)
			{
				if (lextable.table[j + 1].lexema != 'r')
				{
					words.insert(words.begin() + i, "return");
					words.insert(words.begin() + i + 1, "0");
					words.insert(words.begin() + i + 2, ";");
					i--;
					continue;
				}
			}
		}

		if (
			i + 2 < words.size() &&
			words[i + 1] == "=>" &&
			nextIs("{", words, i + 2)
			)
		{
			bool in_table = is_id_in_table(idtable, words[i], scope);
			bool in_scope = is_id_in_this_scope(idtable, words[i], scope);
			ide = new IT::Entry();

			if (isLiteral(words[i]))
			{
				throw ERROR_THROW_IN(612, line, -1);
			}
			else if (isId(words[i]))
			{
				tempId = new IT::Entry();

				word = getNewWord(word, scope);

				strcpy_s(ide->id, word.c_str());

				ide->hasValue = true;
				ide->idtype = IT::P;
				ide->idxfirstLE = lextable.size;

				int j = i - 2;

				while (j >= 0 && words[j] != ",")
				{
					if (j == 0 && words[j] != ",")
					{
						throw ERROR_THROW_IN(613, line, -1);
					}
					j--;
				}

				j++;

				if (isId(words[j]))
				{
					is_id_in_table(idtable, word, scope, fullWord);

					if ((index = IT::IsId(idtable, (char*)fullWord->c_str()) != TI_NULLIDX))
					{
						*tempId = IT::GetEntry(idtable, index);
						ide->iddatatype = tempId->iddatatype;
					}
					else
					{
						throw ERROR_THROW_IN(609, line, -1);
					}
				}
				else if (isLiteral(words[j]))
				{
					ide->iddatatype = words[j][0] == '\'' ? IT::SYMB
						: isFloat(words[j]) ? IT::FLT :
						IT::NUM;
				}
				else
				{
					throw ERROR_THROW_IN(614, line, -1);
				}

				j = i;

				delete tempId;
			}

			i++;
			IT::Add(idtable, *ide);

			lexe.idxTI = idtable.size - 1;
			lexe.lexema = 'i';
			lexe.sn = line;
			lexe.view = 'v';

			LT::Add(lextable, lexe);

			delete ide;
			continue;
		}
		else if (
			i + 1 < words.size() &&
			words[i + 1] == "=>")
		{
			*fullWord = words[i + 2];
			words[i + 1] = "is";

			if (!isId(word))
			{
				throw ERROR_THROW_IN(632, line, -1);
			}

			if (isLiteral(*fullWord))
			{
				if (fullWord->c_str()[0] == '\'')
				{
					words.insert(words.begin() + i + 2, "symb");
				}
				else if (isFloat(*fullWord))
				{
					words.insert(words.begin() + i + 2, "float");
				}
				else
				{
					words.insert(words.begin() + i + 2, "num");
				}
			}

			else if (is_id_in_table(idtable, *fullWord, scope, fullWord))
			{
				index = IT::IsId(idtable, (char*)fullWord->c_str());

				tempId = new IT::Entry();

				*tempId = IT::GetEntry(idtable, index);

				if (tempId->iddatatype == IT::NUM)
				{
					words.insert(words.begin() + i + 2, "num");
				}
				else if (tempId->iddatatype == IT::FLT)
				{
					words.insert(words.begin() + i + 2, "float");
				}
				else
				{
					words.insert(words.begin() + i + 2, "symb");
				}

				delete tempId;
			}
			else
			{
				throw ERROR_THROW_IN(608, line, -1);
			}

			words.insert(words.begin() + i + 3, "=");
		}

		if (check(checker._is, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 's';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == "\n")
		{
			line++;
			if (i - 1 > 0 && words[i - 1] != "\n")
			{
				/*lexe.idxTI = -1;
				lexe.lexema = '\n';
				lexe.sn = line;

				LT::Add(lextable, lexe);*/
			}
		}

		else if (check(checker._num, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 't';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._break, word.c_str()))
		{
			if (!isFor.top())
			{
				throw ERROR_THROW_IN(616, line, -1);
			}
			lexe.idxTI = -1;
			lexe.lexema = 'b';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._for, word.c_str()))
		{
			scope.push("$For" + to_string(countScopes++));
			lexe.lexema = FOR;
			lexe.sn = line;
			lexe.idxTI = -1;

			int j = i + 1;
			isFor.push(true);
			while (words[j] != "=>")
			{
				if (j == words.size())
				{
					throw ERROR_THROW_IN(615, line, -1);
				}
				j++;
			}

			j--; // i/l,el 
			j--; // i/l,
			j--; // i/l

			is_id_in_table(idtable, words[j], scope, fullWord);

			if (isLiteral(words[j]))
			{
				lexe.view = (words[j][0] == '\'' ? 's' : isFloat(words[j]) ? 'f' : 'n');
			}
			else if ((index = IT::IsId(idtable, (char*)fullWord->c_str())) != TI_NULLIDX)
			{
				tempId = new IT::Entry();

				*tempId = IT::GetEntry(idtable, index);
				lexe.view = (tempId->iddatatype == IT::NUM ? 'n' : tempId->iddatatype == IT::FLT ? 'f' : 's');

				delete tempId;
			}

			LT::Add(lextable, lexe);
		}

		else if (check(checker._symb, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 't';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._float, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 't';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._action, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 'a';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._skip, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = SKIP;
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._equalinit, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = '=';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			if (words[i - 1] == "num" || words[i - 1] == "symb" || words[i - 1] == "float")
			{
				*fullWord = words[i - 3];
			}
			else if (isId(words[i - 1]))
			{
				*fullWord = words[i - 1];
			}
			else
			{
				throw ERROR_THROW_IN(607, line, -1);
			}

			if (!is_id_in_table(idtable, *fullWord, scope, fullWord))
			{
				throw ERROR_THROW_IN(602, line, -1);
			}

			index = IT::IsId(idtable, (char*)fullWord->c_str());

			willBeTrue = &idtable.table[index].hasValue;
		}

		else if (check(checker._semicomma, word.c_str()))
		{
			if (i - 1 > 0 && lextable.table[lextable.size - 1].lexema == 't')
			{
				lexe.idxTI = -1;
				lexe.lexema = '=';
				lexe.sn = line;
				lexe.view = '=';

				LT::Add(lextable, lexe);

				lexe.idxTI = idtable.size;
				lexe.lexema = 'l';
				lexe.sn = line;

				LT::Add(lextable, lexe);

				ide = new IT::Entry();

				ide->hasValue = true;
				strcpy_s(ide->id, "literal");
				ide->iddatatype = IT::NUM;
				ide->idtype = IT::L;
				ide->idxfirstLE = lextable.size - 1;
				ide->value.vint = 0;
				IT::Add(idtable, *ide);

				delete ide;
			}
			lexe.idxTI = -1;
			lexe.lexema = ';';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			if (willBeTrue != nullptr)
			{
				*willBeTrue = true;
				willBeTrue = nullptr;
			}
		}

		else if (check(checker._console, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 'c';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._foo, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 'f';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			scope.push(words[i - 2]);

			ide = new IT::Entry();

			*ide = IT::GetEntry(idtable, IT::GetIndexByLTIndex(idtable, lextable.size - 1 - 1 - 1));

			if (ide->iddatatype == IT::ACTION)
			{
				isAction = true;
				countActionScopes = 0;
			}

			if (isFor.top())
			{
				throw ERROR_THROW_IN(633, line, -1);
			}

			delete ide;
		}

		else if (check(checker._leftscope, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = '(';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._rightscope, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = ')';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._leftbracket, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = '{';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			if (lextable.table[lextable.size - 2].lexema != 't')
			{
				scope.push("$l" + to_string(indexOfScope++));
			}

			countScopes++;
			cntScopesRetInt++;

			if (isAction)
			{
				countActionScopes++;
			}
		}

		else if (check(checker._rightbracket, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = '}';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			scope.pop();

			countScopes--;
			cntScopesRetInt--;

			if (
				i + 1 < words.size() &&
				words[i + 1] == ")"
				)
			{
				isFor.pop();
				scope.pop();
			}

			if (isAction)
			{
				countActionScopes--;

				if (countActionScopes == 0)
				{
					scope.pop();
					isAction = false;
				}
			}
		}

		else if (check(checker._comma, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = ',';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._allequalsigns, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 'V';
			lexe.sn = line;
			lexe.view = word.c_str()[0];

			LT::Add(lextable, lexe);
		}

		else if (check(checker._boperations, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 'v';
			lexe.sn = line;
			lexe.view = word[0];

			LT::Add(lextable, lexe);
		}

		else if (check(checker._minus, word.c_str()))
		{
			if (
				i + 1 < words.size() &&
				isLiteral(words[i + 1]) &&
				words[i + 1][0] != '\'')
			{
				words[i + 1] = "-" + words[i + 1];
			}
			else
			{
				throw ERROR_THROW_IN(120, line, -1);
			}
		}

		else if (check(checker._not, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = '~';
			lexe.sn = line;
			lexe.view = word[0];

			LT::Add(lextable, lexe);
		}

		else if (check(checker._main, word.c_str()))
		{
			if (mainWas)
			{
				throw ERROR_THROW_IN(629, line, -1);
			}
			lexe.idxTI = -1;
			lexe.lexema = 'm';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			mainWas = true;

			scope.push("main");

			isMain = true;
		}

		else if (check(checker._return, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 'r';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			if (isAction && isArtScopes(scope))
			{
				if (!nextIs(";", words, i + 1))
				{
					throw ERROR_THROW_IN(630, line, -1);
				}
			}
			else if (nextIs(";", words, i + 1))
			{
				throw ERROR_THROW_IN(631, line, -1);
			}

			if (isFor.top())
			{
				throw ERROR_THROW_IN(634, line, -1);
			}
		}

		else if (check(checker._if, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = '?';
			lexe.sn = 0;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._truth, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 'T';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (check(checker._lie, word.c_str()))
		{
			lexe.idxTI = -1;
			lexe.lexema = 'L';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (isLiteral(word))
		{
			lexe.idxTI = -1;
			lexe.lexema = 'l';
			lexe.sn = line;
			lexe.idxTI = idtable.size;
			bool changed = false;

			ide = new IT::Entry();

			ide->idxfirstLE = lextable.size;
			ide->idtype = IT::L;
			ide->iddatatype = word[0] == '\'' ? IT::SYMB
				: isFloat(word) ? IT::FLT
				: isStr(word) ? IT::STR : IT::NUM;
			ide->hasValue = true;

			if (ide->iddatatype == IT::NUM)
			{
				long long ln = stoll(word);

				if (ln > INT_MAX)
				{
					throw ERROR_THROW_IN(624, line, -1);
				}
				else if (ln < INT_MIN)
				{
					throw ERROR_THROW_IN(625, line, -1);
				}

				ide->value.vint = ln;
			}
			else if (ide->iddatatype == IT::FLT)
			{
				for (auto& i : word)
				{
					if (i == '.')
					{
						i = ',';
					}
				}
				try
				{
					ide->value.vflt = stof(word);
				}
				catch (...)
				{
					throw ERROR_THROW_IN(626, line, -1);
				}
			}
			else if (ide->iddatatype == IT::STR)
			{
				word.erase(word.end() - 1);
				word.erase(word.begin());

				if (word.size() > 255)
				{
					throw ERROR_THROW_IN(627, line, -1);
				}

				strcpy_s(ide->value.vstr, word.c_str());

				if (lextable.size - 1 > 0 &&
					lextable.table[lextable.size - 1].lexema != ',' &&
					lextable.table[lextable.size - 1].lexema != '(' &&
					(
						lextable.table[lextable.size - 1].lexema == '{' || lextable.table[lextable.size - 1].lexema == '}' || lextable.table[lextable.size - 1].lexema == ';')
					)
				{
					changed = true;
					lexe.lexema = 'c';
					LT::Add(lextable, lexe);
					lexe.lexema = '(';
					LT::Add(lextable, lexe);
					lexe.lexema = 'l';
					LT::Add(lextable, lexe);
					lexe.lexema = ')';
					LT::Add(lextable, lexe);
					lexe.lexema = ';';
					LT::Add(lextable, lexe);
					lexe.lexema = 'c';
					LT::Add(lextable, lexe);
					lexe.lexema = '(';
					LT::Add(lextable, lexe);
					lexe.lexema = ')';
					LT::Add(lextable, lexe);
					if (!nextIs(";", words, i + 1))
					{
						lexe.lexema = ';';
						LT::Add(lextable, lexe);

						ide->idxfirstLE = lextable.size - 7;
					}
					else
					{
						ide->idxfirstLE = lextable.size - 6;
					}
				}
			}
			else
			{
				ide->value.vsymb = word[1];
			}

			strcpy_s(ide->id, "literal");

			IT::Add(idtable, *ide);
			if (!changed)
			{
				LT::Add(lextable, lexe);
			}

			delete ide;
		}

		else if (isId(word))
		{
			lexe.idxTI = idtable.size;
			lexe.lexema = 'i';
			lexe.sn = line;
			lexe.idxTI = idtable.size;

			check_id_size(word);

			bool in_scope = is_id_in_this_scope(idtable, word, scope);
			bool in_table = is_id_in_table(idtable, word, scope);

			/////////////////////////////////////////

			/*if (
				nextIs("is", words, i + 1) &&
				(nextIs("num", words, i + 2) || nextIs("symb", words, i + 2) || nextIs("float", words, i + 2) || nextIs("foo", words, i + 2) ||
					(nextIs("ref", words, i + 2) && (nextIs("num", words, i + 3) || nextIs("symb", words, i + 3) || nextIs("symb", words, i + 3)))) &&
				words[i + 4] != "?" &&
				!in_scope
				)*/

			if (
				words[i + 1] == "is" &&
				(words[i + 2] == "num" || words[i + 2] == "symb" || words[i + 2] == "float" || words[i + 2] == "foo" ||
					(words[i + 2] == "ref" && (words[i + 3] == "num" || words[i + 3] == "symb" || words[i + 3] == "float"))) &&
				words[i + 4] != "?" &&
				!in_scope
				)
			{
				ide = new IT::Entry();

				word = getNewWord(word, scope);

				strcpy_s(ide->id, word.c_str());
				ide->idxfirstLE = lextable.size;

				if (words[i + 2] == "ref")
				{
					ide->isRef = true;
					words.erase(words.begin() + i + 2);
				}

				switch (words[i + 2][0])
				{
				case 'n':
					ide->iddatatype = IT::NUM;
					break;
				case 's':
					ide->iddatatype = IT::SYMB;
					break;
				case 'f':
					if (words[i + 2][1] != 'l')
					{

					}
					else
					{
						ide->iddatatype = IT::FLT;
						break;
					}
				case '.':
					ide->idtype = IT::F;
					ide->countParams = 0;

					if (words[i + 3] == "(")
					{
						int j = i + 3;

						while (words[j] != ")")
						{
							j++;

							if (words[j] == "is")
							{
								ide->countParams++;
								ushort itmp = 1;

								if (j + 1 < words.size() && words[j + 1] == "ref")
								{
									itmp++;
								}

								if (
									j + itmp < words.size() &&
									(words[j + itmp] == "num" || words[j + itmp] == "symb" || words[j + itmp] == "float"))
								{
									if (words[j + itmp] == "num")
									{
										ide->params.push_back(itmp == 1 ? "int" : "int&");
									}
									else if (words[j + itmp] == "symb")
									{
										ide->params.push_back(itmp == 1 ? "char" : "char&");
									}
									else if (words[j + itmp] == "float")
									{
										ide->params.push_back(itmp == 1 ? "float" : "float&");
									}
								}
							}

							if (
								j + 1 == words.size() &&
								words[j] != ")"
								)
							{
								throw ERROR_THROW_IN(604, line, i);
							}
						}

						if (
							words[j + 1] == "is" &&
							(words[j + 2] == "num" || words[j + 2] == "symb" || words[j + 2] == "action" || words[j + 2] == "float")
							)
						{
							if (words[j + 2] == "action")
							{
								ide->iddatatype = IT::ACTION;
							}
							else
							{
								ide->iddatatype = (words[j + 2] == "num" ? IT::NUM :
									words[j + 2] == "float" ? IT::FLT : IT::SYMB);
							}

						}
						else
						{
							throw ERROR_THROW_IN(605, line, -1);
						}
					}
					else
					{
						throw ERROR_THROW_IN(603, line, -1);
					}

					break;
				}

				if (
					ide->idtype != IT::F &&
					i > 0 &&
					(words[i - 1] == "," || words[i - 1] == "(") &&
					words[i + 1] == "is")
				{
					ide->idtype = IT::P;
				}
				else if (ide->idtype != IT::F)
				{
					ide->idtype = IT::V;
				}

				if (ide->idtype == IT::P)
				{
					ide->hasValue = true;
				}

				if (ide->isRef && ide->idtype != IT::P)
				{
					throw ERROR_THROW_IN(618, line, -1);
				}

				IT::Add(idtable, *ide);

				delete ide;
			}
			else if (
				words[i - 1] == "(" &&
				words[i + 1] == "is" &&
				(words[i + 2] == "num" || words[i + 2] == "symb" || words[i + 2] == "float") &&
				words[i + 3] == ")" &&
				words[i + 4] == "?" &&
				is_id_in_table(idtable, word, scope, fullWord)
				)
			{
				ide = new IT::Entry;

				index = IT::IsId(idtable, (char*)fullWord->c_str());

				if (index != TI_NULLIDX)
				{
					*ide = idtable.table[index];

					if (
						(ide->iddatatype == IT::NUM && words[i + 2] == "num") ||
						(ide->iddatatype == IT::SYMB && words[i + 2] == "symb") ||
						(ide->iddatatype == IT::FLT && words[i + 2] == "float")
						)
					{
						words[i] = "1";
						words[i + 1] = ">";
						words[i + 2] = "0";
					}
					else
					{
						words[i] = "0";
						words[i + 1] = ">";
						words[i + 2] = "1";
					}

					i--;
					continue;
				}

				delete ide;
			}
			else if (!in_table)
			{
				throw ERROR_THROW_IN(609, line, -1);
			}
			else if (
				words[i + 1] == "is" &&
				(words[i + 2] == "num" || words[i + 2] == "symb" || words[i + 2] == "float" || words[i + 2] == "foo") &&
				words[i + 3] != ")" &&
				in_scope
				)
			{
				throw ERROR_THROW_IN(606, line, -1);
			}
			else if (is_id_in_table(idtable, word, scope, fullWord))
			{
				ide = new IT::Entry();

				index = IT::IsId(idtable, (char*)fullWord->c_str());

				*ide = IT::GetEntry(idtable, index);

				ide->idxfirstLE = lextable.size;

				if (ide->idtype == IT::F && !nextIs("(", words, i + 1))
				{
					throw ERROR_THROW_IN(620, line, -1);
				}
				else if (ide->idtype != IT::F && nextIs("(", words, i + 1))
				{
					words.insert(words.begin() + i + 1, "&");
				}

				IT::Add(idtable, *ide);

				if (
					ide->idtype == IT::F &&
					i + 1 < words.size() &&
					words[i + 1] != "("
					)
				{
					throw ERROR_THROW_IN(617, line, -1);
				}

				if (!ide->hasValue && ide->idtype != IT::P && ide->idtype != IT::F)
				{
					throw ERROR_THROW_IN(602, line, -1);
				}

				delete ide;
			}
			LT::Add(lextable, lexe);
		}

		else
		{
			throw ERROR_THROW_IN(601, line, -1);
		}
	}
}


string toString(char* str)
{
	string s = "";

	for (int i = 0; str[i] != '\0'; i++)
	{
		s += str[i];
	}

	return s;
}

bool isStopSymbol(char symbol)
{
	const char* stopSymbols = " \t\n;,{}()?>:<=`,&|~!\'";

	for (short i = 0; i < strlen(stopSymbols); i++)
	{
		if (symbol == stopSymbols[i])
		{
			return true;
		}
	}

	return false;
}

bool is16Num(string& word)
{
	FST reg16Num(
		(char*)word.c_str(),
		4,
		NODE(3, RELATION('-', 1), RELATION('+', 1), RELATION('x', 2)),
		NODE(1, RELATION('x', 2)),
		NODE(32, RELATION('0', 3), RELATION('0', 2), RELATION('1', 3), RELATION('1', 2), RELATION('2', 3), RELATION('2', 2), RELATION('3', 3), RELATION('3', 2), RELATION('4', 3), RELATION('4', 2), RELATION('5', 3), RELATION('5', 2), RELATION('6', 3), RELATION('6', 2), RELATION('7', 3), RELATION('7', 2), RELATION('8', 3), RELATION('8', 2), RELATION('9', 3), RELATION('9', 2), RELATION('A', 3), RELATION('A', 2), RELATION('B', 3), RELATION('B', 2), RELATION('C', 3), RELATION('C', 2), RELATION('D', 3), RELATION('D', 2), RELATION('E', 3), RELATION('E', 2), RELATION('F', 3), RELATION('F', 2)),
		NODE()
	);

	long long num = 0;
	bool isMinus = (word.front() == '-');

	if (execute(reg16Num))
	{
		for (ushort i = word.size() - 1, j = 0; word[i] != 'x'; i--, j++)
		{
			if (!(word[i] >= 'A' && word[i] <= 'F'))
			{
				num += (word[i] - '0') * pow(16, j);
			}
			else
			{
				num += (word[i] - 'A' + 10) * pow(16, j);
			}
		}

		if (isMinus)
		{
			num *= -1;
		}

		word = to_string(num);

		return true;
	}

	return false;
}

bool isNum(string& word)
{
	FST regNum(
		(char*)word.c_str(),
		3,
		NODE(24, RELATION('+', 1), RELATION('-', 1), RELATION('0', 1), RELATION('0', 2), RELATION('1', 1), RELATION('1', 2), RELATION('2', 1), RELATION('2', 2), RELATION('3', 1), RELATION('3', 2), RELATION('4', 1), RELATION('4', 2), RELATION('5', 1), RELATION('5', 2), RELATION('6', 1), RELATION('6', 2), RELATION('7', 1), RELATION('7', 2), RELATION('8', 1), RELATION('8', 2), RELATION('9', 1), RELATION('9', 2), RELATION('_', 1), RELATION('_', 2)),
		NODE(22, RELATION('_', 1), RELATION('_', 2), RELATION('0', 1), RELATION('0', 2), RELATION('1', 1), RELATION('1', 2), RELATION('2', 1), RELATION('2', 2), RELATION('3', 1), RELATION('3', 2), RELATION('4', 1), RELATION('4', 2), RELATION('5', 1), RELATION('5', 2), RELATION('6', 1), RELATION('6', 2), RELATION('7', 1), RELATION('7', 2), RELATION('8', 1), RELATION('8', 2), RELATION('9', 1), RELATION('9', 2)),
		NODE()
	);

	if (execute(regNum))
	{
		for (int i = 0; i < word.size(); i++)
		{
			if (word[i] == '_')
			{
				word.erase(i, 1);
			}
		}
		return true;
	}

	return false;
}

bool isLiteral(string& word)
{
	bool isOnly = true;
	for (auto i : word)
	{
		if (i != '_')
		{
			isOnly = false;
			break;
		}
	}
	if (isOnly)
		return false;
	regex regSymb("'[^☺]'");

	return isNum(word) || regex_match(word.begin(), word.end(), regSymb) || isFloat(word) || isStr(word) || is16Num(word);
}

bool isFloat(string& word)
{
	char el;
	bool withSign = false;

	if ((word.front() == '-' || word.front() == '+'))
	{
		el = word.front();
		withSign = true;
		word.erase(0, 1);
	}

	FST fstFloat(
		(char*)word.c_str(),
		4,
		NODE(22, RELATION('_', 0), RELATION('.', 2), RELATION('0', 1), RELATION('0', 0), RELATION('1', 1), RELATION('1', 0), RELATION('2', 1), RELATION('2', 0), RELATION('3', 1), RELATION('3', 0), RELATION('4', 1), RELATION('4', 0), RELATION('5', 1), RELATION('5', 0), RELATION('6', 1), RELATION('6', 0), RELATION('7', 1), RELATION('7', 0), RELATION('8', 1), RELATION('8', 0), RELATION('9', 1), RELATION('9', 0)),
		NODE(1, RELATION('.', 2)),
		NODE(22, RELATION('_', 2), RELATION('_', 3), RELATION('0', 2), RELATION('0', 3), RELATION('1', 2), RELATION('1', 3), RELATION('2', 2), RELATION('2', 3), RELATION('3', 2), RELATION('3', 3), RELATION('4', 2), RELATION('4', 3), RELATION('5', 2), RELATION('5', 3), RELATION('6', 2), RELATION('6', 3), RELATION('7', 2), RELATION('7', 3), RELATION('8', 2), RELATION('8', 3), RELATION('9', 2), RELATION('9', 3), RELATION('_', 2), RELATION('_', 3)),
		NODE()
	);

	if (execute(fstFloat))
	{
		if (withSign)
		{
			word = el + word;
		}

		return true;
	}

	if (withSign)
	{
		word = el + word;
	}

	return false;
}

bool isId(string word)
{
	bool isOnly = true;
	for (auto i : word)
	{
		if (i != '_')
		{
			isOnly = false;
			break;
		}
	}

	if (isOnly)
	{
		return false;
	}

	regex reg("[A-Za-z_][A-Za-z0-9_]*");

	return regex_match(word.begin(), word.end(), reg);
}

void check_id_size(string& id)
{
	while (id.size() > ID_MAX_SIZE)
	{
		id.pop_back();
	}
}

bool is_id_in_table(
	IT::IdTable& idtable,
	string word,
	stack<string> scope,
	string* fullWord)
{
	string temp = word;
	int index;

	index = IT::IsId(idtable, (char*)word.c_str());

	if (index != TI_NULLIDX && (word == "sum" || word == "mult" || word == "division" || word == "minus"))
	{
		if (fullWord != nullptr)
		{
			*fullWord = word;
		}
		return true;
	}

	while (true)
	{
		temp = word;

		temp = getNewWord(temp, scope);

		index = IT::IsId(idtable, (char*)temp.c_str());

		if (index == TI_NULLIDX)
		{
			if (scope.empty())
			{
				return false;
			}
			else
			{
				scope.pop();
			}
		}
		else
		{
			if (fullWord != nullptr)
			{
				*fullWord = temp;
			}

			return true;
		}
	}
}

bool is_id_in_this_scope(
	IT::IdTable& idtable,
	string word,
	stack<string> scope
)
{
	word = getNewWord(word, scope);

	auto index = IT::IsId(idtable, (char*)word.c_str());

	if (index == TI_NULLIDX)
	{
		return false;
	}
	else
	{
		return true;
	}
}

string getNewWord(string word, stack<string> scope)
{
	while (!scope.empty() && scope.top() != "")
	{
		word += "." + scope.top();
		scope.pop();
	}

	return word;
}

void insertToStr(
	char* str,
	string insertedStr,
	int index)
{
	int len = strlen(str);

	string after = "";
	string before = "";

	for (int i = 0; i < len; i++)
	{
		if (i < index)
		{
			before += str[i];
		}
		else
		{
			after += str[i];
		}
	}

	string res = before + insertedStr + after;
	strcpy_s(str, res.c_str());
}

bool nextIs(string word, vector<string>& words, int i)
{
	for (; i < words.size(); i++)
	{
		if (words[i] == word)
		{
			return true;
		}
		else if (words[i] == "\n")
		{
			continue;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool isStr(string word)
{
	return word[0] == '`' && word.back() == '`';
}

bool isArtScopes(stack<string> scope, int i)
{
	while (scope.size() > i)
	{
		if (scope.top()[0] != '$')
		{
			return false;
		}

		scope.pop();
	}

	return true;
}