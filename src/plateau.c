#include "../include/plateau.h"

/*
 * Initialise toutes les cases du plateau à -1.
 * -1 signifie : aucune case n'appartient à un joueur.
 */
void initialiser_plateau(Plateau *plateau)
{
    int ligne;
    int colonne;

    for (ligne = 0; ligne < HAUTEUR_PLATEAU; ligne++)
    {
        for (colonne = 0; colonne < LARGEUR_PLATEAU; colonne++)
        {
            plateau->proprietaire[ligne][colonne] = -1;
        }
    }
}

/*
 * Marque la case actuelle du joueur.
 *
 * Cas gérés :
 * - la case lui appartient déjà → aucun changement
 * - la case est libre → +1 case
 * - la case appartient à un autre joueur :
 *      → l'ancien propriétaire perd 1 case
 *      → le joueur courant gagne 1 case
 */
void marquer_case(Plateau *plateau, Joueur joueurs[], Joueur *joueur)
{
    int ancien_proprietaire;
    int x;
    int y;

    x = joueur->x;
    y = joueur->y;

    ancien_proprietaire = plateau->proprietaire[y][x];

    /* Case déjà possédée par le joueur */
    if (ancien_proprietaire == joueur->identifiant)
    {
        return;
    }

    /* Retirer la case à l'ancien propriétaire */
    if (ancien_proprietaire != -1)
    {
        joueurs[ancien_proprietaire].cases_marquees--;
    }

    /* Attribuer la case au joueur courant */
    plateau->proprietaire[y][x] = joueur->identifiant;
    joueur->cases_marquees++;
}