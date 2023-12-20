#include "stdafx.h"
#include "MFST.h"
#include "Gen.h"

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");

	Log::LOG log = Log::INITLOG; // ������������� ����
	Parm::PARM parm; // ������������� ����������
	try
	{
		parm = Parm::getparm(argc, argv); // ��������� ����������
		log = Log::getlog(parm.log); // ��������� ����
		auto in = In::getin(parm.in); // ��������� ������� ������

		LT::LexTable lextable = LT::Create(4000); // �������� ������� ������
		IT::IdTable idtable = IT::Create(4000); // �������� ������� ���������������

		LexAnalize(in, lextable, idtable);

		Log::WriteLog(log); // ������ � ���

		Log::WriteIn(log, in); // ������ �������� ��������� ������� ������
		Log::WriteParm(log, parm); // ������ �������� �������� ����������

		Log::WriteLT(log, lextable); // ������ ������� ������
		Log::WriteLine(log, (char*)"������� ������:						LT.txt\n", (char*)"");
		Log::WriteLine(log, (char*)"������� ���������������:			IT.txt\n", (char*)"");
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
		Log::WriteLine(log, (char*)"��������� ��������������� �������:  SNT.txt\n", (char*)"");

		checkSemantic(lextable, idtable);

		GEN::Generate(lextable, idtable, parm);

		GEN::Run(parm);

		Log::WriteLine(log, (char*)"--------- ���������� � JavaScript ������ ������� ---------\n", (char*)"");

		Log::Close(log); // �������� ����
	}
	catch (Error::ERROR e)
	{
		std::cout << "������: " << e.id << " : " << e.message << std::endl; // ����� ������
		if (e.inext.line != -1)
		{
			std::cout << "������: " << e.inext.line;
			if (e.inext.col != -1)
			{
				cout << "  ������: " << e.inext.col << std::endl;
			}
		}
		Log::WriteError(log, e); // ������ ������ � ���
		Log::WriteLine(log, (char*)"\n--------- ���������� � JavaScript ������ ��������� ---------\n", (char*)"");
		Log::Close(log); // �������� ����
	}
	return 0;
}