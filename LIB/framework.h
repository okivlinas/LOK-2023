#pragma once

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
#include <functional>

int sum(int a, int b);
float sum(float a, float b);

int minus(int a, int b);
float minus(float a, float b);

int mult(int a, int b);
float mult(float a, float b);

int division(int a, int b);
float division(float a, float b);

void For(int, int, int, std::function<bool(int)> foo);

void For(char, char, int, std::function<bool(char)> foo);

void For(float start, float end, float step, std::function<bool(float)> foo);