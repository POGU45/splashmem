#ifndef JOUEUR_H
#define JOUEUR_H

#include "types.h"
#include "actions.h"

void initialiser_joueurs(Joueur joueurs[], int nombre_joueurs);
void appliquer_action(Joueur *joueur, Action action);
int cout_action_base(Action action);
int cout_action_joueur(Joueur *joueur, Action action);

#endif