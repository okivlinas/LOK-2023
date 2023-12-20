#include "Out.h"

namespace Out
{
	OUT::OUT(wchar_t* path_to_file)
	{
		this->path_to_file = path_to_file;
		file.open(path_to_file);
	}

	OUT::~OUT()
	{
		file.close();
	}

	void OUT::Write(char* str)
	{
		if (file.is_open())
		{
			file << str;
		}
	}
}