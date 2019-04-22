#pragma once
#include"multiply.h"

class cache :public multiply
{
public:
	static char* type;

	cache()
	{
		file = new ofstream("cache.txt", ios::app);
	}
	char* get_type()
	{
		return type;
	}
	void run(int n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n])
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
	~cache()
	{
		file->close();
		delete file;
	}
};


char* cache::type = "cache";
