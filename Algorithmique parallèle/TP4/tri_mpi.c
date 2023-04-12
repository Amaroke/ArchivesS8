//
// Programme de tri d'un tableau 1D
//
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
// Déclarations des fonctions utilisées dans le programme
//
void aide(char *nom);                                   // Aide en ligne du programme
void genere_tableau(int *tab, int taille, uint graine); // Génération aléatoire d'un tableau d'entiers
void tri_seq(int *tab, int taille);                     // Tri séquentiel
void tri_mpi(int *tab, int taille, int numP, int nbP);  // Tri parallèle
char repartition(int *gauche, int *droite, int taille); // Répartition des valeurs min et max dans les tableaux gauche et droite
char tabsIdem(int tabA[], int tabB[], int taille);      // Comparaison de deux tableaux d'entiers
char triValide(int *tab, int taille);                   // Vérifie que le tableau tab est trié

//
// Programme principal
//
int main(int argc, char **argv)
{
  //
  // Variables
  //
  int *tabL = NULL;         // Tableau local
  int *tabS = NULL;         // Tableau global du tri séquentiel
  int *tabP = NULL;         // Tableau global du tri parallèle
  int taille = 10000;       // Taille du tableau local à traiter
  double deb;               // Chrono pour les mesures de temps
  double tempsSeq = 0;      // Temps de calcul séquentiel
  double tempsMPI = 0;      // Temps de calcul OpenMP
  double accelMPI, effMPI;  // Accélaration et efficacité OpenMP
  int opt;                  // Option de ligne de commande
  uint graine = time(NULL); // Graine du générateur aléatoire initialisée avec l'horloge de la machine
  int nbThreads = 1;        // Nombre de threads utilisés
  int numP, nbP;            // Numéro du processus MPI et nombre de processus MPI

  //
  // Lecture des paramètres
  //
  while ((opt = getopt(argc, argv, "g:hp:t:")) != -1)
  {
    switch (opt)
    {
    case 'g': // Graine
      graine = atoi(optarg);
      break;
    case 'h': // Aide
      aide(argv[0]);
      break;
    case 'p': // Nombre de threads OpenMP
      nbThreads = atoi(optarg);
      break;
    case 't': // Taille du tableau
      taille = atoi(optarg);
      break;
    }
  }

  // Init MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nbP);  // Récupération du nombre de processus MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &numP); // Récupération du numéro du processus MPI courant

  //
  // Affichage des paramètres du programme
  //
  if (numP == 0)
  {
    printf("Taille du tableau local : %d\n", taille);
    printf("Graine                  : %u\n", graine);
    printf("Nb de processus MPI     : %d\n", nbP);
  }

  //
  // Construction et initialisation aléat du tableau local à chaque processus
  //
  tabL = malloc(taille * sizeof(int));
  srand48(graine);                      // Initialisation du générateur aléatoire avec la graine donnée
  genere_tableau(tabL, taille, graine); // Génération pseudo-aléatoire du tableau

  // Construction des tableaux globaux sur le processus 0 pour comparaison finale
  if (numP == 0)
  {
    tabS = malloc(nbP * taille * sizeof(int));
    tabP = malloc(nbP * taille * sizeof(int));
  }

  // Rassemblement des tableaux sur le processus 0
  MPI_Gather(tabL, taille, MPI_INT, tabS, taille, MPI_INT, 0, MPI_COMM_WORLD);

  //
  // Tri séquentiel sur le processus 0
  //
  if (numP == 0)
  {
    deb = MPI_Wtime();
    tri_seq(tabS, nbP * taille);
    tempsSeq = MPI_Wtime() - deb;
    printf("Temps Séq : %f s\n", tempsSeq);
  }

  //
  // Tri parallèle
  //
  deb = MPI_Wtime();
  tri_mpi(tabL, taille, numP, nbP);
  tempsMPI = MPI_Wtime() - deb;

  // Rassemblement des tableaux sur le processus 0
  MPI_Gather(tabL, taille, MPI_INT, tabP, taille, MPI_INT, 0, MPI_COMM_WORLD);

  //
  // Validation des calculs parallèles (comparaison des résultats)
  //
  if (numP == 0)
  {
    printf("Temps //  : %f s\n", tempsMPI);

    if (tabsIdem(tabS, tabP, nbP * taille) == 'O')
    {
      printf("Les résultats séquentiel et MPI sont IDENTIQUES ");
      if (triValide(tabS, taille) == 'O')
      {
        printf("et BIEN triés !\n");
      }
      else
      {
        printf("mais MAL triés !\n");
      }
    }
    else
    {
      printf("Les résultats séquentiel et MPI sont DIFFÉRENTS :-/\n");
      if (triValide(tabS, nbP * taille) == 'O')
      {
        printf("Tableau séquentiel BIEN trié !\n");
      }
      else
      {
        printf("Tableau séquentiel MAL trié !\n");
      }
      if (triValide(tabP, nbP * taille) == 'O')
      {
        printf("Tableau parallèle BIEN trié !\n");
      }
      else
      {
        printf("Tableau parallèle MAL trié !\n");
      }
    }

    // Mesure de la qualité du parallélisme
    accelMPI = tempsSeq / tempsMPI;
    effMPI = accelMPI / nbP;
    printf("MPI :\n\tTemps        : %f s\n\tAccélération : %f\n\tEfficacité   : %f\n", tempsMPI, accelMPI, effMPI);
  }

  // Fermeture du système de communication mpi
  MPI_Finalize();

  //
  // Destruction du tableau dynamique
  //
  free(tabL);
  free(tabS);
  free(tabP);

  // Fin
  return (EXIT_SUCCESS);
}

//
// Génération du tableau
//
void genere_tableau(int *tab, int taille, uint graine)
{
  int i;

  // printf("Génération du tableau...");
  fflush(stdout);
#pragma omp parallel for
  for (i = 0; i < taille; ++i)
  {
    tab[i] = (rand_r(&graine) % 10001); // Valeurs entre 0 et 10000
  }
  // printf("terminée.\n");
}

//
// Tri en séquentiel
//
void tri_seq(int *tab, int taille)
{
  int i;
  char fini = 0;                 // Booléen indiquant si le tri est fini
  int deb = 0, fin = taille - 1; // Indices de début et fin de la zone non triée

  // Boucle de tri
  while (!fini)
  {
    fini = 1; // On suppose que c'est le dernier tour
    for (i = deb; i < fin; ++i)
    {
      if (tab[i] > tab[i + 1])
      {
        int tmp = tab[i + 1];
        tab[i + 1] = tab[i];
        tab[i] = tmp;
        fini = 0; // Une modification implique un tour suivant
      }
    }
    fin--; // Une case de moins à droite
    for (i = fin; i > deb; --i)
    {
      if (tab[i] < tab[i - 1])
      {
        int tmp = tab[i - 1];
        tab[i - 1] = tab[i];
        tab[i] = tmp;
        fini = 0; // Une modification implique un tour suivant
      }
    }
    deb++; // Une case de moins à gauche
  }
}

//
// Tri en parallèle
//
void tri_mpi(int *tab, int taille, int numP, int nbP)
{
  int *tabR = NULL; // Tableau reçu d'un voisin
  char fini = 0;    // Booléen indiquant si le tri global est fini
  // int  modif = 0;        // Booléen indiquant si une modification a été faite lors de la répartition
  char cote = numP % 2; // Côté avec lequel on échange les valeurs (gauche = 0 et droite = 1)

  // Construction du tableau reçu d'un processus voisin
  tabR = malloc(taille * sizeof(int));

  // Boucle de tri
  while (!fini)
  {

    //
    // À COMPLÉTER/MODIFIER !
    //

    // Tri du tableau local
    for (int i = 0; i < taille / nbP; ++i)
    {
      for (int j = 0; j < i - 1; ++j)
      {
        if (tabR[j + 1] < tabR[j])
        {
          int tmp = tab[j + 1];
          tab[j + 1] = tab[j];
          tab[j] = tmp;
        }
      }
    }

    // Alternance du côté d'échange des valeurs (gauche / droite)
    cote = (cote + 1) % 2;

    // Envois des tableaux entre voisins
    if (cote == 0)
    { // Échange avec voisin de gauche

      MPI_Send(tab, taille, MPI_INT, numP - 1, 1, MPI_COMM_WORLD);
      MPI_Recv(tabR, taille / nbP, MPI_INT, numP + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      if (tabR[(numP + 1) * taille / nbP] < tab[numP * taille / nbP + nbP - 1])
      {
        int tmp = tab[numP * taille / nbP + nbP];
        tab[numP * taille / nbP + nbP] = tabR[(numP + 1) * taille / nbP];
        tabR[(numP + 1) * taille / nbP] = tmp;
      }

      // Répartition des valeurs
      // modif = 0;
    }
    else
    { // Échange avec voisin de droite

      MPI_Recv(tabR, taille / nbP, MPI_INT, numP + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(tab, taille, MPI_INT, numP - 1, 1, MPI_COMM_WORLD);

      if (tabR[numP * taille / nbP + nbP - 1] > tab[numP * taille / nbP + nbP])
      {
        int tmp = tabR[numP * taille / nbP + nbP - 1];
        tabR[numP * taille / nbP + nbP - 1] = tab[numP * taille / nbP + nbP];
        tab[numP * taille / nbP + nbP] = tmp;
      }

      // Répartition des valeurs
      // modif = 0;
    }

    // Vérification de la fin du tri global
    char valide = triValide(tab, taille);

    fini = (valide == 'O') ? 1 : 0;
  }

  // Libéartion du tableau local
  free(tabR);
}

//
// Répartition des éléments min et max dans les parties gauche et droite
//
char repartition(int *gauche, int *droite, int taille)
{
  int g = taille - 1; // Indice dans le tableau gauche
  int d = 0;          // Indice dans le tableau droite
  int tmp;            // Variable d'échange

  while (g >= 0 && d < taille && gauche[g] > droite[d])
  {
    tmp = gauche[g];
    gauche[g] = droite[d];
    droite[d] = tmp;
    g--;
    d++;
  }

  return (d > 0);
}

//
// Comparaison de deux tableaux d'occurrences de lettres
//
char tabsIdem(int tabA[], int tabB[], int taille)
{
  int i;

  for (i = 0; i < taille && tabA[i] == tabB[i]; ++i)
    ;

  if (i < taille)
  {
    return 'N';
  }
  else
  {
    return 'O';
  }
}

//
// Comparaison de deux tableaux d'occurrences de lettres
//
char triValide(int *tab, int taille)
{
  int i;

  for (i = 0; i < taille - 1 && tab[i] <= tab[i + 1]; ++i)
    ;

  if (i < taille - 1)
  {
    return 'N';
  }
  else
  {
    return 'O';
  }
}

//
// Aide en ligne
//
void aide(char *nom)
{
  printf("Usage : %s <-g int> <-h> <-p int> <-t int>\n", nom);
  printf("\t-g int : graine du générateur aléatoire\n");
  printf("\t-h     : aide en ligne\n");
  printf("\t-p int : nombre de processus OpenMP\n");
  printf("\t-t int : taille du tableau généré\n");
  exit(EXIT_SUCCESS);
}
