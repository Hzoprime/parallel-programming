#pragma comment(linker, "/STACK:536870912,536870912")
#include"multiply.h"

#include"serial.h"
#include"cache.h"
#include"sse_float.h"
#include"sse_tile.h"

#include"timer.h"




int main()
{
	int times = 40;
	float a[max_n][max_n];
	float b[max_n][max_n];
	float serial_result[max_n][max_n];
	float cache_result[max_n][max_n];
	float sse_result[max_n][max_n];
	float sse_tile_result[max_n][max_n];

	init_zero_mat(max_n, serial_result);
	init_zero_mat(max_n, cache_result);
	init_zero_mat(max_n, sse_result);
	init_zero_mat(max_n, sse_tile_result);

	serial* serial_multiply = new serial();
	timer* serial_timer = new timer(serial_multiply);

	cache* cache_multiply = new cache();
	timer* cache_timer = new timer(cache_multiply);

	sse_float* sse_multiply = new sse_float();
	timer* sse_timer = new timer(sse_multiply);

	sse_tile* sse_tile_multiply = new sse_tile();
	timer* sse_tile_timer = new timer(sse_tile_multiply);

	for (int n = 1 << 6; n <= max_n; n +=10)
	{
		for (int i = 0; i < times; i++)
		{
			init_rand_mat(max_n, a);
			init_rand_mat(max_n, b);

			serial_timer->run(n, a, b, serial_result);
			cache_timer->run(n, a, b, cache_result);
			sse_timer->run(n, a, b, sse_result);
			sse_tile_timer->run(n, a, b, sse_tile_result);

			//output(n, sse_tile_result);
			cout << compare(n, sse_result, sse_tile_result) << endl;
			system("pause");
		}
		//break;
	}
}

