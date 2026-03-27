#include "../include/actions.h"
#define NOMBRE_ACTIONS 4

char actions_joueur[NOMBRE_ACTIONS] =
{
    ACTION_MOVE_D,
    ACTION_MOVE_R,
    ACTION_MOVE_U,
    ACTION_MOVE_L
};

char get_action(void)
{
    static int indice = 0;
    char action_courante;

    action_courante = actions_joueur[indice];
    indice++;

    if (indice >= NOMBRE_ACTIONS)
    {
        indice = 0;
    }

    return action_courante;
}