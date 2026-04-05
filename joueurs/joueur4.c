#include "../include/actions.h"

Action get_action(void)
{
    static int compteur = 0;
    compteur++;

    if (compteur % 35 == 0)
    {
        return ACTION_CLEAN;
    }

    if (compteur % 18 == 0)
    {
        return ACTION_BOMB;
    }

    if (compteur % 4 == 0)
    {
        return ACTION_MOVE_D;
    }

    return ACTION_MOVE_R;
}