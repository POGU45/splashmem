#ifndef AFFICHAGE_SDL_H
#define AFFICHAGE_SDL_H

#include "types.h"

int initialiser_sdl(void);
void fermer_sdl(void);
int fenetre_sdl_est_ouverte(void);
void gerer_evenements_sdl(void);
void afficher_plateau_sdl(Plateau *plateau, Joueur joueurs[], int nombre_joueurs, int partie_terminee);

#endif