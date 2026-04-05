#include <stdio.h>
#include <SDL2/SDL.h>

#include "../include/affichage_sdl.h"
#include "../include/types.h"
#include "../include/actions.h"
#include "../include/plateau.h"
#include "../include/joueur.h"
#include "../include/chargement.h"
#include "../include/bombe.h"
#include "../include/fork.h"

static int tous_les_joueurs_sont_epuises(Joueur joueurs[], int nombre_joueurs)
{
    int indice_joueur;

    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        if (joueurs[indice_joueur].credit > 0)
        {
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[])
{
    Plateau plateau;
    Joueur joueurs[NOMBRE_MAX_JOUEURS];
    Bombe bombes[NOMBRE_MAX_BOMBES];
    Fork forks[NOMBRE_MAX_FORKS];

    int nombre_joueurs;
    int indice_joueur;
    int partie_terminee;
    Action action;
    int cout;

    if (argc < 2 || argc > 5)
    {
        fprintf(stderr, "Usage : %s joueur1.so [joueur2.so joueur3.so joueur4.so]\n", argv[0]);
        return 1;
    }

    nombre_joueurs = argc - 1;
    partie_terminee = 0;

    initialiser_plateau(&plateau);
    initialiser_joueurs(joueurs, nombre_joueurs);
    initialiser_bombes(bombes, NOMBRE_MAX_BOMBES);
    initialiser_forks(forks, NOMBRE_MAX_FORKS);

    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        if (!charger_joueur(&joueurs[indice_joueur], argv[indice_joueur + 1]))
        {
            fprintf(stderr, "Impossible de charger le joueur %d\n", indice_joueur + 1);
            return 1;
        }

        marquer_case(&plateau, joueurs, &joueurs[indice_joueur]);
    }

    if (!initialiser_sdl())
    {
        for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
        {
            liberer_joueur(&joueurs[indice_joueur]);
        }

        return 1;
    }

    while (fenetre_sdl_est_ouverte())
    {
        gerer_evenements_sdl();

        if (!partie_terminee)
        {
            if (tous_les_joueurs_sont_epuises(joueurs, nombre_joueurs))
            {
                partie_terminee = 1;
            }
            else
            {
                for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
                {
                    if (joueurs[indice_joueur].credit <= 0)
                    {
                        continue;
                    }

                    action = joueurs[indice_joueur].obtenir_action();
                    enregistrer_action_joueur(&joueurs[indice_joueur], action);

                    cout = cout_action_joueur(&joueurs[indice_joueur], action);

                    if (joueurs[indice_joueur].credit < cout)
                    {
                        joueurs[indice_joueur].credit = 0;
                        continue;
                    }

                    joueurs[indice_joueur].credit -= cout;

                    if (action == ACTION_BOMB)
                    {
                        poser_bombe(bombes, NOMBRE_MAX_BOMBES, &joueurs[indice_joueur]);
                        marquer_case(&plateau, joueurs, &joueurs[indice_joueur]);
                    }
                    else if (action == ACTION_FORK)
                    {
                        creer_fork(forks, NOMBRE_MAX_FORKS, &joueurs[indice_joueur]);
                        marquer_case(&plateau, joueurs, &joueurs[indice_joueur]);
                    }
                    else
                    {
                        appliquer_action(&joueurs[indice_joueur], action);
                        marquer_case(&plateau, joueurs, &joueurs[indice_joueur]);
                    }
                }

                mettre_a_jour_bombes(bombes, NOMBRE_MAX_BOMBES, &plateau, joueurs);
                mettre_a_jour_forks(forks, NOMBRE_MAX_FORKS, &plateau, joueurs);
            }
        }

        afficher_plateau_sdl(&plateau, joueurs, nombre_joueurs, forks, NOMBRE_MAX_FORKS, partie_terminee);
        SDL_Delay(100);
    }

    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        liberer_joueur(&joueurs[indice_joueur]);
    }

    fermer_sdl();

    return 0;
}