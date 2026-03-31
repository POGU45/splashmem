#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <dlfcn.h>
#include "actions.h"

#define LARGEUR_PLATEAU 100
#define HAUTEUR_PLATEAU 100
#define NOMBRE_MAX_JOUEURS 4
#define CREDIT_INITIAL 200
#define NOMBRE_MAX_BOMBES 100

typedef Action (*fonction_action_t)(void);

typedef struct
{
    int x;
    int y;
    int credit;
    int identifiant;
    int cases_marquees;

    void *bibliotheque;
    fonction_action_t obtenir_action;
} Joueur;

typedef struct
{
    int proprietaire[HAUTEUR_PLATEAU][LARGEUR_PLATEAU];
} Plateau;

typedef struct
{
    int active;
    int x;
    int y;
    int proprietaire;
    int tours_restants;
} Bombe;

#endif