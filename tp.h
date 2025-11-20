/**
 * @file tp.h
 * @brief Documentation des fonctions à écrire pour le TP.\n
 * <span style="color:#69A1D3;">Ne pas modifier le  fichier tp.h.</span>\n
 * <span style="color:#C36C73;">Modifier uniquement le fichier tp.c.</span>
 */

#ifndef _TP_H
#define _TP_H

#include <assert.h>

#include "alloc.h"
#include "error.h"
#include "tools.h"
#include "util.h"

extern int animation, animation_delay;

/// Renvoie le périmètre de la tournée partielle :
/// `V[P[0]]` → `V[P[1]]` → ... → `V[P[m-1]]` → `V[P[0]]`,
/// c'est-à-dire la somme des distances de cette tournée :
/// \f[ \sum_{i=0}^{m-2}{{\sf dist}(V[P[i]],V[P[i+1]])}\qquad+\qquad{\sf
/// dist}(V[P[m-1]], V[P[0]])
/// \f] On suppose :
/// - que les tableaux `V` et `P` contiennent le même nombre d'éléments, `n`.
/// - que les éléments de `P` sont des indices valides de `V`.
/// - que `m` est un entier tel que `2 ⩽ m ⩽ n`.
double valeur_tournee_partielle(
    point *V, //!< Le tableau de points.
    int m,    //!< Un entier inférieur ou égal au nombre de points de `V`.
    int *P //!< Un tableau de `n` indices valides de `V` dont les `m` premiers
           //!< donnent la tournée partielle.
);

/// Affecte les entiers aux adresses `first_ptr` et `second_ptr` avec les
/// indices distincts `i`,`j` de deux points  de `V` qui sont les plus proches
/// l'un de l'autre parmi tous les points de `V`.
///
/// Si plusieurs couples d'indices `i`,`j` réalisent la distance minimale, on
/// choisit le couple d'indices de valeur la plus petite dans l'ordre
/// lexicographique (voir le sujet pdf pour la définition de cet ordre). En
/// particulier, l'entier pointé par `first_ptr` sera strictement inférieur à
/// celui pointé par `second_ptr` avant le retour de la fonction.
void premier_couple(point *V,       //!< Le tableau de points.
                    int n,          //!< Le nombre de points dans `V`.
                    int *first_ptr, //!< Un pointeur vers un entier déjà alloué.
                    int *second_ptr //!< Un pointeur vers un entier déjà alloué.
);

/// Initialise la tournée `P` de la façon suivante :
/// - On commence par initialiser `P` avec l'identité, c'est-à-dire `P[i] = i`
/// pour tout indice `i` entre `0` et `n-1`.
/// - On trouve ensuite dans `V` les indices `first` et `second` des deux points
///   les plus proches de `V` (en utilisant la fonction `premier_couple`).
/// - Puis, on échange `P[0]` et `P[first]`.
/// - Enfin, on échange `P[1]` et `P[second]`.
void initialise_tournee(
    point *V, //!< Le tableau de points.
    int n,    //!< Le nombre de points dans `V`.
    int *P    //!< Un tableau contenant les `n` indices de `V` avant l'appel.
);

/// Renvoie l'augmentation **minimale** de la valeur de la tournée partielle
/// `V[P[0]]` → `V[P[1]]` → ... → `V[P[m-1]]` → `V[P[0]]`
/// si insère le point `V[i]` dans cette tournée.
///
/// On doit donc tester toutes les positions auxquelles on peut insérer ce
/// point. Autrement dit, il faut considérer l'augmentation de la valeur de la
/// tournée si on insère le point `V[i]` :
/// - entre `V[P[0]]` et `V[P[1]]`,
/// - entre `V[P[1]]` et `V[P[2]]`,
/// ...
/// - entre `V[P[m-2]]` et `V[P[m-1]]`,
/// - entre `V[P[m-1]]` et `V[P[0]]`,
///
/// et trouver le cas où cette augmentation est la plus petite possible.
///
/// La fonction remplit dans `pred_ptr` l'indice `k` dans `P` (parmi `0`..`m-1`)
/// après lequel insérer `i` pour minimiser cette augmentation. S'il y a
/// plusieurs possibilités, on choisit l'entier `k` minimal.
double score(point *V, //!< Le tableau de points.
             int m,    //!< La longueur de la tournée partielle.
             int *P,   //!< Un tableau de `n` indices valides de `V` dont les
                       //!< `m` premiers donnent la tournée partielle.
             int i,    //!< L'indice du point à insérer.
             int *pred_ptr //!< Un pointeur vers un entier déjà alloué.
);

/// Retourne l'indice `i` ≥ `m` du tableau `P` tel que `V[P[i]]` réalise
/// l'insertion la moins coûteuse dans la tournée partielle `V[P[0]]` →
/// `V[P[1]]` → ... → `V[P[m-1]]` → `V[P[0]]`.
///
/// Remplit `pred_ptr` avec l'indice dans `P` du point de la tournée partielle
/// qui réalise cet optimum.
int selection_point(point *V, //!< Le tableau de points.
                    int n,    //!< Le nombre de points dans `V`.
                    int m,    //!< La longueur de la tournée partielle.
                    int *P,   //!< Un tableau de `n` indices valides de `V` dont
                            //!< les `m` premiers donnent la tournée partielle.
                    int *pred_ptr //!< Un pointeur vers un entier déjà alloué.
);

/// Réalise la rotation droite dans le tableau `P` entre les indices `p`
/// (inclus) et `q` (inclus), comme schématisé sur la dernière figure du sujet
/// pdf.
void rotation_droite(int *P, //!< Le tableau.
                     int p,  //!< le premier indice.
                     int q   //!< le second indice (on suppose p < q).
);

/// En utilisant les fonctions précédentes, calcule une tournée par ajouts
/// successifs d'un point à partir de la tournée initiale.
/// @return La valeur de la tournée obtenue.
double tsp_approximation(point *V, //!< Le tableau de points.
                         int n,    //!< Le nombre de points dans `V`.
                         int *P //!< Un tableau de `n` entiers, que la fonction
                                //!< doit remplir avec la tournée calculée.
);

#endif /* _TP_H */
