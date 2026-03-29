#ifndef PLATEAU_H
#define PLATEAU_H

#include "types.h"

/*
 * Initialise le plateau :
 * toutes les cases sont libres (-1).
 */
void initialiser_plateau(Plateau *plateau);

/*
 * Marque la case occupée par un joueur.
 * Gère le changement de propriétaire et les scores.
 */
void marquer_case(Plateau *plateau, Joueur joueurs[], Joueur *joueur);

#endif