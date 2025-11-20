/**
 * @file util.h
 * @brief Fonctions utiles : distance entre deux points, min, max, affichage de
 * tableaux.\n
 * <span style="color:#69A1D3;">Ne pas modifier ce fichier.</span>
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "tools.h"

/// Renvoie la distance euclidienne entre deux points.
double dist(point A, point B);

/// Macro pour simplifier les expressions de calcul des distances
#define D(a, b) (dist(V[P[a]], V[P[b]]))

/// Renvoie le minimum de deux entiers.
int min(int a, int b);

/// Renvoie le maximum de deux entiers.
int max(int a, int b);

/// Affiche le tableau P de taille n.
void print_tab(int *P, int n);

/// Affiche les `n` points du tableau `V` dans le flux `f`.
void print_points(FILE *f, point *V, int n);

#endif // UTIL_H_
