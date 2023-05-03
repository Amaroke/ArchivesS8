/*
   Programme de calcul des positions min et max dans un tableau 2D
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>

//
// Déclarations des types spécifiques utilisés dans le programme
//
typedef double reel; // Type utilisé pour représenter les réels

typedef struct
{
  int lig, col; // Ligne et colonne dans le tableau 2D
} Position;

typedef struct
{
  Position min, max; // Positions min et max dans le tableau 2D
} MinMax;

//
// Déclarations des fonctions utilisées dans le programme
//
reel **genere_tab(int L, int C, long graine);                                            // Génération d'un tableau aléatoire de réels
void minmaxSeq(reel **tab, int L, int C, MinMax *minmax);                                // Calcul des positions min et max dans tab en séquentiel
void minmaxOMP(reel **tab, int L, int C, int nbT, MinMax *minmax);                       // Calcul des positions min et max dans tab avec nbT threads OpenMP
void minmaxMPI(reel **tab, int L, int C, int numP, int nbP, MinMax *minmax);             // Calcul des positions min et max dans tab avec nbP processus MPI
void minmaxMPIOMP(reel **tab, int L, int C, int numP, int nbP, int nbT, MinMax *minmax); // Calcul des positions min et max dans tab avec nbP processus MPI et nbT threads OpenMP par processus MPI
void afficheMinMax(reel **tab, MinMax *minmax);                                          // Affichage d'un couple de positions min et max avec leurs valeurs respectives

//
// Programme principal
//
int main(int argc, char **argv)
{
  //
  // Variables
  //
  reel **tab = NULL;             // Tableau à traiter
  int L = 10000;                 // Nombre par défaut de lignes de tab
  int C = 10000;                 // Nombre par défaut de colonnes de tab
  double deb;                    // Chrono pour les mesures de temps
  double tempsSeq = 0;           // Temps de calcul séquentiel
  double tempsOMP = 0;           // Temps de calcul OpenMP
  double tempsMPI = 0;           // Temps de calcul MPI
  double tempsMPIOMP = 0;        // Temps de calcul MPI + OpenMP
  double accelOMP, effOMP;       // Accélaration et efficacité OpenMP
  double accelMPI, effMPI;       // Accélaration et efficacité MPI
  double accelMPIOMP, effMPIOMP; // Accélaration et efficacité MPI + OpenMP
  int opt;                       // Option de ligne de commande
  long graine = time(NULL);      // Graine du générateur aléatoire initialisée avec l'horloge de la machine
  int nbT = 1;                   // Nombre de threads utilisés
  int numP, nbP;                 // Numéro du processus MPI et nombre de processus MPI
  MinMax mmSeq;                  // Résultat de référence calculé en séquentiel
  MinMax mmOMP, mmMPI, mmMPIOMP; // Résultats pour les trois versions parallèles

  //
  // Lecture des paramètres
  //
  while ((opt = getopt(argc, argv, "c:g:l:t:")) != -1)
  {
    switch (opt)
    {
    case 'c': // Colonnes de tab
      C = atoi(optarg);
      break;
    case 'g': // Graine
      graine = atol(optarg);
      break;
    case 'l': // Lignes de tab
      L = atoi(optarg);
      break;
    case 't': // Nb de threads
      nbT = atoi(optarg);
      break;
    }
  }

  //
  // Init MPI
  //
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nbP);  // Récupération du nombre de processus MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &numP); // Récupération du numéro du processus MPI courant

  //
  // Affichage des paramètres du programme
  //
  if (numP == 0)
  {
    printf("\n");
    printf("Taille du tableau : %dx%d\n", L, C);
    printf("Graine            : %ld\n", graine);
    printf("Nb Procs MPI      : %d\n", nbP);
    printf("Nb Threads OMP    : %d\n", nbT);
  }

  //
  // Construction et remplissage du tableau sur tous les processus MPI
  //
  tab = genere_tab(L, C, graine);

  //
  // Calcul en séquentiel sur proc MPI 0
  //
  if (numP == 0)
  {
    printf("Séquentiel :\n");
    deb = omp_get_wtime();
    minmaxSeq(tab, L, C, &mmSeq);
    tempsSeq = omp_get_wtime() - deb;
    afficheMinMax(tab, &mmSeq);
    printf("\tTemps        : %f s\n", tempsSeq);
  }

  //
  // Barrière pour attendre que le processus 0 ait fini la version séquentielle
  //
  MPI_Barrier(MPI_COMM_WORLD);

  //
  // Calcul avec OpenMP sur proc MPI 0
  //
  if (numP == 0)
  {
    printf("OpenMP :\n");
    deb = omp_get_wtime();
    minmaxOMP(tab, L, C, nbT, &mmOMP);
    tempsOMP = omp_get_wtime() - deb;

    // Affichage pour comparaison des résultats
    afficheMinMax(tab, &mmOMP);

    // Mesure de la qualité du parallélisme OpenMP
    accelOMP = tempsSeq / tempsOMP;
    effOMP = accelOMP / nbT;
    printf("\tTemps        : %f s\n\tAccélération : %f\n\tEfficacité   : %f\n", tempsOMP, accelOMP, effOMP);
  }

  //
  // Barrière pour attendre que le processus 0 ait fini la version OpenMP
  //
  MPI_Barrier(MPI_COMM_WORLD);

  //
  // Calcul avec MPI mono-thread et récupération du résultat et du temps sur proc MPI 0
  //
  if (numP == 0)
  {
    deb = omp_get_wtime();
  }
  minmaxMPI(tab, L, C, numP, nbP, &mmMPI);
  if (numP == 0)
  {
    printf("MPI :\n");
    tempsMPI = omp_get_wtime() - deb;

    // Affichage pour comparaison des résultats
    afficheMinMax(tab, &mmMPI);

    // Mesure de la qualité du parallélisme MPI
    accelMPI = tempsSeq / tempsMPI;
    effMPI = accelMPI / nbP;
    printf("\tTemps        : %f s\n\tAccélération : %f\n\tEfficacité   : %f\n", tempsMPI, accelMPI, effMPI);
  }

  //
  // Barrière pour attendre que le processus 0 ait fini la version MPI
  //
    MPI_Barrier(MPI_COMM_WORLD);

  //
  // Calcul avec MPI multi-threads et récupération du résultat et du temps sur proc MPI 0
  //
  if (numP == 0)
  {
    deb = omp_get_wtime();
  }
  minmaxMPIOMP(tab, L, C, numP, nbP, nbT, &mmMPIOMP);
  if (numP == 0)
  {
    printf("MPI + OpenMP :\n");
    tempsMPIOMP = omp_get_wtime() - deb;

    // Affichage pour comparaison des résultats
    afficheMinMax(tab, &mmMPIOMP);

    // Mesure de la qualité du parallélisme MPI multi-threads
    accelMPIOMP = tempsSeq / tempsMPIOMP;
    effMPIOMP = accelMPIOMP / (nbP * nbT);
    printf("\tTemps        : %f s\n\tAccélération : %f\n\tEfficacité   : %f\n", tempsMPIOMP, accelMPIOMP, effMPIOMP);
  }

  //
  // Arrêt du système MPI
  //
  MPI_Finalize();

  //
  // Destruction du tableau dynamique
  //
  free(tab[0]);
  free(tab);

  // Fin
  return 0;
}

//
// Génération du tableau
//
reel **genere_tab(int L, int C, long graine)
{
  int l, c;
  reel min = 1e6, max = -1e6;
  reel **tab = (reel **)malloc(L * sizeof(reel *));
  reel bmin, bmax, diff;

  tab[0] = (reel *)malloc(L * C * sizeof(reel));
  for (l = 1; l < L; ++l)
  {
    tab[l] = tab[l - 1] + C;
  }

  srand(graine);
  bmin = -100. * rand() / RAND_MAX;
  bmax = 100. * rand() / RAND_MAX;
  diff = (bmax - bmin) / RAND_MAX;
  printf("Bornes %.3f et %.3f\n", bmin, bmax);

  printf("Génération du tableau...");
  fflush(stdout);

#pragma omp parallel
  {
    uint etat = graine;

#pragma omp for reduction(min                  \
                          : min) reduction(max \
                                           : max)
    for (l = 0; l < L; ++l)
    {
      for (c = 0; c < C; ++c)
      {
        tab[l][c] = (reel)(rand_r(&etat) * diff + bmin);
        if (tab[l][c] < min)
        {
          min = tab[l][c];
        }
        if (tab[l][c] > max)
        {
          max = tab[l][c];
        }
      }
    }
  }
  printf("terminée avec min = %.3f et max = %.3f.\n", min, max);

  return tab;
}

//
// Calcul des positions min et max dans tab en séquentiel
//
void minmaxSeq(reel **tab, int L, int C, MinMax *minmax)
{
  int l, c;

  // Initialisation des positions
  minmax->min = {0, 0};
  minmax->max = {0, 0};

  // Parcours du tableau
  for (l = 0; l < L; ++l)
  {
    for (c = 0; c < C; ++c)
    {
      if (tab[l][c] < tab[minmax->min.lig][minmax->min.col])
      {
        minmax->min.lig = l;
        minmax->min.col = c;
      }
      if (tab[l][c] > tab[minmax->max.lig][minmax->max.col])
      {
        minmax->max.lig = l;
        minmax->max.col = c;
      }
    }
  }
}

//
// Calcul des positions min et max dans tab avec nbT threads OpenMP
//
void minmaxOMP(reel **tab, int L, int C, int nbT, MinMax *minmax)
{
  int l, c;
  
  // Initialisation des positions
  minmax->min = {0, 0};
  minmax->max = {0, 0};

  // À COMPLÉTER/MODIFIER !
  // --------------------
  // -> Créer les threads qui vont se répartir le tableau à parcourir
  //    Chacun calcule un minmax sur sa partie
  //    Les mimmax obtenus doivent être fusionnés dans le résultat final

  // Calcul des indices de début et de fin pour chaque thread et répartitions des lignes restantes
  int tailleSousTableau = L / nbT;
  int lignesRestantes = L % nbT;
  int deb[nbT], fin[nbT];
  int emplacement = 0;
  for (int i = 0; i < nbT; ++i) {
    deb[i] = emplacement;
    emplacement += tailleSousTableau;
    if (lignesRestantes > 0) {
      ++emplacement;
      --lignesRestantes;
    }
    fin[i] = emplacement;
  }

  // Parcours tableau + création Threads
  #pragma omp parallel num_threads(nbT)
  {
    int numP = omp_get_thread_num();
    int ligne_min_local = deb[numP], ligne_max_local = deb[numP];
    int colonne_min_local = 0, colonne_max_local = 0;
    for (l = deb[numP]; l < fin[numP]; ++l) {
      for (c = 0; c < C; ++c) {
        if (tab[l][c] < tab[ligne_min_local][colonne_min_local]) {
          ligne_min_local = l;
          colonne_min_local = c;
        }
        if (tab[l][c] > tab[ligne_max_local][colonne_max_local]) {
          ligne_max_local = l;
          colonne_max_local = c;
        }
      }
    }
    // Fusion des min max
    #pragma omp critical
    {
      if (tab[ligne_min_local][colonne_min_local] < tab[minmax->min.lig][minmax->min.col]) {
        minmax->min.lig = ligne_min_local;
        minmax->min.col = colonne_min_local;
      }
      if (tab[ligne_max_local][colonne_max_local] > tab[minmax->max.lig][minmax->max.col]) {
        minmax->max.lig = ligne_max_local;
        minmax->max.col = colonne_max_local;
      }
    }
  }
}


//
// Calcul des positions min et max dans tab avec nbP processus MPI
//
void minmaxMPI(reel **tab, int L, int C, int numP, int nbP, MinMax *minmax)
{
  int l, c;

  // Initialisation des positions
  minmax->min = {0, 0};
  minmax->max = {0, 0};

  // À COMPLÉTER/MODIFIER !
  // --------------------
  // -> Répartir le tableau à parcourir entre les processus MPI
  //    Chacun calcule un minmax sur sa partie
  //    Les mimmax obtenus doivent être fusionnés dans le résultat final du processus 0

  // Répartition des tableaux
  int tailleSousTableau = L / nbP;
  int lignesRestantes = L % nbP;
  int deb[nbP], fin[nbP];
  int emplacement = 0;
  for (int i = 0; i < nbP; ++i) {
    deb[i] = emplacement;
    emplacement += tailleSousTableau;
    if (lignesRestantes > 0) {
      ++emplacement;
      --lignesRestantes;
    }
    fin[i] = emplacement;
  }

  // Chaque processus MPI recherche son min et son max
  int ligne_min = deb[numP], ligne_max = deb[numP];
  int colonne_min = 0, colonne_max = 0;
  for (l = deb[numP]; l < fin[numP]; ++l) {
    for (c = 0; c < C; ++c) {
      if (tab[l][c] < tab[ligne_min][colonne_min]) {
        ligne_min = l;
        colonne_min = c;
      }
      if (tab[l][c] > tab[ligne_max][colonne_max]) {
        ligne_max = l;
        colonne_max = c;
      }
    }
  }

  // Fusion des résultats dans le processus 0 avec send et recv
  if (numP == 0) {
    int ligne_min_local, ligne_max_local, colonne_min_local, colonne_max_local;
    for (int i = 1; i < nbP; ++i) {
      MPI_Recv(&ligne_min_local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&colonne_min_local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&ligne_max_local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&colonne_max_local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if (tab[ligne_min_local][colonne_min_local] < tab[ligne_min][colonne_min]) {
        ligne_min = ligne_min_local;
        colonne_min = colonne_min_local;
      }
      if (tab[ligne_max_local][colonne_max_local] > tab[ligne_max][colonne_max]) {
        ligne_max = ligne_max_local;
        colonne_max = colonne_max_local;
      }
    }
    minmax->min.lig = ligne_min;
    minmax->min.col = colonne_min;
    minmax->max.lig = ligne_max;
    minmax->max.col = colonne_max;
  } else {
    MPI_Send(&ligne_min, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&colonne_min, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&ligne_max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&colonne_max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

}

//
// Calcul des positions min et max dans tab avec nbP processus MPI et nbT threads OpenMP par processus MPI
//
void minmaxMPIOMP(reel **tab, int L, int C, int numP, int nbP, int nbT, MinMax *minmax)
{
  int l, c;
  
  // Initialisation des positions
  minmax->min = {0, 0};
  minmax->max = {0, 0};

  // À COMPLÉTER/MODIFIER !
  // --------------------
  // -> Créer les threads qui vont se répartir le tableau à parcourir
  //    Chacun calcule un minmax sur sa partie
  //    Les mimmax obtenus doivent être fusionnés dans le résultat final

  // Répartition des tableaux
  int tailleSousTableau = L / nbP;
  int lignesRestantes = L % nbP;
  int deb[nbP], fin[nbP];
  int emplacement = 0;
  for (int i = 0; i < nbP; ++i) {
    deb[i] = emplacement;
    emplacement += tailleSousTableau;
    if (lignesRestantes > 0) {
      ++emplacement;
      --lignesRestantes;
    }
    fin[i] = emplacement;
  }

  // Chaque processus MPI recherche son min et son max avec OpenMP
  int ligne_min = deb[numP], ligne_max = deb[numP];
  int colonne_min = 0, colonne_max = 0;

  #pragma omp parallel num_threads(nbT)
  {
    int ligne_min_local = deb[numP], ligne_max_local = deb[numP];
    int colonne_min_local = 0, colonne_max_local = 0;

    #pragma omp for
    for (l = deb[numP]; l < fin[numP]; ++l) {
      for (c = 0; c < C; ++c) {
        if (tab[l][c] < tab[ligne_min_local][colonne_min_local]) {
          ligne_min_local = l;
          colonne_min_local = c;
        }
        if (tab[l][c] > tab[ligne_max_local][colonne_max_local]) {
          ligne_max_local = l;
          colonne_max_local = c;
        }
      }
    }

    #pragma omp critical
    {
      if (tab[ligne_min_local][colonne_min_local] < tab[ligne_min][colonne_min]) {
        ligne_min = ligne_min_local;
        colonne_min = colonne_min_local;
      }
      if (tab[ligne_max_local][colonne_max_local] > tab[ligne_max][colonne_max]) {
        ligne_max = ligne_max_local;
        colonne_max = colonne_max_local;
      }
    }
  }

  // Fusion des résultats dans le processus 0 avec send et recv
    if (numP == 0) {
        int ligne_min_local, ligne_max_local, colonne_min_local, colonne_max_local;
        for (int i = 1; i < nbP; ++i) {
        MPI_Recv(&ligne_min_local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&colonne_min_local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&ligne_max_local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&colonne_max_local, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (tab[ligne_min_local][colonne_min_local] < tab[ligne_min][colonne_min]) {
            ligne_min = ligne_min_local;
            colonne_min = colonne_min_local;
        }
        if (tab[ligne_max_local][colonne_max_local] > tab[ligne_max][colonne_max]) {
            ligne_max = ligne_max_local;
            colonne_max = colonne_max_local;
        }
        }
        minmax->min.lig = ligne_min;
        minmax->min.col = colonne_min;
        minmax->max.lig = ligne_max;
        minmax->max.col = colonne_max;
    } else {
        MPI_Send(&ligne_min, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&colonne_min, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&ligne_max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&colonne_max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

//
// Affichage d'un couple de positions min et max avec leurs valeurs respectives
//
void afficheMinMax(reel **tab, MinMax *minmax)
{
  printf("\tMin          : %5d %5d %.3f\n", minmax->min.lig, minmax->min.col, tab[minmax->min.lig][minmax->min.col]);
  printf("\tMax          : %5d %5d %.3f\n", minmax->max.lig, minmax->max.col, tab[minmax->max.lig][minmax->max.col]);
}
