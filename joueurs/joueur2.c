#include "../include/actions.h"

char get_action(void)
{
    static int phase = 0;
    static int compteur = 0;

    compteur++;

    if (phase == 0)
    {
        if (compteur % 10 == 0)
            return ACTION_DASH_R;

        if (compteur > 20)
        {
            compteur = 0;
            phase = 1;
        }

        return ACTION_MOVE_R;
    }

    if (phase == 1)
    {
        if (compteur > 3)
        {
            compteur = 0;
            phase = 2;
        }
        return ACTION_MOVE_D;
    }

    if (phase == 2)
    {
        if (compteur % 10 == 0)
            return ACTION_DASH_L;

        if (compteur > 20)
        {
            compteur = 0;
            phase = 3;
        }

        return ACTION_MOVE_L;
    }

    if (phase == 3)
    {
        if (compteur > 3)
        {
            compteur = 0;
            phase = 0;
        }
        return ACTION_MOVE_D;
    }

    return ACTION_MOVE_R;
}