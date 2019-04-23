#pragma once
#include"multiply.h"

class sse_tile :public multiply
{
	static char* type;
	int tile;
public:
	sse_tile(int tile_ = 64) :tile(tile_ - tile_ & 3)
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

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				c[i][j] = 0;
			}
		}

		for (int r = 0; r <= n / tile; r++)
		{
			for (int q = 0; q <= n / tile; q++)
			{
				for (int p = 0; p < n / tile; p++)
				{
					for (int i = 0; i < tile; i++)
					{
						if (r*tile + i >= n)
						{
							break;
						}
						for (int j = 0; j < tile; j++)
						{
							if (q*tile + j >= n)
							{
								break;
							}
							sum = _mm_setzero_ps();
							for (int k = tile - 4; k >= 0; k -= 4)
							{
								t1 = _mm_loadu_ps(a[r*tile + i] + p*tile + k);
								t2 = _mm_loadu_ps(b[q*tile + j] + p*tile + k);
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
				/*consider p = tile in the loop of r and q*/
				{
					int p = n / tile;
					for (int i = 0; i < tile; i++)
					{
						if (r*tile + i >= n)
						{
							break;
						}
						for (int j = 0; j < tile; j++)
						{
							if (q*tile + j >= n)
							{
								break;
							}
							sum = _mm_setzero_ps();

							for (int l = (n - p*tile) % 4 - 1; l >= 0; l--)
							{
								c[r*tile + i][q*tile + j] += a[r*tile + i][p*tile + l] * b[q*tile + j][p*tile + l];
							}

							for (int k = n - p*tile - 4; k >= 0; k -= 4)
							{
								t1 = _mm_loadu_ps(a[r*tile + i] + p*tile + k);
								t2 = _mm_loadu_ps(b[q*tile + j] + p*tile + k);
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
	~sse_tile()
	{
		file->close();
		delete file;
	}
};


char* sse_tile::type = "sse_tile";
