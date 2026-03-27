#ifndef CHARGEMENT_H
#define CHARGEMENT_H

#include "types.h"

int charger_joueur(Joueur *joueur, const char *chemin_bibliotheque);
void liberer_joueur(Joueur *joueur);

#endif