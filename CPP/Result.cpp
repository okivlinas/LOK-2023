#include <iostream>
#include <windows.h>

#pragma comment(lib, "D:\\CP\\CPP\\LIB.lib")
#include "D:\\CP\\LIB\\framework.h"


int fact_$(int a_fact_$)
{
	int res_fact_$ = 1; 
	For((int)1, (int)a_fact_$, 1, [&](int i_$For1_fact_$)
	{
		res_fact_$ = mult((int)res_fact_$, (int)i_$For1_fact_$); 
		
	
		return false;
	}); 
	return (int)(res_fact_$); 
	
}
void your_$(int n_your_$)
{
	if (n_your_$ == 1021)
	{
		std::cout << "�� ����� ����������� ����� " << n_your_$ << " � ����������������� �������!\n"; 
		
	}
	else	
	{
		std::cout << "�� ������� ����������� ����� " << n_your_$ << " � ����������������� �������!\n"; 
		
	}	
}
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int n_$l4_main_$ = 5; 
	int res_$l4_main_$ = fact_$((int)n_$l4_main_$); 
	std::cout << "��������� " << n_$l4_main_$ << " �����: " << res_$l4_main_$ << '\n'; 
	int n1_$l4_main_$ = 225; 
	your_$((int)n1_$l4_main_$); 
	int a1_$l4_main_$ = 5; 
	int a2_$l4_main_$ = 3; 
	int bit1_$l4_main_$ = a1_$l4_main_$ & a2_$l4_main_$; 
	int bit2_$l4_main_$ = a1_$l4_main_$ | a2_$l4_main_$ | a2_$l4_main_$; 
	int bit3_$l4_main_$ = ~a2_$l4_main_$; 
	std::cout << "a1 & a2: " << bit1_$l4_main_$ << '\n'; 
	std::cout << "a1 | a2: " << bit2_$l4_main_$ << '\n'; 
	std::cout << "~a2: " << bit3_$l4_main_$ << '\n'; 
	char c_$l4_main_$ = 'c'; 
	std::cout << "���������� ���� ������ ������������������ ����: " << c_$l4_main_$ << '\n'; 
	char s_$l4_main_$ = 'f'; 
	std::cout << "���������� ���� ������ ������������������ ������: " << s_$l4_main_$ << '\n'; 
	float f1_$l4_main_$ = 5.6; 
	auto round_$l4_main_$ = [&](float a_round_$l4_main_$)
	{
		int numa_round_$l4_main_$ = a_round_$l4_main_$; 
		if (minus((float)a_round_$l4_main_$, (float)numa_round_$l4_main_$) > 0.5)
		{
			return (float)(sum((int)numa_round_$l4_main_$, (int)1)); 
			
		}
		return (float)(numa_round_$l4_main_$); 
		
	}; 
	std::cout << "��������� ���������� f1: " << round_$l4_main_$((float)f1_$l4_main_$); 
	return (int)(0); 
	
}