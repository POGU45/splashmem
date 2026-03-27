#include <stdio.h>
#include <SDL2/SDL.h>
#include "../include/affichage_sdl.h"

#define TAILLE_CASE 10
#define LARGEUR_FENETRE (LARGEUR_PLATEAU * TAILLE_CASE)
#define HAUTEUR_FENETRE (HAUTEUR_PLATEAU * TAILLE_CASE)

static SDL_Window *fenetre = NULL;
static SDL_Renderer *rendu = NULL;
static int programme_actif = 1;

/*
 * Initialise SDL, la fenêtre et le moteur de rendu.
 */
int initialiser_sdl(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return 0;
    }

    fenetre = SDL_CreateWindow(
        "Splashmem",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR_FENETRE,
        HAUTEUR_FENETRE,
        0
    );

    if (fenetre == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    rendu = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (rendu == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        return 0;
    }

    return 1;
}

/*
 * Libère les ressources SDL.
 */
void fermer_sdl(void)
{
    if (rendu != NULL)
    {
        SDL_DestroyRenderer(rendu);
        rendu = NULL;
    }

    if (fenetre != NULL)
    {
        SDL_DestroyWindow(fenetre);
        fenetre = NULL;
    }

    SDL_Quit();
}

/*
 * Indique si la fenêtre est toujours ouverte.
 */
int fenetre_sdl_est_ouverte(void)
{
    return programme_actif;
}

/*
 * Gère les événements SDL, notamment la fermeture de fenêtre.
 */
void gerer_evenements_sdl(void)
{
    SDL_Event evenement;

    while (SDL_PollEvent(&evenement))
    {
        if (evenement.type == SDL_QUIT)
        {
            programme_actif = 0;
        }
    }
}

/*
 * Affiche le plateau et les joueurs.
 */
void afficher_plateau_sdl(Plateau *plateau, Joueur joueurs[], int nombre_joueurs)
{
    int ligne;
    int colonne;
    int indice_joueur;
    SDL_Rect rectangle;

    /* Fond noir */
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
    SDL_RenderClear(rendu);

    /* Dessin des cases possedees */
    for (ligne = 0; ligne < HAUTEUR_PLATEAU; ligne++)
    {
        for (colonne = 0; colonne < LARGEUR_PLATEAU; colonne++)
        {
            rectangle.x = colonne * TAILLE_CASE;
            rectangle.y = ligne * TAILLE_CASE;
            rectangle.w = TAILLE_CASE;
            rectangle.h = TAILLE_CASE;

            switch (plateau->proprietaire[ligne][colonne])
            {
                case 0:
                    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
                    SDL_RenderFillRect(rendu, &rectangle);
                    break;

                case 1:
                    SDL_SetRenderDrawColor(rendu, 0, 255, 0, 255);
                    SDL_RenderFillRect(rendu, &rectangle);
                    break;

                case 2:
                    SDL_SetRenderDrawColor(rendu, 0, 0, 255, 255);
                    SDL_RenderFillRect(rendu, &rectangle);
                    break;

                case 3:
                    SDL_SetRenderDrawColor(rendu, 255, 255, 0, 255);
                    SDL_RenderFillRect(rendu, &rectangle);
                    break;

                default:
                    break;
            }
        }
    }

    /* Dessin de la grille */
    SDL_SetRenderDrawColor(rendu, 40, 40, 40, 255);

    for (ligne = 0; ligne <= HAUTEUR_PLATEAU; ligne++)
    {
        SDL_RenderDrawLine(rendu,
                           0,
                           ligne * TAILLE_CASE,
                           LARGEUR_FENETRE,
                           ligne * TAILLE_CASE);
    }

    for (colonne = 0; colonne <= LARGEUR_PLATEAU; colonne++)
    {
        SDL_RenderDrawLine(rendu,
                           colonne * TAILLE_CASE,
                           0,
                           colonne * TAILLE_CASE,
                           HAUTEUR_FENETRE);
    }

    /* Dessin des joueurs */
    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        rectangle.x = joueurs[indice_joueur].x * TAILLE_CASE;
        rectangle.y = joueurs[indice_joueur].y * TAILLE_CASE;
        rectangle.w = TAILLE_CASE;
        rectangle.h = TAILLE_CASE;

        switch (indice_joueur)
        {
            case 0:
                SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
                break;

            case 1:
                SDL_SetRenderDrawColor(rendu, 255, 128, 255, 255);
                break;

            case 2:
                SDL_SetRenderDrawColor(rendu, 0, 255, 255, 255);
                break;

            case 3:
                SDL_SetRenderDrawColor(rendu, 255, 165, 0, 255);
                break;

            default:
                SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
                break;
        }

        SDL_RenderFillRect(rendu, &rectangle);
    }

    SDL_RenderPresent(rendu);
}