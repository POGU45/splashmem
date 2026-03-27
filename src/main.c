#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../include/affichage_sdl.h"
#include "../include/types.h"
#include "../include/plateau.h"
#include "../include/joueur.h"
#include "../include/chargement.h"

#define AFFICHAGE_PLATEAU_COMPLET 1
#define AFFICHAGE_DEBUG 0
#define AFFICHAGE_PLATEAU 0
#define LARGEUR_AFFICHEE 20
#define HAUTEUR_AFFICHEE 12

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

static void afficher_resultats(Joueur joueurs[], int nombre_joueurs)
{
    int indice_joueur;

    printf("\n=== RESULTATS FINAUX ===\n");

    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        printf("Joueur %d : %d cases marquees | credit restant : %d | position finale : (%d,%d)\n",
               joueurs[indice_joueur].identifiant + 1,
               joueurs[indice_joueur].cases_marquees,
               joueurs[indice_joueur].credit,
               joueurs[indice_joueur].x,
               joueurs[indice_joueur].y);
    }
}

int main(int argc, char *argv[])
{
    Plateau plateau;
    Joueur joueurs[NOMBRE_MAX_JOUEURS];
    int nombre_joueurs;
    int indice_joueur;
    //int numero_tour;
    char action;
    int cout;

    if (argc < 2 || argc > 5)
    {
        fprintf(stderr, "Usage : %s joueur1.so [joueur2.so joueur3.so joueur4.so]\n", argv[0]);
        return 1;
    }

    nombre_joueurs = argc - 1;
    //numero_tour = 1;

    initialiser_plateau(&plateau);
    initialiser_joueurs(joueurs, nombre_joueurs);

    if (!initialiser_sdl())
    {
        return 1;
    }

afficher_plateau_sdl(&plateau, joueurs, nombre_joueurs);
SDL_Delay(500);

    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        if (!charger_joueur(&joueurs[indice_joueur], argv[indice_joueur + 1]))
        {
            fprintf(stderr, "Impossible de charger le joueur %d\n", indice_joueur + 1);
            return 1;
        }

        marquer_case(&plateau, joueurs, &joueurs[indice_joueur]);
    }

    if (AFFICHAGE_PLATEAU_COMPLET)
    {
    afficher_plateau_entier(&plateau, joueurs, nombre_joueurs);
    }

    if (AFFICHAGE_PLATEAU)
    {
        printf("Tour initial\n");
        afficher_plateau_reduit(&plateau, joueurs, nombre_joueurs, LARGEUR_AFFICHEE, HAUTEUR_AFFICHEE);
    }

    while (!tous_les_joueurs_sont_epuises(joueurs, nombre_joueurs) && fenetre_sdl_est_ouverte())
{
    gerer_evenements_sdl();

    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        if (joueurs[indice_joueur].credit <= 0)
        {
            continue;
        }

        action = joueurs[indice_joueur].obtenir_action();
        cout = cout_action(action);

        if (joueurs[indice_joueur].credit < cout)
        {
            joueurs[indice_joueur].credit = 0;
            continue;
        }

        joueurs[indice_joueur].credit -= cout;
        appliquer_action(&joueurs[indice_joueur], action);
        marquer_case(&plateau, joueurs, &joueurs[indice_joueur]);
    }

    afficher_plateau_sdl(&plateau, joueurs, nombre_joueurs);
    SDL_Delay(300);
}

    afficher_resultats(joueurs, nombre_joueurs);

    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        liberer_joueur(&joueurs[indice_joueur]);
    }

    return 0;
    fermer_sdl();
}

