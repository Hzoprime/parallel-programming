#pragma once
#include"multiply_matrix.h"


class sse_float_matrix :public multiply_matrix
{
public:
	static char* type;
	sse_float_matrix(string filename = "sse_float_matrix.txt") :multiply_matrix(filename)
	{

	}
	void record(const int& n, const long long& head, const long long& tail, const long long& frequency)
	{
		(*time)[to_string(n)].push_back((((double)(tail)) - ((double)(head))) / (double)(frequency) * 1000);
	}
	void run(const int& n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n])
	{
		__m128 t1, t2, sum;
		transpose(n, b);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				c[i][j] = 0.0;
				sum = _mm_setzero_ps();
				for (int k = n - 4; k >= 0; k -= 4)
				{
					t1 = _mm_loadu_ps(a[i] + k);
					t2 = _mm_loadu_ps(b[j] + k);
					t1 = _mm_mul_ps(t1, t2);
					sum = _mm_add_ps(sum, t1);
				}
				sum = _mm_hadd_ps(sum, sum);
				sum = _mm_hadd_ps(sum, sum);
				_mm_store_ss(c[i] + j, sum);
				for (int k = (n % 4) - 1; k >= 0; k--)
				{
					c[i][j] += a[i][k] * b[j][k];
				}
			}
		}
		transpose(n, b);
	}
};


char* sse_float_matrix::type = "sse_float";
