#ifndef ACTIONS_H
#define ACTIONS_H

/*
 * Enumération des actions possibles pour un joueur.
 * L'ordre est IMPORTANT car certaines valeurs peuvent être utilisées implicitement.
 */
typedef enum action
{
    ACTION_STILL,       // 0

    ACTION_MOVE_L,      // 1
    ACTION_MOVE_R,      // 2
    ACTION_MOVE_U,      // 3
    ACTION_MOVE_D,      // 4

    ACTION_DASH_L,      // 5
    ACTION_DASH_R,      // 6
    ACTION_DASH_U,      // 7
    ACTION_DASH_D,      // 8

    ACTION_TELEPORT_L,  // 9
    ACTION_TELEPORT_R,  // 10
    ACTION_TELEPORT_U,  // 11
    ACTION_TELEPORT_D,  // 12

    ACTION_BOMB,         // 13
    ACTION_FORK          // 14
} Action;

#endif