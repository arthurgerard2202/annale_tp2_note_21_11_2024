/// @file tp.c
/// @brief Fonctions à écrire, modifier uniquement ce fichier.
///
/// Pour la documentation de ces fonctions, ouvrir avec
/// un navigateur web le fichier ./Documentation/html/index.html.


#include "tp.h"

double valeur_tournee_partielle(point *V, int m, int *P) {
    double perimetre = 0.0;
    for (int i = 0; i < m - 1; i++) {
        perimetre += dist(V[P[i]], V[P[i + 1]]);
    }
    perimetre += dist(V[P[m - 1]], V[P[0]]);
    return perimetre;
}

void premier_couple(point *V, int n, int *first_ptr, int *second_ptr) {
    double dist_min = DBL_MAX;
    int first = 0;
    int second = 1;
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double d = dist(V[i], V[j]);
            if (d < dist_min) {
                dist_min = d;
                first = i;
                second = j;
            }
        }
    }
    
    *first_ptr = first;
    *second_ptr = second;
}

void initialise_tournee(point *V, int n, int *P) {
    for (int i = 0; i < n; i++) {
        P[i] = i;
    }
    
    int first, second;
    premier_couple(V, n, &first, &second);
    
    int temp;
    SWAP(P[0], P[first], temp);
    SWAP(P[1], P[second], temp);
}

double score(point *V, int m, int *P, int i, int *pred_ptr) {
    double augmentation_min = DBL_MAX;
    int k_min = 0;
    
    for (int k = 0; k < m; k++) {
        int next = (k + 1) % m;
        double augmentation = dist(V[P[k]], V[i]) + dist(V[i], V[P[next]]) - dist(V[P[k]], V[P[next]]);
        
        if (augmentation < augmentation_min) {
            augmentation_min = augmentation;
            k_min = k;
        }
    }
    
    *pred_ptr = k_min;
    return augmentation_min;
}

int selection_point(point *V, int n, int m, int *P, int *pred_ptr) {
    double score_min = DBL_MAX;
    int index_min = m;
    int pred_min = 0;
    
    for (int j = m; j < n; j++) {
        int pred;
        double s = score(V, m, P, P[j], &pred);
        
        if (s < score_min) {
            score_min = s;
            index_min = j;
            pred_min = pred;
        }
    }
    
    *pred_ptr = pred_min;
    return index_min;
}

void rotation_droite(int *P, int p, int q) {
    int dernier = P[q];
    for (int i = q; i > p; i--) {
        P[i] = P[i - 1];
    }
    P[p] = dernier;
}

double tsp_approximation(point *V, int n, int *P) {
    initialise_tournee(V, n, P);
    
    for (int m = 2; m < n; m++) {
        if (running && animation) {
            drawPartialTour(V, n, P, m);
            usleep(animation_delay * 50000);
        }
        
        int pred;
        int index = selection_point(V, n, m, P, &pred);
        rotation_droite(P, pred + 1, index);
    }
    
    return valeur_tournee_partielle(V, n, P);
}
