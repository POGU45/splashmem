#include "../include/actions.h"

Action get_action(void)
{
    static int phase = 0;
    static int compteur_pas = 0;
    static int compteur_global = 0;

    compteur_global++;
    compteur_pas++;

    if (compteur_global % 30 == 0)
    {
        return ACTION_FORK;
    }

    switch (phase)
    {
        case 0:
            if (compteur_pas >= 20)
            {
                compteur_pas = 0;
                phase = 1;
            }
            return ACTION_MOVE_R;

        case 1:
            if (compteur_pas >= 3)
            {
                compteur_pas = 0;
                phase = 2;
            }
            return ACTION_MOVE_D;

        case 2:
            if (compteur_pas >= 20)
            {
                compteur_pas = 0;
                phase = 3;
            }
            return ACTION_MOVE_L;

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