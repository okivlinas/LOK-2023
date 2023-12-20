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
	bool Generate( // запустить генерацию кода
		LT::LexTable& lextable, // таблица лексем
		IT::IdTable& idtable, // таблица идентификаторов
		Parm::PARM& parm // параметры командной строки
	);

	void changeDoth(char* str); // заменить точки в строке

	void Run(Parm::PARM& parm); // запустить компил€цю и сборку файла Cpp
}