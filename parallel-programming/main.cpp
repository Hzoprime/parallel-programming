#pragma comment(linker, "/STACK:536870912,536870912")
#include"multiply_matrix.h"

#include"serial_matrix.h"
#include"cache_matrix.h"
#include"sse_float_matrix.h"
#include"sse_tile_matrix.h"

#include"timer.h"


const int times = 20;
float a[max_n][max_n];
float b[max_n][max_n];
float result[max_n][max_n];

void test_size()
{
	init_zero_mat(max_n, result);

	vector<multiply_matrix*> multiply_vector;
	vector<timer*> timer_vector;

	multiply_vector.push_back(new serial_matrix);
	multiply_vector.push_back(new cache_matrix());
	multiply_vector.push_back(new sse_float_matrix());
	multiply_vector.push_back(new sse_tile_matrix());

	for (int i = 0; i < multiply_vector.size(); i++)
	{
		timer_vector.push_back(new timer(multiply_vector[i]));
	}

	int increase = 4;

	for (int n = 1 << 6; n <= max_n; n += increase)
	{
		for (int i = 0; i < times; i++)
		{
			init_rand_mat(max_n, a);
			init_rand_mat(max_n, b);
			for (int i = 0; i < timer_vector.size(); i++)
			{
				timer_vector[i]->run(n, a, b, result);
			}
		}
		if (n >= (increase) * 16)
		{
			increase *= 2;
		}
		printf("n = %d\n", n);
	}

	for (int i = 0; i < timer_vector.size(); i++)
	{
		timer_vector[i]->p_multiply->get_performance();
	}

	for (int i = 0; i < timer_vector.size(); i++)
	{
		delete timer_vector[i];
		delete multiply_vector[i];
	}
	timer_vector.clear();
	multiply_vector.clear();
}

void test_tile()
{
	sse_tile_matrix* sse_tile = new sse_tile_matrix("tile.txt");
	timer* tile_timer = new timer(sse_tile);

	for (int tile = 1 << 4; tile <= max_n / 2; tile += 16)
	{
		sse_tile->tile = tile;
		for (int i = 0; i < times; i++)
		{
			init_rand_mat(max_n, a);
			init_rand_mat(max_n, b);

			tile_timer->run(max_n, a, b, result);
		}
		printf("tile = %d\n", tile);
	}

	sse_tile->get_performance();
	delete sse_tile;
	delete tile_timer;
	sse_tile = nullptr;
	tile_timer = nullptr;
}

int main()
{
	cout.setf(ios::fixed);
	//test_size(); 
	test_tile();
}

