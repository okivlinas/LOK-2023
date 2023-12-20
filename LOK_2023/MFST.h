#pragma once
#include "stdafx.h"
#include "GRB.h"
typedef std::stack<short> MFSTSTSTACK;		///стек автомата

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3
#define MFST_TRACE_START	fout << std::setw(4) << std::left << "Шаг" << ": " \
									  << std::setw(20) << std::left << "Правило" \
									  << std::setw(30) << std::left << "Входная лента" \
									  << std::setw(20) << std::left << "Стек" \
									  << std::endl;

#define MFST_TRACE1		fout << std::setw(4) << std::left << ++FST_TRACE_n << ": " \
								  << std::setw(20) << std::left << rule.getCRule(rbuf, nrulechain) \
								  << std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
								  << std::setw(20) << std::left << getCSt(sbuf) \
								  << std::endl;

#define MFST_TRACE2		fout << std::setw(4) << std::left << FST_TRACE_n<<": " \
								  << std::setw(20) << std::left << " " \
								  << std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
								  << std::setw(20) << std::left << getCSt(sbuf) \
								  << std::endl;

#define MFST_TRACE3		fout << std::setw(4) << std::left << ++FST_TRACE_n <<": " \
								  << std::setw(20) << std::left << " " \
								  << std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
								  << std::setw(20) << std::left << getCSt(sbuf) \
								  << std::endl;

#define MFST_TRACE4(c)		fout << std::setw(4) << std::left << ++FST_TRACE_n << ": " << std::setw(20) << std::left << c <<std::endl; 
#define MFST_TRACE5(c)		fout << std::setw(4) << std::left << FST_TRACE_n << ": " << std::setw(20) << std::left << c << std::endl; 
#define MFST_TRACE6(c, k)	fout << std::setw(4) << std::left << FST_TRACE_n << ": " << std::setw(20) << std::left << c << k << std::endl; 
#define MFST_TRACE7			fout << std::setw(4) << std::left << state.lenta_position << ": " \
								  << std::setw(20) << std::left << rule.getCRule(rbuf, state.nrulechain) \
								  << std::endl;

namespace MFST
{
	struct MfstState			//Состояние автомата (для сохранения)
	{
		short lenta_position;	///позиция на ленте
		short nrule;
		short nrulechain;		///№ тек. цепочки, тек. правила
		MFSTSTSTACK st;			///стек автомата
		MfstState();
		MfstState(
			short pposition,	///позиция на ленте
			MFSTSTSTACK pst,	///стек автомата
			short pnrulechain	///№ текущей цепочки, тек. правила
		);
		MfstState(
			short pposition,
			MFSTSTSTACK pst,
			short pnrule,
			short pnrulechain
		);
	};

	struct Mfst					//Магазинный автомат
	{
		enum RC_STEP {			///код возврата ф-и step
			NS_OK,				///0 (найдено правило и цепочка, цеп. записана в стек)
			NS_NORULE,			///1 (не найдено правило (ошибка в грамматике)) 
			NS_NORULECHAIN,		///2 (не найдена подходящая цепочка правила (ошибка в исх. коде)
			NS_ERROR,			///3 (неизвестный нетерминал)
			TS_OK,				///4 (тек. символ ленты == вершине стека, продвинулась лента, pop стека)
			TS_NOK,				///5 (тек. символ ленты != вершине стека, восстановлено состояние)
			LENTA_END,			///6 (тек. позиция ленты >= lenta_size)
			SURPRISE
		};			///7 (неожиданный код возврата (ошибка в step)

		struct MfstDiagnosis		//Диагностика
		{
			short lenta_position;	///позиция на ленте
			RC_STEP rc_step;		///код завершения шага
			short nrule;			///номер правила
			short nrule_chain;		///номер цепочки правила
			MfstDiagnosis();
			MfstDiagnosis(
				short plenta_position,
				RC_STEP prc_step,
				short pnrule,
				short pnrule_chain);
		} diagnosis[MFST_DIAGN_NUMBER];	///последние самые глубокие сообщения

		GRBALPHABET* lenta;		///перекодированная (TS/NS) лента (из LEX)
		short lenta_position;	///тек. позиция на ленте
		short nrule;			///номер тек. правила
		short nrulechain;		///номер тек. цепочки, тек. правила
		short lenta_size;		///размер ленты
		GRB::Greibach grebach;	///граматика Грейбах
		LT::LexTable lex;		///рез. работы ЛА
		MFSTSTSTACK st;			///стек автомата
		std::stack<MfstState> storestate;	///стек для сохранения состояний
		Mfst();
		Mfst(
			LT::LexTable plex,
			GRB::Greibach pgrebach);

		char* getCSt		///получить содержимое стека
		(
			char* buf
		);
		char* getCLenta		///лента: n символов с pos
		(
			char* buf,
			short pos,
			short n = 25
		);
		char* getDiagnosis	///получ. n-ую строку диагностики или 0x00
		(
			short n,
			char* buf
		);

		bool savestate(ofstream& fout);	///сохранить состояние автомата

		bool reststate(ofstream& fout);	///восст. состояние автомата

		bool push_chain		///поместить цепочку правила в стек
		(
			GRB::Rule::Chain chain	///цепочка правила
		);

		RC_STEP step();		///выполнить шаг автомата

		bool start();		///запустить автомат

		bool savediagnosis
		(
			RC_STEP pprc_step	///код завершения шага
		);
		void printrules(ofstream& fout);	///вывести последовательность правил


		struct Deducation	///вывод
		{
			short size;				///кол-во шагов в выводе
			short* nrules;			///номера правил грамматики
			short* nrulechains;		///номера цепочек правил грамматики (nrules)

			Deducation() {
				size = 0;
				nrules = 0;
				nrulechains = 0;
			};
		} deducation;

		bool savededucation();		///сохранить дерево вывода
	};
}