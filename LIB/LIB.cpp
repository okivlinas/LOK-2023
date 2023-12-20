// LIB.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"

int sum(int a, int b)
{
	int result = 0;

	__asm
	{
		push a;
		push b;
		pop eax;
		pop ebx;

		add eax, ebx;

		mov result, eax;
	}

	return result;
}

float sum(float a, float b)
{
	return a + b;
}

int minus(int a, int b)
{
	int result;

	__asm
	{
		push b;
		push a;
		pop eax;
		pop ebx;

		sub eax, ebx;

		mov result, eax;
	}

	return result;
}

float minus(float a, float b)
{
	return a - b;
}

int mult(int a, int b)
{
	int result;

	__asm
	{
		push b;
		push a;
		pop eax;
		pop ebx;

		imul eax, ebx;

		mov result, eax;
	}

	return result;
}

float mult(float a, float b)
{
	return a * b;
}

int division(int a, int b)
{
	int result;

	__asm
	{
		push b;
		push a;
		pop eax;
		pop ebx;

		push 0;
		pop edi;

		cmp b, edi;
		je isNull;

		idiv ebx;
		mov result, eax;
		jmp end;

	isNull:
		push INT_MAX;
		pop eax;

		mov result, eax;
		.
			end:

	}

	return result;
}

float division(float a, float b)
{
	return a / b;
}

void For(int start, int end, int step, std::function<bool(int)> foo)
{
	step = abs(step);
	bool isBreak;
	if (start < end)
	{
		for (int i = start; i <= end; i += step)
		{
			isBreak = foo(i);

			if (isBreak) break;
		}
	}
	else
	{
		for (int i = start; i >= end; i -= step)
		{
			isBreak = foo(i);

			if (isBreak) break;
		}
	}
}

void For(float start, float end, float step, std::function<bool(float)> foo)
{
	step = abs(step);
	bool isBreak;
	if (start < end)
	{
		for (float i = start; i <= end; i += step)
		{
			isBreak = foo(i);

			if (isBreak) break;
		}
	}
	else
	{
		for (float i = start; i >= end; i -= step)
		{
			foo(i); isBreak = foo(i);

			if (isBreak) break;
		}
	}
}

void For(char start, char end, int step, std::function<bool(char)> foo)
{
	step = abs(step);
	bool isBreak;

	if (start < end)
	{
		for (char i = start; i <= end; i += step)
		{
			isBreak = foo(i);

			if (isBreak) break;
		}
	}
	else
	{
		for (char i = start; i >= end; i -= step)
		{
			isBreak = foo(i);

			if (isBreak) break;
		}
	}
}