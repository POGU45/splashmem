#include "../include/actions.h"

char get_action(void)
{
    static int compteur = 0;

    compteur++;

    if (compteur % 7 == 0)
        return ACTION_TELEPORT_R;

    if (compteur % 11 == 0)
        return ACTION_TELEPORT_U;

    if (compteur % 3 == 0)
        return ACTION_MOVE_R;

    if (compteur % 5 == 0)
        return ACTION_MOVE_D;

    return ACTION_MOVE_L;
}