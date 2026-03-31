#ifndef JOUEUR_H
#define JOUEUR_H

#include "types.h"
#include "actions.h"

void initialiser_joueurs(Joueur joueurs[], int nombre_joueurs);
int cout_action(Action action);
void appliquer_action(Joueur *joueur, Action action);

#endif