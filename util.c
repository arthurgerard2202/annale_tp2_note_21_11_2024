#include "util.h"

// Distance Euclidienne
double dist(point A, point B) {
    const double x = A.x - B.x;
    const double y = A.y - B.y;
    return sqrt(x * x + y * y);
}

inline int min(int a, int b) {
    return a < b ? a : b;
}

inline int max(int a, int b) {
    return a > b ? a : b;
}

// Affiche le tableau d'entiers P
void print_tab(int *P, int n) {
    for (int i = 0; i < n; ++i) {
        fprintf(stderr, "%d ", P[i]);
    }
    fprintf(stderr, "\n");
}

// Affiche le tableau d'entiers P
void print_points(FILE *f, point *V, int n) {
    for (int i = 0; i < n; ++i) {
       fprintf(f, "V[%2d] = (%-3.2f, %-3.2f)\n", i, V[i].x, V[i].y);
    }
    fprintf(f, "\n");
}
