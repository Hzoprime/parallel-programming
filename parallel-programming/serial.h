#pragma once

#include"multiply.h"
class serial : public multiply
{
public:
	static char* type;

	serial()
	{
		file = new ofstream("serial.txt", ios::app);
	}
	char* get_type()
	{
		return type;
	}
	void run(int n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n])
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				c[i][j] = 0.0;
				for (int k = 0; k < n; k++)
				{
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}
	}

};

char* serial::type = "serial";
