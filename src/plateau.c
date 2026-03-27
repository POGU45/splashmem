#include <stdio.h>
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
 * Cas possibles :
 * - la case lui appartient déjà : aucun changement
 * - la case est libre : le joueur gagne 1 case
 * - la case appartient à un autre joueur :
 *     l'ancien propriétaire perd 1 case
 *     le joueur courant gagne 1 case
 *     la propriété est mise à jour
 */
void marquer_case(Plateau *plateau, Joueur joueurs[], Joueur *joueur)
{
    int ancien_proprietaire;
    int position_x;
    int position_y;

    position_x = joueur->x;
    position_y = joueur->y;

    ancien_proprietaire = plateau->proprietaire[position_y][position_x];

    if (ancien_proprietaire == joueur->identifiant)
    {
        return;
    }

    if (ancien_proprietaire != -1)
    {
        joueurs[ancien_proprietaire].cases_marquees--;
    }

    plateau->proprietaire[position_y][position_x] = joueur->identifiant;
    joueur->cases_marquees++;
}

/*
 * Affiche la totalité du plateau.
 *
 * Symboles :
 * - '.' : case libre
 * - '1', '2', '3', '4' : case possédée
 * - 'A', 'B', 'C', 'D' : position actuelle des joueurs
 */
void afficher_plateau_entier(Plateau *plateau, Joueur joueurs[], int nombre_joueurs)
{
    int ligne;
    int colonne;
    int indice_joueur;
    int joueur_trouve;
    int proprietaire_case;

    printf("\n=== PLATEAU COMPLET %dx%d ===\n", LARGEUR_PLATEAU, HAUTEUR_PLATEAU);
    printf("A/B/C/D : position actuelle des joueurs\n");
    printf("1/2/3/4 : cases possedees\n");
    printf(". : case libre\n\n");

    printf("     ");
    for (colonne = 0; colonne < LARGEUR_PLATEAU; colonne++)
    {
        printf("%2d ", colonne);
    }
    printf("\n");

    for (ligne = 0; ligne < HAUTEUR_PLATEAU; ligne++)
    {
        printf("%3d  ", ligne);

        for (colonne = 0; colonne < LARGEUR_PLATEAU; colonne++)
        {
            joueur_trouve = 0;

            for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
            {
                if (joueurs[indice_joueur].x == colonne && joueurs[indice_joueur].y == ligne)
                {
                    printf("%2c ", 'A' + indice_joueur);
                    joueur_trouve = 1;
                    break;
                }
            }

            if (joueur_trouve)
            {
                continue;
            }

            proprietaire_case = plateau->proprietaire[ligne][colonne];

            if (proprietaire_case == -1)
            {
                printf("%2c ", '.');
            }
            else
            {
                printf("%2d ", proprietaire_case + 1);
            }
        }

        printf("\n");
    }

    printf("\n");
}

void afficher_plateau_reduit(Plateau *plateau, Joueur joueurs[], int nombre_joueurs, int largeur_affichee, int hauteur_affichee)
{
    int ligne;
    int colonne;
    int indice_joueur;
    int joueur_trouve;
    int proprietaire_case;

    printf("\n=== APERCU DU PLATEAU ===\n");
    printf("A/B/C/D : position actuelle des joueurs\n");
    printf("1/2/3/4 : cases possedees\n");
    printf(". : case libre\n\n");

    printf("    ");
    for (colonne = 0; colonne < largeur_affichee; colonne++)
    {
        printf("%2d ", colonne);
    }
    printf("\n");

    for (ligne = 0; ligne < hauteur_affichee; ligne++)
    {
        printf("%2d  ", ligne);

        for (colonne = 0; colonne < largeur_affichee; colonne++)
        {
            joueur_trouve = 0;

            for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
            {
                if (joueurs[indice_joueur].x == colonne && joueurs[indice_joueur].y == ligne)
                {
                    printf("%2c ", 'A' + indice_joueur);
                    joueur_trouve = 1;
                    break;
                }
            }

            if (joueur_trouve)
            {
                continue;
            }

            proprietaire_case = plateau->proprietaire[ligne][colonne];

            if (proprietaire_case == -1)
            {
                printf("%2c ", '.');
            }
            else
            {
                printf("%2d ", proprietaire_case + 1);
            }
        }

        printf("\n");
    }

    printf("\n");
}