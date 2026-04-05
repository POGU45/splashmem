#ifndef FORK_H
#define FORK_H

#include "types.h"

void initialiser_forks(Fork forks[], int nombre_max_forks);
int creer_fork(Fork forks[], int nombre_max_forks, Joueur *joueur);
void enregistrer_action_joueur(Joueur *joueur, Action action);
void mettre_a_jour_forks(Fork forks[], int nombre_max_forks, Plateau *plateau, Joueur joueurs[]);

#endif