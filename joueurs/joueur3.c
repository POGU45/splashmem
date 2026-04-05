#include "../include/actions.h"

Action get_action(void)
{
    static int compteur = 0;

    compteur++;

    if (compteur % 40 == 0)
    {
        return ACTION_MUTE;
    }

    if (compteur % 20 == 0)
    {
        return ACTION_BOMB;
    }

    if (compteur % 7 == 0)
    {
        return ACTION_TELEPORT_R;
    }

    if (compteur % 3 == 0)
    {
        return ACTION_MOVE_R;
    }

    if (compteur % 5 == 0)
    {
        return ACTION_MOVE_D;
    }

    return ACTION_MOVE_L;
}