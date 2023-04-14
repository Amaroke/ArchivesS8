#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> 
#include <chrono>
#include <time.h>

using namespace std;

#include "clutils.h"

void sort(int T[], int n, bool croissant, int stage);
void merge(int T[], int L, int R, bool croissant, int column, int stage);
void compare(int T[], int L, int R, bool croissant, int column, int stage);

void sort(int T[], int L, int R, bool croissant, int stage) {
	if (R - L + 1 > 1) {
		sort(T, L, (R + L) / 2, croissant, stage - 1);
		sort(T, (R + L) / 2 + 1, R, !croissant, stage - 1);
		merge(T, L, R, croissant, 1, stage);
	}
}

void merge(int T[], int L, int R, bool croissant, int column, int stage) {
	if (R - L + 1 > 1) {
		compare(T, L, R, croissant, column, stage);
		merge(T, L, (L + R) / 2, croissant, column + 1, stage);
		merge(T, (L + R) / 2 + 1, R, croissant, column + 1, stage);
	}
}

void compare(int T[], int L, int R, bool croissant, int column, int stage) {
	int k = (R - L + 1) / 2;
	std::cout << "stage " << stage << ", column " << column << ", " << T[L] << " > " << T[L + k] << endl;
	for (int i = 0; i < k; i++) {
		if (!croissant) {
			if (T[L + i] < T[L + i + k]) {
				int tmp = T[L + i + k];
				T[L + i + k] = T[L + i];
				T[L + i] = tmp;
			}
		}
		else {
			if (T[L + i] > T[L + i + k]) {
				int tmp = T[L + i + k];
				T[L + i + k] = T[L + i];
				T[L + i] = tmp;
			}
		}
	}
}

int main(int argc, char** argv)
{	const int N = 64;
	int T[N];
	for (int i = 0; i < N; i++) {
		T[i] = rand() % 100;
	}

	sort(T, 0, N - 1, true, log2(N));

	// Affichage du tableau trié
	std::cout << "Tableau trie en ordre croissant : ";
	for (int i = 0; i < N; i++) {
		cout << T[i] << " ";
	}
	cout << endl;

	return 0;
}