#include "Gen.h"
#define SKIP 'z'
namespace GEN
{
	bool Generate(
		LT::LexTable& lextable,
		IT::IdTable& idtable,
		Parm::PARM& parm
	)
	{
		ofstream fout(parm.out);
		IT::Entry* edi;
		IT::Entry* tedi;
		IT::Entry* t2edi;
		int size = 0;
		const int MAX_SIZE = 100;
		char* expr = new char[MAX_SIZE];
		stack<string> tabs;
		auto t = tabs;
		bool isEndOfIf = false;

		int countScopes = 0;
		int countParams = 0;
		bool needAdd = false;

		int countScopesForConsole = 0;
		bool isConsole = false;

		bool isFor = false;
		char stateFor;
		short countParamsGone = 0;
		const short maxChanged = 2;

		stack<bool> isCalledFoo;
		isCalledFoo.push(false);
		stack<int> cntScopesCF;
		cntScopesCF.push(0);
		stack<int> numberOfP;
		stack<vector<string>> paramsCF;

		stack<IT::IDDATATYPE> typesOfFunc;
		stack<int> countScopesInit;
		int cntScopesTypes = 0;

		bool isReturn = false;

		tabs.push("");

		fout << HEADER;

		for (int i = 0; i < lextable.size; i++)
		{
			switch (lextable.table[i].lexema)
			{
			case 'i':
			{
				edi = new IT::Entry();
				*edi = IT::GetEntry(idtable, lextable.table[i].idxTI);

				if (edi->idtype == IT::F && lextable.table[i + 1].lexema == 's')
				{
					typesOfFunc.push(edi->iddatatype);
					countScopesInit.push(cntScopesTypes);
				}

				delete edi;

				if (
					i + 2 < lextable.size &&
					lextable.table[i + 1].lexema == 's' &&
					(lextable.table[i + 2].lexema == 't' || lextable.table[i + 2].lexema == 'f'))
				{
					edi = new IT::Entry();

					*edi = IT::GetEntry(idtable, lextable.table[i].idxTI);
					changeDoth(edi->id);
					if (tabs.size() > 1 && lextable.table[i + 2].lexema == 'f')
					{
						fout << "auto " << edi->id << " = [&]";
					}
					else if (edi->iddatatype == IT::NUM)
					{
						fout << "int" << (edi->isRef ? "& " : " ") << edi->id;
					}
					else if (edi->iddatatype == IT::SYMB)
					{
						fout << "char" << (edi->isRef ? "& " : " ") << edi->id;
					}
					else if (edi->iddatatype == IT::FLT)
					{
						fout << "float" << (edi->isRef ? "& " : " ") << edi->id;
					}
					else if (edi->iddatatype == IT::ACTION)
					{
						fout << "void " << edi->id;
					}

					i += 2;

					delete edi;
				}

				else if (
					i + 1 < lextable.size &&
					lextable.table[i + 1].lexema == '{')
				{
					edi = new IT::Entry();

					*edi = IT::GetEntry(idtable, lextable.table[i].idxTI);
					changeDoth(edi->id);

					fout << "[&](" << (edi->iddatatype == IT::NUM ? "int " : edi->iddatatype == IT::FLT ? "float " : "char ") << edi->id << ")";

					delete edi;
				}

				else
				{
					edi = new IT::Entry();

					*edi = IT::GetEntry(idtable, lextable.table[i].idxTI);
					changeDoth(edi->id);

					countParams = 0;
					int j = i + 1;
					int countScopes = 0;

					if (edi->idtype == IT::F)
					{

						if (lextable.table[j].lexema == '(')
						{
							do
							{
								if (
									lextable.table[j].lexema != ')' &&
									lextable.table[j].lexema != '(' &&
									lextable.table[j].lexema != ',' &&
									countScopes == 1)
								{
									countParams++;
								}
								if (lextable.table[j].lexema == '(')
								{
									countScopes++;
								}
								if (lextable.table[j].lexema == ')')
								{
									countScopes--;
								}
								j++;
							} while (countScopes != 0);

							if (countParams != edi->countParams)
							{
								throw ERROR_THROW(610);
							}
						}
					}

					if (isFor)
					{
						switch (stateFor)
						{
						case 'n':
							fout << "(int)";
							break;
						case 's':
							fout << "(char)";
							break;
						case 'f':
							fout << "(float)";
							break;
						}

						countParamsGone++;

						if (countParamsGone == maxChanged)
						{
							isFor = false;
						}
					}

					if (isCalledFoo.top() && cntScopesCF.top() == 1)
					{
						if (paramsCF.top()[numberOfP.top()].back() == '&' && idtable.table[lextable.table[i].idxTI].idtype == IT::F)
						{
							throw ERROR_THROW_IN(619, lextable.table[i].sn, -1);
						}
						fout << "(" << paramsCF.top()[numberOfP.top()] << ")";
						short tmp = numberOfP.top() + 1;
						numberOfP.pop();
						numberOfP.push(tmp);
					}

					if (edi->idtype == IT::F)
					{
						if (!edi->isFromStatic)
						{
							isCalledFoo.push(true);
							cntScopesCF.push(0);
							numberOfP.push(0);
							paramsCF.push(edi->params);
						}
						else
						{
							isCalledFoo.push(true);
							cntScopesCF.push(0);
							numberOfP.push(0);

							tedi = new IT::Entry();

							*tedi = IT::GetEntry(idtable, IT::GetIndexByLTIndex(idtable, i + 2));

							vector<string> v;

							if (tedi->iddatatype == IT::NUM)
							{
								v.push_back("int");
								v.push_back("int");
							}
							else if (tedi->iddatatype == IT::FLT)
							{
								v.push_back("float");
								v.push_back("float");
							}
							else if (tedi->iddatatype == IT::SYMB)
							{
								v.push_back("char");
								v.push_back("char");
							}

							paramsCF.push(v);

							delete tedi;
						}
					}

					fout << (edi->needToInt ? "(int)" : "") << edi->id;

					delete edi;
				}
				break;
			}

			case 'l':
			{
				edi = new IT::Entry();

				*edi = IT::GetEntry(idtable, lextable.table[i].idxTI);

				if (isFor)
				{
					switch (stateFor)
					{
					case 'n':
						fout << "(int)";
						break;
					case 's':
						fout << "(char)";
						break;
					case 'f':
						fout << "(float)";
						break;
					}

					countParamsGone++;

					if (countParamsGone == maxChanged)
					{
						isFor = false;
					}
				}

				if (isCalledFoo.top() && cntScopesCF.top() == 1)
				{
					if (paramsCF.top()[numberOfP.top()].back() == '&')
					{
						throw ERROR_THROW_IN(619, lextable.table[i].sn, -1);
					}

					fout << "(" << paramsCF.top()[numberOfP.top()] << ")";
					short tmp = numberOfP.top() + 1;
					numberOfP.pop();
					numberOfP.push(tmp);
				}

				if (edi->iddatatype == IT::NUM)
				{
					fout << edi->value.vint;
				}
				else if (edi->iddatatype == IT::FLT)
				{
					fout << (edi->needToInt ? "(int)" : "") << edi->value.vflt/* << "f"*/;
				}
				else if (edi->iddatatype == IT::STR)
				{
					if (!isConsole)
					{
						throw ERROR_THROW_IN(623, lextable.table[i].sn, -1);
					}
					fout << '"' << edi->value.vstr << '"';
				}
				else
				{
					char symb = edi->value.vsymb;
					string w = "";

					switch (symb)
					{
					case '\n':
						w = "\\n";
						break;
					case '\t':
						w = "\\t";
						break;
					case '\'':
						w = "\\'";
						break;
					case '\\':
						w = "\\\\";
						break;
					default:
						w = symb;
					}

					fout << '\'' << w << '\'';
				}

				delete edi;
				break;
			}

			case SKIP:
			{
				fout << "return false;\n";
				break;
			}

			case 'b':
			{
				fout << "return true";
				break;
			}

			case 'r':
			{
				fout << "return ";
				isReturn = true;
				if (typesOfFunc.size() > 0 && typesOfFunc.top() != IT::ACTION)
				{
					if (typesOfFunc.top() == IT::NUM)
					{
						fout << "(int)(";
					}
					else if (typesOfFunc.top() == IT::FLT)
					{
						fout << "(float)(";
					}
					else if (typesOfFunc.top() == IT::SYMB)
					{
						fout << "(char)(";
					}
				}

				break;
			}

			case FOR:
			{
				fout << "For";

				isFor = true;
				stateFor = lextable.table[i].view;
				countParamsGone = 0;

				break;
			}

			case '=':
			{
				size = 0;

				fout << " = ";


				break;
			}

			case 'm':
			{
				fout << STARTMAIN;

				typesOfFunc.push(IT::NUM);
				countScopesInit.push(cntScopesTypes);
				cntScopesTypes = 1;

				tabs.push("\t");
				t = tabs;

				while (!t.empty())
				{
					fout << t.top();
					t.pop();
				}

				i++;
				break;
			}

			case 'v':
			{
				fout << " " << lextable.table[i].view << " ";
				break;
			}

			case 'V':
			{
				if (lextable.table[i].view == '=')
				{
					fout << " == ";
				}
				else if (lextable.table[i].view == '!')
				{
					fout << " != ";
				}
				else if (lextable.table[i].view != '=')
				{
					fout << " " << lextable.table[i].view << " ";
				}
				break;
			}

			case '{': case '}':
			{
				fout << "\n";

				t = tabs;

				if (lextable.table[i].lexema == '}')
				{
					t.pop();
					cntScopesTypes--;

					if (cntScopesTypes == countScopesInit.top())
					{
						countScopesInit.pop();
						typesOfFunc.pop();
					}
				}

				while (!t.empty())
				{
					fout << t.top();
					t.pop();
				}

				if (lextable.table[i].lexema == '{')
				{
					fout << lextable.table[i].lexema << (i + 1 < lextable.size && (lextable.table[i + 1].lexema != ';' && lextable.table[i + 1].lexema != '}' && lextable.table[i + 1].lexema != '{') ? "\n" : "");
					tabs.push("\t");
					t = tabs;

					cntScopesTypes++;

					while (!t.empty())
					{
						fout << t.top();
						t.pop();
					}
				}
				else
				{
					if (i + 1 < lextable.size && lextable.table[i + 1].lexema == ')')
					{
						fout << "\n";


						t = tabs;

						while (!t.empty())
						{
							fout << t.top();
							t.pop();
						}

						fout << "return false;\n";

						tabs.pop();

						t = tabs;

						while (!t.empty())
						{
							fout << t.top();
							t.pop();
						}

						fout << "}";

						if (i + 1 < lextable.size &&
							(lextable.table[i + 1].lexema != ';' && lextable.table[i + 1].lexema != ')'))
						{
							fout << "\n";
							t = tabs;

							while (!t.empty())
							{
								fout << t.top();
								t.pop();
							}
						}
					}
					else
					{
						tabs.pop();

						fout << lextable.table[i].lexema << (i + 1 < lextable.size && (lextable.table[i + 1].lexema != ';' && lextable.table[i + 1].lexema != '}' && lextable.table[i + 1].lexema != '{') ? "\n" : "");
						t = tabs;
						if (lextable.size > i + 1 && lextable.table[i + 1].lexema != ';')
						{
							while (!t.empty())
							{
								fout << t.top();
								t.pop();
							}
						}
					}
				}

				break;
			}

			case '(':
			{
				if (lextable.table[i].view == 'i')
					fout << "(int)";
				if (isCalledFoo.top())
				{
					short tmp = cntScopesCF.top() + 1;
					cntScopesCF.pop();
					cntScopesCF.push(tmp);
				}
				if (isConsole && lextable.table[i - 1].lexema != 'i')
				{
					countScopesForConsole++;
				}
				else
				{
					if (isConsole) countScopesForConsole++;
					if (needAdd)
					{
						countScopes++;
					}
					size = 0;
					isEndOfIf = false;
					if (lextable.table[i - 1].lexema != 'i')
					{
						for (int j = i; lextable.table[j].lexema != '?'; j++)
						{
							if (
								lextable.table[j].lexema == ';' ||
								lextable.table[j].lexema == '{' ||
								lextable.table[j].lexema == '}')
							{
								isEndOfIf = true;
								break;
							}
							size++;
						}
					}
					else
					{
						fout << "(";
						break;
					}

					if (isEndOfIf)
					{
						fout << "(";
						break;
					}

					fout << "if (";

					if (lextable.table[i + size + 1].lexema != 'T')
					{
						needAdd = true;
						countScopes++;
					}

				}
				break;
			}

			case 'T':
			{
				break;
			}

			case 'L':
			{
				fout << "else";
				t = tabs;

				while (!t.empty())
				{
					fout << t.top();
					t.pop();
				}
				break;
			}

			case ')':
			{
				if (isCalledFoo.top())
				{
					short tmp = cntScopesCF.top() - 1;
					cntScopesCF.pop();
					cntScopesCF.push(tmp);

					if (cntScopesCF.top() == 0)
					{
						isCalledFoo.pop();
						numberOfP.pop();
						cntScopesCF.pop();
						paramsCF.pop();
					}
				}
				if (isConsole && lextable.table[i + 1].lexema != ',' && lextable.table[i + 1].lexema != ')')
				{
					countScopesForConsole--;

					if (countScopesForConsole == 0)
					{
						isConsole = false;
					}
				}
				else
				{
					if (isConsole) countScopesForConsole--;

					fout << ")";
					if (needAdd)
					{
						countScopes--;

						if (countScopes == 0)
						{
							fout << "\t{}\n";
							t = tabs;

							while (!t.empty())
							{
								fout << t.top();
								t.pop();
							}
							needAdd = false;
						}


					}

					if (lextable.table[i - 1].lexema == 't')
					{
						i += 2;
					}
				}

				break;
			}

			case ';':
			{
				fout << (isReturn ? ")" : "") << "; " << (i + 1 < lextable.size && lextable.table[i].lexema != '}' ? "\n" : "");
				isReturn = false;
				t = tabs;

				while (!t.empty())
				{
					fout << t.top();
					t.pop();
				}

				break;
			}

			case '~':
			{
				fout << "~";
				break;
			}

			case ',':
			{
				fout << (isConsole && countScopesForConsole == 1 ? " << " : ", ");
				break;
			}

			case 'c':
			{
				if (lextable.table[i + 1].lexema == '(' && lextable.table[i + 2].lexema == ')')
				{
					fout << "std::cout << '\\n'";
					i += 2;
				}
				else
				{
					fout << "std::cout << ";
					countScopesForConsole = 0;
					isConsole = true;
				}
				break;
			}
			}
		}

		delete[] expr;
		fout.close();

		return true;
	}

	void Run(Parm::PARM& parm)
	{
		/*char* name = new char[300];
		string command =

		wcstombs_s(NULL, name, 300, parm.out, 300);

		delete[] name;*/
	}


	void changeDoth(char* str)
	{
		for (int i = 0; i < strlen(str); i++)
		{
			if (str[i] == '.')
			{
				str[i] = '_';
			}
		}
	}
}