#pragma once
#include"multiply_matrix.h"

class cache_matrix :public multiply_matrix
{
public:
	static char* type;
	cache_matrix(string filename = "cache_matrix.txt") :multiply_matrix(filename)
	{
	}
	void record(const int& n, const long long& head, const long long& tail, const long long& frequency)
	{
		(*time)[to_string(n)].push_back((((double)(tail)) - ((double)(head))) / (double)(frequency) * 1000);
	}

	void run(const int& n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n])
	{
		transpose(n, b);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				c[i][j] = 0.0;
				for (int k = 0; k < n; k++)
				{
					c[i][j] += a[i][k] * b[j][k];
				}
			}
		}
		transpose(n, b);
	}
};


char* cache_matrix::type = "cache";
