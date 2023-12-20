#pragma once
#include "stdafx.h"

#define HEADER "#include <iostream>\n" <<\
			   "#include <windows.h>\n" <<\
			   "\n#pragma comment(lib, \"D:\\\\CP\\\\CPP\\\\LIB.lib\")\n"<<\
			   "#include \"D:\\\\CP\\\\LIB\\\\framework.h\"\n\n\n"


#define STARTMAIN "int main()\n" <<\
				  "{\n" <<\
				  "\tSetConsoleCP(1251);\n" <<\
				  "\tSetConsoleOutputCP(1251);\n\n"

#define ENDMAIN "\nreturn 0;\n" <<\
				"}\n"

namespace GEN
{
	bool Generate( // ��������� ��������� ����
		LT::LexTable& lextable, // ������� ������
		IT::IdTable& idtable, // ������� ���������������
		Parm::PARM& parm // ��������� ��������� ������
	);

	void changeDoth(char* str); // �������� ����� � ������

	void Run(Parm::PARM& parm); // ��������� ��������� � ������ ����� Cpp
}