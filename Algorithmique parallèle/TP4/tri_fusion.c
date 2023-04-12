#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Fonction de tri fusion du tableau tab de taille spécifiée
// avec ou sans parallélisme selon la valeur de par
void triFusion(double *tab, int taille, char par);

//
// Programme principal
//
int main(int argc, char **argv)
{
  int i;                               // Compteur
  double *tabS = NULL, *tabP = NULL;   // Tableaux de version séquentielle et parallèle
  int taille = 10000;                   // Taille du tableau
  int graine = 1;                      // Graine pour générateur aléatoire
  int nbT = 1;                         // Nombre de threads
  double deb, fin, tempsS, tempsP = 1; // Chronos
  double acc, eff;                     // Mesures de qualité
  int prec = 100;                      // Précision

  // Lecture des paramètres (mieux avec getopt())
  for (i = 1; i < argc; ++i)
  {
    if (argv[i][0] == '-')
    {
      switch (argv[i][1])
      {
      case 'n': // Taille du tableau
        i++;
        taille = atoi(argv[i]);
        break;
      case 'g': // Graine aléat
        i++;
        graine = atoi(argv[i]);
        break;
      case 'p': // Précision
        i++;
        prec = atoi(argv[i]);
        break;
      case 't': // Nombre de threads
        i++;
        nbT = atoi(argv[i]);
        break;
      }
    }
  }

  // Construction des tableaux
  tabS = malloc(taille * sizeof(double));
  tabP = malloc(taille * sizeof(double));

  // Remplissage des tableaux
  srand(graine);
  for (i = 0; i < taille; ++i)
  {
    tabS[i] = floor((2.0 * rand() / RAND_MAX - 1.0) * prec) / prec;
    tabP[i] = tabS[i];
  }

  // Tri séquentiel avec mesure du temps
  deb = omp_get_wtime();
  triFusion(tabS, taille, 0);
  fin = omp_get_wtime();
  tempsS = fin - deb;

  // Vérification tri séquentiel
  char triOK = 1;
  for (i = 0; i < taille - 1 && triOK == 1; ++i)
  {
    if (tabS[i] > tabS[i + 1])
      triOK = 0;
  }

  // Affichage lié au tri séquentiel
  printf("Tri Séq : %s %lf s\n", ((triOK == 1) ? "OK" : "PAS BON"), tempsS);

  // Tri parallèle
  deb = omp_get_wtime();
  #pragma omp parallel num_threads(nbT)
  {
    #pragma omp single
    {
      triFusion(tabP, taille, 1);
    }
  }
  fin = omp_get_wtime();
  tempsP = fin - deb;

  // Vérification tri parallèle
  triOK = 1;
  for (i = 0; i < taille - 1 && triOK == 1; ++i)
  {
    if (tabP[i] != tabS[i])
      triOK = 0;
  }

  // Affichage lié au tri parallèle
  printf("Tri Par : %s %lf s\n", ((triOK == 1) ? "OK" : "PAS BON"), tempsP);

  // Calcul et affichage des mesures de qualité
  acc = tempsS / tempsP;
  eff = acc / nbT;
  printf("Acc = %lf\tEff = %lf\n", acc, eff);

  // Libération des tableaux
  free(tabS);
  free(tabP);

  return (EXIT_SUCCESS);
}

//
// Fusion des 2 parties triées de tab séparées par sep
//
void fusion(double *tab, int taille, int sep)
{
  int i, j, k;
  double *tmp = malloc(taille * sizeof(double)); // Tableau temporaire

  // Parcours des 2 sous-parties
  for (i = 0, j = sep, k = 0; i < sep && j < taille; k++)
  {
    if (tab[i] <= tab[j])
    {
      tmp[k] = tab[i];
      i++;
    }
    else
    {
      tmp[k] = tab[j];
      j++;
    }
  }

  // Parcours reste 1ère partie
  for (; i < sep; ++i, k++)
  {
    tmp[k] = tab[i];
  }

  // Parcours reste 2nde partie
  for (; j < taille; ++j, k++)
  {
    tmp[k] = tab[j];
  }

  // Recopie dans tab
  for (i = 0; i < taille; ++i)
  {
    tab[i] = tmp[i];
  }

  // Libération du tableau temporaire
  free(tmp);
}

//
// Tri fusion
//
void triFusion(double *tab, int taille, char par)
{
  if (taille > 2)
  {
    int demieTaille = taille / 2, reste = taille - demieTaille;

    if (par == 1 && taille > 10000)
    {
      #pragma omp task
      triFusion(tab, demieTaille, par);
      #pragma omp task
      triFusion(tab + demieTaille, reste, par);
      #pragma omp taskwait
    }
    else
    {
      triFusion(tab, demieTaille, par);
      triFusion(tab + demieTaille, reste, par);
    }
    fusion(tab, taille, demieTaille);
  }
  else
  {
    if (taille == 2)
    {
      if (tab[0] > tab[1])
      {
        double tmp = tab[0];
        tab[0] = tab[1];
        tab[1] = tmp;
      }
    }
  }
}
