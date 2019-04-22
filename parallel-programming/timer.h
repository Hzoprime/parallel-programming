#pragma once
#include"multiply.h"

class timer: public multiply
{
public:
	static char* type;
	multiply *p_multiply;
	timer(multiply *p)
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

		cout << p_multiply->get_type() << "    n = " << n << ": " << ((tail.QuadPart - head.QuadPart)) << endl;
		(*(p_multiply->file)) << p_multiply->get_type() << "    n = " << n << ": " << ((tail.QuadPart - head.QuadPart)) << endl;
	}
	char* get_type()
	{
		return type;
	}
};

char* timer::type = "timer";