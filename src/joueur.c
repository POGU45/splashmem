#include "../include/joueur.h"
#include "../include/actions.h"

#include <stdlib.h>

static int coordonnee_boucle(int valeur, int taille_max)
{
    // Gère l'effet "Pac-Man" :
    // si on sort d'un côté, on réapparaît de l'autre.
    while (valeur < 0)
    {
        valeur += taille_max;
    }

    while (valeur >= taille_max)
    {
        valeur -= taille_max;
    }

    return valeur;
}

void initialiser_joueurs(Joueur joueurs[], int nombre_joueurs)
{
    int i;

    for (i = 0; i < nombre_joueurs; i++)
    {
        joueurs[i].x = 10 + i;
        joueurs[i].y = 10;
        joueurs[i].credit = CREDIT_INITIAL;
        joueurs[i].identifiant = i;
        joueurs[i].cases_marquees = 0;
        joueurs[i].bibliotheque = NULL;
        joueurs[i].obtenir_action = NULL;
    }
}

int cout_action(char action)
{
    switch (action)
    {
        case ACTION_MOVE_L:
        case ACTION_MOVE_R:
        case ACTION_MOVE_U:
        case ACTION_MOVE_D:
        case ACTION_STILL:
            return 1;

        case ACTION_DASH_L:
        case ACTION_DASH_R:
        case ACTION_DASH_U:
        case ACTION_DASH_D:
            return 10;

        case ACTION_TELEPORT_L:
        case ACTION_TELEPORT_R:
        case ACTION_TELEPORT_U:
        case ACTION_TELEPORT_D:
            return 2;

        default:
            return 1;
    }
}

void appliquer_action(Joueur *joueur, char action)
{
    int deplacement = 0;

    switch (action)
    {
        case ACTION_MOVE_L:
            joueur->x = coordonnee_boucle(joueur->x - 1, LARGEUR_PLATEAU);
            break;

        case ACTION_MOVE_R:
            joueur->x = coordonnee_boucle(joueur->x + 1, LARGEUR_PLATEAU);
            break;

        case ACTION_MOVE_U:
            joueur->y = coordonnee_boucle(joueur->y - 1, HAUTEUR_PLATEAU);
            break;

        case ACTION_MOVE_D:
            joueur->y = coordonnee_boucle(joueur->y + 1, HAUTEUR_PLATEAU);
            break;

        case ACTION_DASH_L:
            deplacement = 8;
            joueur->x = coordonnee_boucle(joueur->x - deplacement, LARGEUR_PLATEAU);
            break;

        case ACTION_DASH_R:
            deplacement = 8;
            joueur->x = coordonnee_boucle(joueur->x + deplacement, LARGEUR_PLATEAU);
            break;

        case ACTION_DASH_U:
            deplacement = 8;
            joueur->y = coordonnee_boucle(joueur->y - deplacement, HAUTEUR_PLATEAU);
            break;

        case ACTION_DASH_D:
            deplacement = 8;
            joueur->y = coordonnee_boucle(joueur->y + deplacement, HAUTEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_L:
            deplacement = 8;
            joueur->x = coordonnee_boucle(joueur->x - deplacement, LARGEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_R:
            deplacement = 8;
            joueur->x = coordonnee_boucle(joueur->x + deplacement, LARGEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_U:
            deplacement = 8;
            joueur->y = coordonnee_boucle(joueur->y - deplacement, HAUTEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_D:
            deplacement = 8;
            joueur->y = coordonnee_boucle(joueur->y + deplacement, HAUTEUR_PLATEAU);
            break;

        case ACTION_STILL:
        default:
            break;
    }
}