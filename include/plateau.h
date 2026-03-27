#ifndef PLATEAU_H
#define PLATEAU_H

#include "types.h"

void initialiser_plateau(Plateau *plateau);
void marquer_case(Plateau *plateau, Joueur joueurs[], Joueur *joueur);
void afficher_plateau_reduit(Plateau *plateau, Joueur joueurs[], int nombre_joueurs, int largeur_affichee, int hauteur_affichee);
void afficher_plateau_entier(Plateau *plateau, Joueur joueurs[], int nombre_joueurs);
#endif