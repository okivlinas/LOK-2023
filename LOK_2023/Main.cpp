#include "stdafx.h"
#include "MFST.h"
#include "Gen.h"

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");

	Log::LOG log = Log::INITLOG; // инициализация лога
	Parm::PARM parm; // инициализация параметров
	try
	{
		parm = Parm::getparm(argc, argv); // получение параметров
		log = Log::getlog(parm.log); // получение лога
		auto in = In::getin(parm.in); // получение входных данных

		LT::LexTable lextable = LT::Create(4000); // создание таблицы лексем
		IT::IdTable idtable = IT::Create(4000); // создание таблицы идентификаторов

		LexAnalize(in, lextable, idtable);

		Log::WriteLog(log); // запись в лог

		Log::WriteIn(log, in); // запись процесса обработки входных данных
		Log::WriteParm(log, parm); // запись процесса проверки параметров

		Log::WriteLT(log, lextable); // запись таблицы лексем
		Log::WriteLine(log, (char*)"Таблица лексем:						LT.txt\n", (char*)"");
		Log::WriteLine(log, (char*)"Таблица идентификаторов:			IT.txt\n", (char*)"");
		LT::Output(lextable);
		IT::Output(idtable);

		ofstream fout("SNT.txt");

		MFST_TRACE_START

			fout.close();
		MFST::Mfst mfst(lextable, GRB::getGreibach());

		bool resultSint = mfst.start();
		mfst.savededucation();

		fout.open("SNT.txt", ios::app);
		mfst.printrules(fout);
		fout.close();


		if (!resultSint)
		{
			throw ERROR_THROW(121);
		}
		Log::WriteLine(log, (char*)"Результат синтаксического анализа:  SNT.txt\n", (char*)"");

		checkSemantic(lextable, idtable);

		GEN::Generate(lextable, idtable, parm);

		GEN::Run(parm);

		Log::WriteLine(log, (char*)"--------- Трансляция в JavaScript прошла успешно ---------\n", (char*)"");

		Log::Close(log); // закрытие лога
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка: " << e.id << " : " << e.message << std::endl; // вывод ошибки
		if (e.inext.line != -1)
		{
			std::cout << "Строка: " << e.inext.line;
			if (e.inext.col != -1)
			{
				cout << "  Символ: " << e.inext.col << std::endl;
			}
		}
		Log::WriteError(log, e); // запись ошибки в лог
		Log::WriteLine(log, (char*)"\n--------- Трансляция в JavaScript прошла неуспешно ---------\n", (char*)"");
		Log::Close(log); // закрытие лога
	}
	return 0;
}