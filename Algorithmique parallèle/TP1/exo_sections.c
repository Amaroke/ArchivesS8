#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

int main()
{
  #pragma omp parallel num_threads(3)
  {
    double deb, fin;
    int num = omp_get_thread_num();
    int nbT = omp_get_num_threads();

    deb = omp_get_wtime(); // Horloge début

    printf("Proc %d parmi %d dans section parallèle\n", num, nbT);
    sleep(2); // Simulation d'un calcul de 2s

    #pragma omp sections nowait
    {
      #pragma omp section
      {
        printf("Proc %d effectue la section 1\n", num);
        sleep(3); // Simulation d'un calcul de 3s
      }
      #pragma omp section
      {
        printf("Proc %d effectue la section 2\n", num);
        sleep(2); // Simulation d'un calcul de 2s
      }
    }
    //#pragma omp barrier
    #pragma omp master
    {
      printf("Proc␣%d␣fait␣le␣single\n", num);
      sleep(1);
    }

    fin = omp_get_wtime(); // Horloge finale

    printf("Retour à la section parallèle commune après %lfs : %d\n", fin-deb, num);
  }

  return(EXIT_SUCCESS);
}
