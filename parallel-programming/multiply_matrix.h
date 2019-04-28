#pragma once
#include"stdafx.h"

class multiply_matrix
{
	void check_file()
	{
		if (!file->is_open())
		{
			if (!file->is_open())
			{
				cout << "File is not open." << endl;
				system("pause");
			}
		}
	}

public:
	ofstream* file;
	map<string, vector<double>> *time;
	multiply_matrix(string filename)
	{
		file = new ofstream("./data/" + filename, ios::trunc);
		time = new map<string, vector<double>>();
		check_file();
	}
	void get_performance()
	{
		//sort(time->begin(), time->end());
		(*file).setf(ios::fixed);
		for (auto it = time->begin(); it != time->end(); it++)
		{
			(*file) << it->first << " " << accumulate(it->second.begin(), it->second.end(), 0.0) / it->second.size()  << endl;
		}
	}
	virtual void run(const int& n, float a[max_n][max_n], float b[max_n][max_n], float c[max_n][max_n]) = 0;
	virtual void record(const int& n, const long long& head, const long long& tail, const long long& frequency) = 0;
	virtual ~multiply_matrix()
	{
		file->close();
		delete file;
		delete time;
	}
};