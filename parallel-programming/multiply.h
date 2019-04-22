#pragma once
#include"stdafx.h"

class multiply
{
	
public:
	ofstream* file;
	virtual void run(int n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n]) = 0;
	virtual char* get_type() = 0;
};