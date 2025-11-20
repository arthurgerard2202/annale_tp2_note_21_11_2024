#include <getopt.h>

#include "tools.h"
#include "util.h"
#include "tp.h"

static int no_flag       = 0;
static int cheapest_flag = 0;
static int help_flag = 0;

int animation, animation_delay;

static int n_option    = -1;
static int seed_option = -1;

// pour l'option --xy
enum {
    XY_UNIFORM,
    XY_CIRCLE,
    XY_LOAD,
    XY_POWER,
    XY_GRID,
    XY_CONVEX,
};

static int    xy_option = XY_UNIFORM;
static int    xy_circle;
static int    xy_grid_p;
static int    xy_grid_q;
static double xy_power;
static char   xy_load[MAX_FILE_NAME];

static struct option long_options[] = {
    {"no", no_argument, &no_flag, 1},
    {"cheapest", no_argument, &cheapest_flag, 1},
    {"help", no_argument, &help_flag, 1},
    {"delay", required_argument, NULL, 'd'},
    {"size", required_argument, NULL, 'n'},
    {"seed", required_argument, NULL, 's'},
    {"height", required_argument, NULL, 'h'},
    {"width", required_argument, NULL, 'w'},
    {"scale", required_argument, NULL, 'a'},
    {"color", required_argument, NULL, 'c'},
    {"xy", required_argument, NULL, 'x'},
    {NULL, 0, NULL, 0}};

static void usage(int argc, char *argv[]) {
    fprintf(stderr, "Usage: %s [options]\n", argv[0]);
    fprintf(stderr,
        "\n"
        "Options :\n"
        "\n"
        "  Cette aide :\n"
        "    --help\n"
        "\n"
        "  Fixe le nombre de sommets (10 par défaut) :\n"
        "    --size <int>\n"
        "\n"
        "  Sélectionne la graine du générateur aléatoire :\n"
        "    --seed <int>\n"
        "\n"

        "  Fixe la largeur/hauteur/échelle de la fenêtre :\n"
        "    --width <int> (défaut 640)\n"
        "    --height <int> (défaut 480)\n"
        "    --scale <float> (défaut 1.0)\n"
        "\n"
            "  Fixe un délai d'animation :\n"
        "    --delay <int> (défaut : pas de délai)\n"
        "\n"
        // "  Sélectionne des couleurs :\n"
        // "    --color item=R,G,B\n"
        // "      item: ground, point, line, path, root, tree\n"
        // "      R,G,B: integer valeurs in [0,256)\n"
        // "     Ex: --color ground=50,50,50\n"
        // "\n"
        "  Selectionne la méthode de génération des points :\n"
        "    --xy uniform: génération aléatoire uniforme dans la fenêtre (le défaut)\n"
        "    --xy grid=pxq: points espacés régulièrement sur une grille pxq\n"
        "    --xy convex: points aléatoires en position convexe\n"
        "    --xy power=p: points sur un disque, distribution exponentielle en rayon\n"
        "    --xy circle=k: Positionne les points sur k cercles concentriques\n"
        "                   de tailles j*min{height,width}/(2.2*k) pour j=1..k\n"
        "    --xy load=fichier: points chargés depuis un fichier. Format :\n"
        "                   n\n"
        "                   x_1 y_2\n"
        "                   ... ...\n"
        "                   x_n y_n\n"
        "\n");
}

static void parse_argv(int argc, char *argv[]) {
    int v, c;
    for (;;) {
        // lecture des options dans long_options: remplit les flags ou
        // renvoit dans c le code de l'option et dans optarg son paramètre
        c = getopt_long(argc, argv, "nshwagrcx", long_options, NULL);
        if (c == -1)
            break;  // fin de lecture des options

        if (optarg == NULL)
            continue;  // pas d'argument à l'option
        v = atoi(
            optarg);  // valeur de l'argument supposé <int> (=0 si pas nombre)
        if (v < 0)
            continue;  // on souhaite une valeur positive

        switch (c) {
        case 'n':
            n_option = v;
            break;
        case 's':
            seed_option = v;
            break;
        case 'h':
            height = v;
            break;
        case 'w':
            width = v;
            break;
        case 'd':
            animation = 1;
            animation_delay = v;
            break;
        case 'a':
            scale = atof(optarg);
            break;

        case 'c':
            // analyse de la couleur
            // ex: optarg="ground=128,13,0"
            {
                char *T[] = {"ground", "point", "line", "path", "root", "tree"};
                GLfloat *G[] = {COLOR_GROUND, COLOR_POINT, COLOR_LINE,
                                COLOR_PATH,   COLOR_ROOT,  COLOR_TREE};
                char *   p   = index(optarg, '=');
                if (p == NULL)
                    break;  // fin si pas de '='
                *p = '\0';  // coupe optarg
                int x, y, z, n = sizeof(T) / sizeof(*T);
                for (int i = 0; i < n; i++) {         // cherche l'item dans T[]
                    if (strcmp(optarg, T[i]) == 0) {  // si trouvé
                        sscanf(p + 1, "%i,%i,%i", &x, &y, &z);  // décode R,G,B
                        G[i][0] = (GLfloat)x / 255.0;
                        G[i][1] = (GLfloat)y / 255.0;
                        G[i][2] = (GLfloat)z / 255.0;
                        break;
                    }
                }
            }
            break;

        case 'x':
            // analyse des options --xy
            // ex: optarg="circle=6"
            {
                char *p = index(optarg, '=');
                if (p)
                    *p = '\0', p++;  // coupe optarg
                // ici p = argument de l'option ou p=NULL

                // option sans argument (p==NULL)
                if (strcmp(optarg, "uniforme") == 0) {
                    xy_option = XY_UNIFORM;
                    break;
                }

                // options avec argument(s)
                if (strcmp(optarg, "circle") == 0) {
                    xy_option = XY_CIRCLE;
                    sscanf(p, "%i", &xy_circle);
                    if (xy_circle <= 0)
                        xy_circle = 1;
                    break;
                }
                if (strcmp(optarg, "grid") == 0) {
                    xy_option = XY_GRID;
                    sscanf(p, "%ix%i", &xy_grid_p, &xy_grid_q);
                    if (xy_grid_p <= 0)
                        xy_grid_p = 1;
                    if (xy_grid_q <= 0)
                        xy_grid_q = 1;
                    break;
                }
                if (strcmp(optarg, "power") == 0) {
                    xy_option = XY_POWER;
                    sscanf(p, "%lf", &xy_power);
                    break;
                }
                if (strcmp(optarg, "convex") == 0) {
                    xy_option = XY_CONVEX;
                    break;
                }
                if (strcmp(optarg, "load") == 0) {
                    xy_option = XY_LOAD;
                    sscanf(p, "%s", xy_load);  // file ou "file" marchent
                    break;
                }
            }
            break;

        default:
            break;
        }
    }

    // il faut choisir un algorithme
    // sinon affichage de l'usage
    if (help_flag) {
        usage(argc, argv);
        exit(EXIT_SUCCESS);
    }
    cheapest_flag = 1;

    return;
}

static void print_SDL_version(void) {
    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("Compilé avec SDL version %d.%d.%d.\t", compiled.major,
           compiled.minor, compiled.patch);
    printf("Lié avec SDL version %d.%d.%d.\n\n", linked.major,
           linked.minor, linked.patch);
    return;
}

int main(int argc, char *argv[]) {
    print_SDL_version();
    parse_argv(argc, argv);

    int n = (n_option > 1) ? n_option : 10;

    unsigned seed = time(NULL) % 1000;
    if (seed_option >= 0)
        seed = seed_option;
    printf("Pour rejouer les mêmes tirages aléatoires : ./tp --seed %u\n", seed);  // pour rejouer la même chose au cas où
    srandom(seed);

    TopChrono(0);  // initialise tous les chronos

    point *V = NULL;  // tableau de points
    if (xy_option == XY_UNIFORM)
        V = generatePoints(n);  // uniforme
    if (xy_option == XY_CIRCLE)
        V = generateCircles(n, xy_circle);  // sur k cercles
    if (xy_option == XY_POWER)
        V = generatePower(n, xy_power);  // sur disque et loi puissance
    if (xy_option == XY_CONVEX)
        V = generateConvex(n);  // aléatoire en position convexe
    if (xy_option == XY_GRID)
        V = generateGrid(&n, xy_grid_p, xy_grid_q);  // sur grille pxq
    if (xy_option == XY_LOAD)
        V = generateLoad(&n, xy_load);  // depuis un fichier

    if (!V){
      fprintf(stderr, "Points non générés, essayez à nouveau\n");
      exit(EXIT_FAILURE);
    }

    int *P = malloc(n * sizeof(int));  // P = la tournée
    P[0]   = -1;  // permutation qui ne sera pas dessinée par drawTour()

    init_SDL_OpenGL();     // initialisation avant de dessiner
    drawTour(V, n, NULL);  // dessine seulement les points

    if (cheapest_flag) {
        running = true;  // force l'exécution
        TopChrono(1);    // départ du chrono 1
        printf("Valeur : %g\n", tsp_approximation(V, n, P));
        printf("Temps d'exécution : %s\n", TopChrono(1));  // durée
        printf("Appuyez sur une touche... ");
        fflush(stdout);
        bool new_redraw = false;
        update          = true;  // force l'affichage
        while (running) {  // affiche le résultat et attend (q pour sortir)
            if (new_redraw) {
                tsp_approximation(V, n, P);
            }

            drawTour(V, n, P);  // dessine la tournée
            new_redraw = handleEvent(update);  // attend un évènement (si affichage) ou pas
        }
        printf("\n");
    }

// Libération de la mémoire
    TopChrono(-1);
    free(V);
    free(P);

    cleaning_SDL_OpenGL();
    return 0;
}
