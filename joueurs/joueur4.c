#include "../include/actions.h"

/*
 * Joueur 4 :
 * rectangles évolutifs avec décalage.
 *
 * Objectif :
 * éviter de tourner toujours sur le même carré.
 *
 * Fonctionnement :
 * - dessine un rectangle
 * - augmente un peu sa taille
 * - effectue un décalage
 * - recommence
 *
 * Cela permet de couvrir une zone de plus en plus large.
 */

Action get_action(void)
{
    static int phase = 0;
    static int compteur_pas = 0;
    static int largeur_rectangle = 6;
    static int hauteur_rectangle = 4;
    static int nombre_rectangles = 0;

    compteur_pas++;

    switch (phase)
    {
        /* Côté haut vers la droite */
        case 0:
            if (compteur_pas >= largeur_rectangle)
            {
                compteur_pas = 0;
                phase = 1;
            }
            return ACTION_MOVE_R;

        /* Côté droit vers le bas */
        case 1:
            if (compteur_pas >= hauteur_rectangle)
            {
                compteur_pas = 0;
                phase = 2;
            }
            return ACTION_MOVE_D;

        /* Côté bas vers la gauche */
        case 2:
            if (compteur_pas >= largeur_rectangle)
            {
                compteur_pas = 0;
                phase = 3;
            }
            return ACTION_MOVE_L;

        /* Côté gauche vers le haut */
        case 3:
            if (compteur_pas >= hauteur_rectangle)
            {
                compteur_pas = 0;
                phase = 4;
            }
            return ACTION_MOVE_U;

        /* Décalage vers la droite pour ne pas repasser au même endroit */
        case 4:
            if (compteur_pas >= 3)
            {
                compteur_pas = 0;
                phase = 5;
            }
            return ACTION_MOVE_R;

        /* Décalage vers le bas */
        case 5:
            if (compteur_pas >= 2)
            {
                compteur_pas = 0;
                phase = 0;

                nombre_rectangles++;

                largeur_rectangle += 2;
                hauteur_rectangle += 1;

                if (largeur_rectangle > 14)
                {
                    largeur_rectangle = 6;
                }

                if (hauteur_rectangle > 8)
                {
                    hauteur_rectangle = 4;
                }
            }
            return ACTION_MOVE_D;

        default:
            return ACTION_STILL;
    }
}