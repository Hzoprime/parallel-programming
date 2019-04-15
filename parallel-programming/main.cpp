#pragma comment(linker, "/STACK:536870912,536870912")
#include <immintrin.h>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <windows.h>
const int max_n = 1024;
using namespace std;

void output(int n, float a[max_n][max_n])
{

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << a[i][j] << '\t';
		}
		cout << endl;
	}
}

bool compare(int n, float a[max_n][max_n], float b[max_n][max_n])
{
	unsigned long long int count = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (abs(a[i][j] - b[i][j]) > 1e-1)
			{
				printf("i: %d, j:%d\na[i][j] = %lf\tb[i][j] = %lf\n", i, j, a[i][j], b[i][j]);
				count++;
			}
		}
	}
	return !count;
}

void transpose(int n, float b[max_n][max_n])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			swap(b[i][j], b[j][i]);
		}
	}

}



class multiply
{
public:
	virtual void run(int n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n]) = 0;
	virtual char* get_type() = 0;
};

template<typename T>
class serial : public multiply
{
	static char *type;
public:
	char* get_type()
	{
		return type;
	}
	void run(int n, T a[max_n][max_n], T b[max_n][max_n], T c[max_n][max_n])
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

template<typename T>
class cache :public multiply
{
	static char *type;
public:
	char* get_type()
	{
		return type;
	}
	void run(int n, T a[max_n][max_n], T b[max_n][max_n], T c[max_n][max_n])
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


class sse_float :public multiply
{
	static char* type;
public:
	char* get_type()
	{
		return type;
	}
	void run(int n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n])
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

class sse_tile :public multiply
{
	static char* type;
	static int tile;
public:
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


template<typename T>
char* serial<T>::type = "serial";
template<typename T>
char* cache<T>::type = "cache";
char* sse_float::type = "sse_float";
char* sse_tile::type = "sse_tile";
int sse_tile::tile = 64;


class timer
{
public:
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
		cout << p_multiply->get_type() << ": " << ((float)(tail.QuadPart - head.QuadPart)) / frequency.QuadPart << endl;
	}
};



int main()
{
	float a[max_n][max_n];
	float b[max_n][max_n];
	float serial_result[max_n][max_n];
	float cache_result[max_n][max_n];
	float sse_result[max_n][max_n];
	float sse_tile_result[max_n][max_n];
	int mod = 1 << 6;
	for (int i = 0; i < max_n; i++)
	{
		for (int j = 0; j < max_n; j++)
		{
			a[i][j] = (float(rand() % mod)) / (float(rand() % mod));
			b[i][j] = (float(rand() % mod)) / (float(rand() % mod));
			serial_result[i][j] = 0;
			cache_result[i][j] = 0;
			sse_result[i][j] = 0;
			sse_tile_result[i][j] = 0;
		}
	}
	int n = max_n;

	serial<float>* serial_multiply = new serial<float>();
	timer* serial_timer = new timer(serial_multiply);
	//serial_timer->run(n, a, b, serial_result);



	cache<float>* cache_multiply = new cache<float>();
	timer* cache_timer = new timer(cache_multiply);
	cache_timer->run(n, a, b, cache_result);



	sse_float* sse_multiply = new sse_float();
	timer* sse_timer = new timer(sse_multiply);
	sse_timer->run(n, a, b, sse_result);

	sse_tile* sse_tile_multiply = new sse_tile();
	timer* sse_tile_timer = new timer(sse_tile_multiply);
	sse_tile_timer->run(n, a, b, sse_tile_result);

	//cout << compare(n, cache_result, serial_result) << endl;
	cout << compare(n, sse_tile_result, cache_result) << endl;


	// __m128 sum;
	// sum = _mm_setzero_ps();
	system("pause");
}

