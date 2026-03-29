#include "../include/actions.h"

/*
 * Joueur 1 :
 * balayage horizontal en serpentin.
 *
 * Objectif :
 * couvrir une grande surface sans rester bloqué
 * dans une seule ligne droite.
 *
 * Fonctionnement :
 * - grande phase vers la droite
 * - petite descente
 * - grande phase vers la gauche
 * - petite descente
 * - répétition
 *
 * Quelques dash sont insérés pour accélérer l'expansion.
 */

char get_action(void)
{
    static int phase = 0;
    static int compteur_pas = 0;
    static int compteur_global = 0;

    compteur_global++;
    compteur_pas++;

    switch (phase)
    {
        /* Balayage vers la droite */
        case 0:
            if (compteur_pas % 12 == 0)
            {
                if (compteur_pas >= 24)
                {
                    compteur_pas = 0;
                    phase = 1;
                }
                return ACTION_DASH_R;
            }

            if (compteur_pas >= 24)
            {
                compteur_pas = 0;
                phase = 1;
            }
            return ACTION_MOVE_R;

        /* Petite descente */
        case 1:
            if (compteur_pas >= 3)
            {
                compteur_pas = 0;
                phase = 2;
            }
            return ACTION_MOVE_D;

        /* Balayage vers la gauche */
        case 2:
            if (compteur_pas % 12 == 0)
            {
                if (compteur_pas >= 24)
                {
                    compteur_pas = 0;
                    phase = 3;
                }
                return ACTION_DASH_L;
            }

            if (compteur_pas >= 24)
            {
                compteur_pas = 0;
                phase = 3;
            }
            return ACTION_MOVE_L;

        /* Petite descente */
        case 3:
            if (compteur_pas >= 3)
            {
                compteur_pas = 0;
                phase = 0;
            }
            return ACTION_MOVE_D;

        default:
            return ACTION_STILL;
    }
}