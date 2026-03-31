#include "../include/bombe.h"
#include "../include/plateau.h"

/*
 * Initialise toutes les bombes comme inactives.
 */
void initialiser_bombes(Bombe bombes[], int nombre_max_bombes)
{
    int indice_bombe;

    for (indice_bombe = 0; indice_bombe < nombre_max_bombes; indice_bombe++)
    {
        bombes[indice_bombe].active = 0;
        bombes[indice_bombe].x = 0;
        bombes[indice_bombe].y = 0;
        bombes[indice_bombe].proprietaire = -1;
        bombes[indice_bombe].tours_restants = 0;
    }
}

/*
 * Pose une bombe à la position actuelle du joueur.
 * Retourne 1 si la bombe a été posée, 0 sinon.
 */
int poser_bombe(Bombe bombes[], int nombre_max_bombes, Joueur *joueur)
{
    int indice_bombe;

    for (indice_bombe = 0; indice_bombe < nombre_max_bombes; indice_bombe++)
    {
        if (bombes[indice_bombe].active == 0)
        {
            bombes[indice_bombe].active = 1;
            bombes[indice_bombe].x = joueur->x;
            bombes[indice_bombe].y = joueur->y;
            bombes[indice_bombe].proprietaire = joueur->identifiant;
            bombes[indice_bombe].tours_restants = 5;
            return 1;
        }
    }

    return 0;
}

/*
 * Déclenche une bombe :
 * elle marque un carré 3x3 centré sur sa position.
 */
static void declencher_bombe(Bombe *bombe, Plateau *plateau, Joueur joueurs[])
{
    int delta_x;
    int delta_y;
    int x_case;
    int y_case;
    Joueur joueur_fictif;

    joueur_fictif.identifiant = bombe->proprietaire;
    joueur_fictif.x = 0;
    joueur_fictif.y = 0;
    joueur_fictif.credit = 0;
    joueur_fictif.cases_marquees = 0;
    joueur_fictif.bibliotheque = NULL;
    joueur_fictif.obtenir_action = NULL;

    for (delta_y = -1; delta_y <= 1; delta_y++)
    {
        for (delta_x = -1; delta_x <= 1; delta_x++)
        {
            x_case = bombe->x + delta_x;
            y_case = bombe->y + delta_y;

            while (x_case < 0)
            {
                x_case += LARGEUR_PLATEAU;
            }

            while (x_case >= LARGEUR_PLATEAU)
            {
                x_case -= LARGEUR_PLATEAU;
            }

            while (y_case < 0)
            {
                y_case += HAUTEUR_PLATEAU;
            }

            while (y_case >= HAUTEUR_PLATEAU)
            {
                y_case -= HAUTEUR_PLATEAU;
            }

            joueur_fictif.x = x_case;
            joueur_fictif.y = y_case;

            marquer_case(plateau, joueurs, &joueur_fictif);
        }
    }
}

/*
 * Décrémente les bombes et déclenche celles arrivées à 0.
 */
void mettre_a_jour_bombes(Bombe bombes[], int nombre_max_bombes, Plateau *plateau, Joueur joueurs[])
{
    int indice_bombe;

    for (indice_bombe = 0; indice_bombe < nombre_max_bombes; indice_bombe++)
    {
        if (bombes[indice_bombe].active)
        {
            bombes[indice_bombe].tours_restants--;

            if (bombes[indice_bombe].tours_restants <= 0)
            {
                declencher_bombe(&bombes[indice_bombe], plateau, joueurs);
                bombes[indice_bombe].active = 0;
            }
        }
    }
}