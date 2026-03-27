#include "../include/actions.h"

char get_action(void)
{
    static int phase = 0;
    static int compteur = 0;
    static int sens = 1;

    compteur++;

    if (compteur % 15 == 0)
    {
        sens = -sens;
    }

    switch (phase)
    {
        case 0:
            if (compteur > 8)
            {
                compteur = 0;
                phase = 1;
            }
            return (sens == 1) ? ACTION_MOVE_R : ACTION_MOVE_L;

        case 1:
            if (compteur > 4)
            {
                compteur = 0;
                phase = 2;
            }
            return ACTION_MOVE_D;

        case 2:
            if (compteur > 8)
            {
                compteur = 0;
                phase = 3;
            }
            return (sens == 1) ? ACTION_MOVE_L : ACTION_MOVE_R;

        case 3:
            if (compteur > 4)
            {
                compteur = 0;
                phase = 0;
            }
            return ACTION_MOVE_U;
    }

    return ACTION_MOVE_R;
}