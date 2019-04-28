#pragma once
#include"multiply_matrix.h"

class timer
{
public:
	multiply_matrix *p_multiply;
	timer(multiply_matrix *p)
	{
		p_multiply = p;
	}
	void run(int n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n])
	{
		LARGE_INTEGER head, tail, frequency;
		QueryPerformanceFrequency(&frequency);

		QueryPerformanceCounter(&head);

		p_multiply->run(n, a, b, c);

		QueryPerformanceCounter(&tail);
		p_multiply->record(n, head.QuadPart, tail.QuadPart, frequency.QuadPart);
	}
};