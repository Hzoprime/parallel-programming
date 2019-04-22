#pragma once
#include <immintrin.h>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <windows.h>
#include <fstream>
const int max_n = 1 << 10;
const int mod = 1 << 6;
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




template<typename T>
void init_rand_mat(int n, T a[max_n][max_n])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			a[i][j] = (float(rand() % mod)) / (float(rand() % mod));
		}
	}
}

template<typename T>
void init_zero_mat(int n, T a[max_n][max_n])
{
	memset(a, 0, sizeof(T)*(max_n)*(max_n));
}