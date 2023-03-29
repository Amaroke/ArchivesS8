/*
   Programme de démarrage de MPI
   Fichier à compiler avec mpicc
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  int nbP, num; // Nombre de processus et numéro du processus courant

  // Initialisation du système MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nbP); // Récupération du nombre de processus
  MPI_Comm_rank(MPI_COMM_WORLD, &num); // Récupération du numéro du processus courant

  if (num != 0)
  {
    // Réception du message du processus précédent
    int source = (num - 1 + nbP) % nbP;
    int message;
    MPI_Recv(&message, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Je suis le processus %d et j'ai reçu le message %d\n", num, message);
    fflush(stdout);
    sleep(1);
  }
  // Envoi du message au processus suivant
  if (num != nbP - 1)
  {
    int dest = (num + 1) % nbP;
    MPI_Ssend(&num, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
  }

  // Arrêt du système MPI
  MPI_Finalize();

  // Retour système
  return (EXIT_SUCCESS);
}
