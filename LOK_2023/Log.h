#pragma once
#include "Error.h"
#include "stdafx.h"
#include "In.h"
#include "Parm.h"
#include "LT.h"
namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE];
		std::ofstream* stream;
	};

	static const LOG INITLOG{ L"log.txt", nullptr };
	LOG getlog(wchar_t logfile[]);
	void WriteLine(LOG log, char* c, ...);
	void WriteLine(LOG log, wchar_t* c, ...);
	void WriteLog(LOG log);
	void WriteParm(LOG, Parm::PARM parm);
	void WriteIn(LOG log, In::IN in);
	void WriteError(LOG log, Error::ERROR error);
	void WriteLT(LOG log, LT::LexTable lextable);
	void Close(LOG log);
}