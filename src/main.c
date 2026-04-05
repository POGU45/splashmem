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

static int distance_absolue(int a)
{
    if (a < 0)
    {
        return -a;
    }

    return a;
}

static int distance_torique_1d(int a, int b, int taille)
{
    int distance_directe;
    int distance_boucle;

    distance_directe = distance_absolue(a - b);
    distance_boucle = taille - distance_directe;

    if (distance_boucle < distance_directe)
    {
        return distance_boucle;
    }

    return distance_directe;
}

static int distance_joueurs(Joueur *joueur_a, Joueur *joueur_b)
{
    int dx;
    int dy;

    dx = distance_torique_1d(joueur_a->x, joueur_b->x, LARGEUR_PLATEAU);
    dy = distance_torique_1d(joueur_a->y, joueur_b->y, HAUTEUR_PLATEAU);

    return dx + dy;
}

static int trouver_joueur_le_plus_proche(Joueur joueurs[], int nombre_joueurs, int indice_source)
{
    int indice_joueur;
    int meilleur_indice;
    int meilleure_distance;
    int distance_courante;

    meilleur_indice = -1;
    meilleure_distance = 1000000;

    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        if (indice_joueur == indice_source)
        {
            continue;
        }

        distance_courante = distance_joueurs(&joueurs[indice_source], &joueurs[indice_joueur]);

        if (distance_courante < meilleure_distance)
        {
            meilleure_distance = distance_courante;
            meilleur_indice = indice_joueur;
        }
    }

    return meilleur_indice;
}

static void appliquer_mute(Joueur joueurs[], int nombre_joueurs, int indice_source)
{
    int indice_cible;

    indice_cible = trouver_joueur_le_plus_proche(joueurs, nombre_joueurs, indice_source);

    if (indice_cible != -1)
    {
        joueurs[indice_cible].est_muet = 1;
        joueurs[indice_cible].tours_mute_restants = 10;
    }
}

static void mettre_a_jour_mutes(Joueur joueurs[], int nombre_joueurs)
{
    int indice_joueur;

    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        if (joueurs[indice_joueur].est_muet)
        {
            joueurs[indice_joueur].tours_mute_restants--;

            if (joueurs[indice_joueur].tours_mute_restants <= 0)
            {
                joueurs[indice_joueur].est_muet = 0;
                joueurs[indice_joueur].tours_mute_restants = 0;
            }
        }
    }
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
                    else if (action == ACTION_CLEAN)
                    {
                        nettoyer_zone(&plateau, joueurs, joueurs[indice_joueur].x, joueurs[indice_joueur].y, 3);
                        marquer_case(&plateau, joueurs, &joueurs[indice_joueur]);
                    }
                    else if (action == ACTION_MUTE)
                    {
                        appliquer_mute(joueurs, nombre_joueurs, indice_joueur);

                        if (!joueurs[indice_joueur].est_muet)
                        {
                            marquer_case(&plateau, joueurs, &joueurs[indice_joueur]);
                        }
                    }
                    else
                    {
                        appliquer_action(&joueurs[indice_joueur], action);

                        if (!joueurs[indice_joueur].est_muet)
                        {
                            marquer_case(&plateau, joueurs, &joueurs[indice_joueur]);
                        }
                    }

                    mettre_a_jour_bombes(bombes, NOMBRE_MAX_BOMBES, &plateau, joueurs);
                    mettre_a_jour_forks(forks, NOMBRE_MAX_FORKS, &plateau, joueurs);
                    mettre_a_jour_mutes(joueurs, nombre_joueurs);
                }
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