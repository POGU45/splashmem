#include "../include/actions.h"

char get_action(void)
{
    static int phase = 0;
    static int compteur = 0;
    static int longueur = 4;
    static int dir = 0;

    compteur++;

    /* Phase 1 : spirale */
    if (phase == 0)
    {
        switch (dir)
        {
            case 0: if (compteur < longueur) return ACTION_MOVE_R; break;
            case 1: if (compteur < longueur) return ACTION_MOVE_D; break;
            case 2: if (compteur < longueur) return ACTION_MOVE_L; break;
            case 3: if (compteur < longueur) return ACTION_MOVE_U; break;
        }

        compteur = 0;
        dir = (dir + 1) % 4;

        if (dir % 2 == 0)
            longueur++;

        if (longueur > 10)
            phase = 1;
    }

    /* Phase 2 : dash horizontal */
    if (phase == 1)
    {
        if (compteur % 5 == 0)
            return ACTION_DASH_R;

        return ACTION_MOVE_R;
    }

    return ACTION_MOVE_R;
}