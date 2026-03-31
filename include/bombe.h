#ifndef BOMBE_H
#define BOMBE_H

#include "types.h"

void initialiser_bombes(Bombe bombes[], int nombre_max_bombes);
int poser_bombe(Bombe bombes[], int nombre_max_bombes, Joueur *joueur);
void mettre_a_jour_bombes(Bombe bombes[], int nombre_max_bombes, Plateau *plateau, Joueur joueurs[]);

#endif