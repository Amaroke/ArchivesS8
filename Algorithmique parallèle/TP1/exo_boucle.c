#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num() 0
#define omp_get_wtime() 0
#endif

int main()
{
  int i;
  long cumul = 0;
  double deb, fin;

  deb = omp_get_wtime(); // Horloge début

#pragma omp parallel num_threads(4)
  {
    int num = omp_get_thread_num();

    printf("Processus %d\n", num);

#pragma omp for reduction(+ \
                          : cumul) // schedule(static, 2)
    for (i = 0; i < 1000000; i++)
    {
      // printf("Proc %d calcul itération %d\n", num, i);
      // #pragma omp atomic
      cumul++;
    }
  }

  fin = omp_get_wtime(); // Horloge finale

  printf("Cumul = %ld / %lfs\n", cumul, fin - deb);

  return (EXIT_SUCCESS);
}
