#include "../include/joueur.h"
#include "../include/actions.h"
#include <stddef.h>

static int coordonnee_boucle(int valeur, int taille_max)
{
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
    int indice_joueur;
    int espacement;

    espacement = LARGEUR_PLATEAU / nombre_joueurs;

    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        joueurs[indice_joueur].x = indice_joueur * espacement;
        joueurs[indice_joueur].y = HAUTEUR_PLATEAU / 2;
        joueurs[indice_joueur].credit = CREDIT_INITIAL;
        joueurs[indice_joueur].identifiant = indice_joueur;
        joueurs[indice_joueur].cases_marquees = 0;
        joueurs[indice_joueur].bibliotheque = NULL;
        joueurs[indice_joueur].obtenir_action = NULL;
    }
}

int cout_action(Action action)
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

        case ACTION_BOMB:
            return 5;

        default:
            return 1;
    }
}

void appliquer_action(Joueur *joueur, Action action)
{
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
            joueur->x = coordonnee_boucle(joueur->x - 8, LARGEUR_PLATEAU);
            break;

        case ACTION_DASH_R:
            joueur->x = coordonnee_boucle(joueur->x + 8, LARGEUR_PLATEAU);
            break;

        case ACTION_DASH_U:
            joueur->y = coordonnee_boucle(joueur->y - 8, HAUTEUR_PLATEAU);
            break;

        case ACTION_DASH_D:
            joueur->y = coordonnee_boucle(joueur->y + 8, HAUTEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_L:
            joueur->x = coordonnee_boucle(joueur->x - 8, LARGEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_R:
            joueur->x = coordonnee_boucle(joueur->x + 8, LARGEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_U:
            joueur->y = coordonnee_boucle(joueur->y - 8, HAUTEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_D:
            joueur->y = coordonnee_boucle(joueur->y + 8, HAUTEUR_PLATEAU);
            break;

        case ACTION_BOMB:
        case ACTION_STILL:
        default:
            break;
    }
}