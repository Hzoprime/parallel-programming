#pragma once
#include"multiply.h"

class sse_tile :public multiply
{
	static char* type;
	static int tile;
public:
	sse_tile()
	{
		file = new ofstream("sse_tile.txt", ios::app);
	}
	char* get_type()
	{
		return type;
	}
	void run(int n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n])
	{
		__m128 t1, t2, sum;
		float temp;
		transpose(n, b);

		for (int r = 0; r < n / tile; r++)
		{
			for (int q = 0; q < n / tile; q++)
			{
				for (int i = 0; i < tile; i++)
				{
					for (int j = 0; j < tile; j++)
					{
						c[r*tile + i][q*tile + j] = 0.0;
					}
				}
				for (int p = 0; p < n / tile; p++)
				{
					for (int i = 0; i < tile; i++)
					{
						for (int j = 0; j < tile; j++)
						{
							sum = _mm_setzero_ps();
							for (int k = 0; k < tile; k += 4)
							{
								t1 = _mm_loadu_ps(a[r*tile + i] + p*tile + k);
								t2 = _mm_loadu_ps(b[q*tile + i] + p*tile + k);
								t1 = _mm_mul_ps(t1, t2);
								sum = _mm_add_ps(sum, t1);
							}
							sum = _mm_hadd_ps(sum, sum);
							sum = _mm_hadd_ps(sum, sum);
							_mm_store_ss(&temp, sum);
							c[r*tile + i][q*tile + j] += temp;
						}
					}
				}
			}
		}
		transpose(n, b);
	}
};


char* sse_tile::type = "sse_tile";
int sse_tile::tile = 64;
