#include <stdio.h>
#include <omp.h>
#include <time.h>

long long num_steps = 100000000;
double step;

int main(int argc, char* argv[])
{
	clock_t start, stop;

	double pi, sum = 0.0,tab[80];

	int i;

	for (int j = 0; j < 30; j++)
	{
		step = 1. / (double)num_steps;

		start = clock();
		omp_set_num_threads(2);

#pragma omp parallel
		{
			double x;
			int id = omp_get_thread_num();
			tab[j+id] = 0;
			sum = 0;

#pragma omp for
			for (i = 0; i < num_steps; i++)
			{

				//int tid = omp_get_thread_num();
				//printf("Hello world from omp thread %d\n", tid);

				x = (i + .5)*step;

#pragma omp flush (tab)
				tab[j+id] += 4.0 / (double)(1. + x * x);
			}
#pragma omp atomic
			sum += tab[j+id];
		}
		pi = sum * step;
		stop = clock();

		//printf("Wartosc liczby PI wynosi %15.12f\n", pi);
		printf("Czas rzeczywisty przetwarzania iteracji %d to: %f sekund\n", j,((double)(stop - start) / CLOCKS_PER_SEC));
	}
	return 0;
}