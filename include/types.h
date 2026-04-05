#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <dlfcn.h>
#include "actions.h"

#define LARGEUR_PLATEAU 100
#define HAUTEUR_PLATEAU 100
#define NOMBRE_MAX_JOUEURS 4
#define CREDIT_INITIAL 900

#define NOMBRE_MAX_BOMBES 100
#define NOMBRE_MAX_FORKS 16

#define DUREE_ATTENTE_FORK 5
#define DUREE_CLONE_FORK 20
#define TAILLE_HISTORIQUE_ACTIONS 64

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

    Action historique_actions[TAILLE_HISTORIQUE_ACTIONS];
    int nombre_actions_historique;

    int fork_actif;
    int tours_fork_restants;
    int est_muet;
    int tours_mute_restants;
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

typedef struct
{
    int actif;
    int clone_demarre;

    int proprietaire_original;

    int x_depart;
    int y_depart;

    int x_clone;
    int y_clone;

    int tours_avant_depart;
    int duree_restante;

    int indice_action_historique_depart;
} Fork;

#endif