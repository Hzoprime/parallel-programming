#pragma once

#include"multiply_matrix.h"
class serial_matrix : public multiply_matrix
{
public:
	static char* type;
	serial_matrix(string filename = "serial_matrix.txt") :multiply_matrix(filename)
	{

	}
	void record(const int& n, const long long& head, const long long& tail, const long long& frequency)
	{
		//cout << ((double)(tail)) - ((double)(head)) << endl;
		(*time)[to_string(n)].push_back((((double)(tail)) - ((double)(head))) / (double)(frequency) * 1000);
	}

	void run(const int& n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n])
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

char* serial_matrix::type = "serial";
