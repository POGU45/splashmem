#ifndef JOUEUR_H
#define JOUEUR_H

#include "types.h"

void initialiser_joueurs(Joueur joueurs[], int nombre_joueurs);
int cout_action(char action);
void appliquer_action(Joueur *joueur, char action);

#endif