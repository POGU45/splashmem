#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../include/affichage_sdl.h"

#define TAILLE_CASE 10
#define LARGEUR_FENETRE (LARGEUR_PLATEAU * TAILLE_CASE)
#define HAUTEUR_FENETRE (HAUTEUR_PLATEAU * TAILLE_CASE)

static SDL_Window *fenetre = NULL;
static SDL_Renderer *rendu = NULL;
static TTF_Font *police = NULL;
static int programme_actif = 1;

/*
 * Dessine un texte dans la fenêtre SDL.
 */
static void dessiner_texte(const char *texte, int position_x, int position_y, Uint8 rouge, Uint8 vert, Uint8 bleu)
{
    SDL_Color couleur = {rouge, vert, bleu, 255};
    SDL_Surface *surface_texte;
    SDL_Texture *texture_texte;
    SDL_Rect rectangle_texte;

    surface_texte = TTF_RenderText_Blended(police, texte, couleur);
    if (surface_texte == NULL)
    {
        return;
    }

    texture_texte = SDL_CreateTextureFromSurface(rendu, surface_texte);
    if (texture_texte == NULL)
    {
        SDL_FreeSurface(surface_texte);
        return;
    }

    rectangle_texte.x = position_x;
    rectangle_texte.y = position_y;
    rectangle_texte.w = surface_texte->w;
    rectangle_texte.h = surface_texte->h;

    SDL_RenderCopy(rendu, texture_texte, NULL, &rectangle_texte);

    SDL_DestroyTexture(texture_texte);
    SDL_FreeSurface(surface_texte);
}

/*
 * Initialise SDL et SDL_ttf.
 */
int initialiser_sdl(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() != 0)
    {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
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
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    rendu = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (rendu == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    police = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 16);
    if (police == NULL)
    {
        fprintf(stderr, "Erreur TTF_OpenFont : %s\n", TTF_GetError());
        SDL_DestroyRenderer(rendu);
        SDL_DestroyWindow(fenetre);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    programme_actif = 1;
    return 1;
}

/*
 * Libère les ressources SDL.
 */
void fermer_sdl(void)
{
    if (police != NULL)
    {
        TTF_CloseFont(police);
        police = NULL;
    }

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

    TTF_Quit();
    SDL_Quit();
}

/*
 * Retourne 1 si la fenêtre est encore ouverte.
 */
int fenetre_sdl_est_ouverte(void)
{
    return programme_actif;
}

/*
 * Gère les événements SDL.
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

static void dessiner_texte_centre(const char *texte, int position_y, int taille_police, Uint8 rouge, Uint8 vert, Uint8 bleu)
{
    TTF_Font *police_temporaire;
    SDL_Color couleur = {rouge, vert, bleu, 255};
    SDL_Surface *surface_texte;
    SDL_Texture *texture_texte;
    SDL_Rect rectangle_texte;

    police_temporaire = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", taille_police);
    if (police_temporaire == NULL)
    {
        return;
    }

    surface_texte = TTF_RenderText_Blended(police_temporaire, texte, couleur);
    if (surface_texte == NULL)
    {
        TTF_CloseFont(police_temporaire);
        return;
    }

    texture_texte = SDL_CreateTextureFromSurface(rendu, surface_texte);
    if (texture_texte == NULL)
    {
        SDL_FreeSurface(surface_texte);
        TTF_CloseFont(police_temporaire);
        return;
    }

    rectangle_texte.w = surface_texte->w;
    rectangle_texte.h = surface_texte->h;
    rectangle_texte.x = (LARGEUR_FENETRE - rectangle_texte.w) / 2;
    rectangle_texte.y = position_y;

    SDL_RenderCopy(rendu, texture_texte, NULL, &rectangle_texte);

    SDL_DestroyTexture(texture_texte);
    SDL_FreeSurface(surface_texte);
    TTF_CloseFont(police_temporaire);
}

/*
 * Affiche le plateau, les joueurs, les scores et le classement.
 */
void afficher_plateau_sdl(Plateau *plateau, Joueur joueurs[], int nombre_joueurs, Fork forks[], int nombre_max_forks, int partie_terminee)
{
    int ligne;
    int colonne;
    int indice_joueur;
    int indice_classement;
    int indice_comparaison;
    int temporaire;

    int classement[NOMBRE_MAX_JOUEURS];

    SDL_Rect rectangle_case;
    SDL_Rect rectangle_joueur;
    SDL_Rect fond_hud;

    char texte_hud[128];

    /* Préparation du classement */
    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        classement[indice_joueur] = indice_joueur;
    }

    for (indice_classement = 0; indice_classement < nombre_joueurs - 1; indice_classement++)
    {
        for (indice_comparaison = indice_classement + 1; indice_comparaison < nombre_joueurs; indice_comparaison++)
        {
            if (joueurs[classement[indice_comparaison]].cases_marquees >
                joueurs[classement[indice_classement]].cases_marquees)
            {
                temporaire = classement[indice_classement];
                classement[indice_classement] = classement[indice_comparaison];
                classement[indice_comparaison] = temporaire;
            }
        }
    }

    /* Fond général */
    SDL_SetRenderDrawColor(rendu, 20, 20, 20, 255);
    SDL_RenderClear(rendu);

    /* Dessin des cases possédées */
    for (ligne = 0; ligne < HAUTEUR_PLATEAU; ligne++)
    {
        for (colonne = 0; colonne < LARGEUR_PLATEAU; colonne++)
        {
            rectangle_case.x = colonne * TAILLE_CASE;
            rectangle_case.y = ligne * TAILLE_CASE;
            rectangle_case.w = TAILLE_CASE;
            rectangle_case.h = TAILLE_CASE;

            switch (plateau->proprietaire[ligne][colonne])
            {
                case 0:
                    SDL_SetRenderDrawColor(rendu, 220, 40, 40, 255);
                    SDL_RenderFillRect(rendu, &rectangle_case);
                    break;

                case 1:
                    SDL_SetRenderDrawColor(rendu, 40, 220, 40, 255);
                    SDL_RenderFillRect(rendu, &rectangle_case);
                    break;

                case 2:
                    SDL_SetRenderDrawColor(rendu, 60, 120, 255, 255);
                    SDL_RenderFillRect(rendu, &rectangle_case);
                    break;

                case 3:
                    SDL_SetRenderDrawColor(rendu, 240, 220, 60, 255);
                    SDL_RenderFillRect(rendu, &rectangle_case);
                    break;

                default:
                    break;
            }
        }
    }

    /* Grille */
    SDL_SetRenderDrawColor(rendu, 55, 55, 55, 255);

    for (ligne = 0; ligne <= HAUTEUR_PLATEAU; ligne++)
    {
        SDL_RenderDrawLine(rendu, 0, ligne * TAILLE_CASE, LARGEUR_FENETRE, ligne * TAILLE_CASE);
    }

    for (colonne = 0; colonne <= LARGEUR_PLATEAU; colonne++)
    {
        SDL_RenderDrawLine(rendu, colonne * TAILLE_CASE, 0, colonne * TAILLE_CASE, HAUTEUR_FENETRE);
    }

    /* Joueurs */
    for (indice_joueur = 0; indice_joueur < nombre_joueurs; indice_joueur++)
    {
        rectangle_joueur.x = joueurs[indice_joueur].x * TAILLE_CASE;
        rectangle_joueur.y = joueurs[indice_joueur].y * TAILLE_CASE;
        rectangle_joueur.w = TAILLE_CASE;
        rectangle_joueur.h = TAILLE_CASE;

        switch (indice_joueur)
        {
            case 0:
                SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
                break;
            case 1:
                SDL_SetRenderDrawColor(rendu, 255, 105, 180, 255);
                break;
            case 2:
                SDL_SetRenderDrawColor(rendu, 0, 255, 255, 255);
                break;
            case 3:
                SDL_SetRenderDrawColor(rendu, 255, 140, 0, 255);
                break;
            default:
                SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
                break;
        }

        SDL_RenderFillRect(rendu, &rectangle_joueur);

        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderDrawRect(rendu, &rectangle_joueur);
    }

        /* Dessin des clones */
    for (indice_joueur = 0; indice_joueur < nombre_max_forks; indice_joueur++)
    {
        SDL_Rect rectangle_clone;

        if (!forks[indice_joueur].actif || !forks[indice_joueur].clone_demarre)
        {
            continue;
        }

        rectangle_clone.x = forks[indice_joueur].x_clone * TAILLE_CASE + TAILLE_CASE / 4;
        rectangle_clone.y = forks[indice_joueur].y_clone * TAILLE_CASE + TAILLE_CASE / 4;
        rectangle_clone.w = TAILLE_CASE / 2;
        rectangle_clone.h = TAILLE_CASE / 2;

        switch (forks[indice_joueur].proprietaire_original)
        {
            case 0:
                SDL_SetRenderDrawColor(rendu, 255, 120, 120, 255);
                break;
            case 1:
                SDL_SetRenderDrawColor(rendu, 120, 255, 120, 255);
                break;
            case 2:
                SDL_SetRenderDrawColor(rendu, 120, 180, 255, 255);
                break;
            case 3:
                SDL_SetRenderDrawColor(rendu, 255, 230, 120, 255);
                break;
            default:
                SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
                break;
        }

        SDL_RenderFillRect(rendu, &rectangle_clone);

        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderDrawRect(rendu, &rectangle_clone);
    }

    /* Fond du HUD */
    SDL_SetRenderDrawBlendMode(rendu, SDL_BLENDMODE_BLEND);

    fond_hud.x = 15;
    fond_hud.y = 15;
    fond_hud.w = 220;
    fond_hud.h = 280;

    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 200);
    SDL_RenderFillRect(rendu, &fond_hud);

    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
    SDL_RenderDrawRect(rendu, &fond_hud);

    /* Titre du HUD */
    dessiner_texte("Scores", 20, 15, 255, 255, 255);

    /* Scores individuels */
    if (nombre_joueurs > 0)
    {
        snprintf(texte_hud, sizeof(texte_hud), "J1 : %d", joueurs[0].cases_marquees);
        dessiner_texte(texte_hud, 20, 40, 220, 40, 40);
    }

    if (nombre_joueurs > 1)
    {
        snprintf(texte_hud, sizeof(texte_hud), "J2 : %d", joueurs[1].cases_marquees);
        dessiner_texte(texte_hud, 20, 62, 40, 220, 40);
    }

    if (nombre_joueurs > 2)
    {
        snprintf(texte_hud, sizeof(texte_hud), "J3 : %d", joueurs[2].cases_marquees);
        dessiner_texte(texte_hud, 20, 84, 60, 120, 255);
    }

    if (nombre_joueurs > 3)
    {
        snprintf(texte_hud, sizeof(texte_hud), "J4 : %d", joueurs[3].cases_marquees);
        dessiner_texte(texte_hud, 20, 106, 240, 220, 60);
    }

    /* Classement */
    dessiner_texte("Classement", 20, 140, 255, 255, 255);

    for (indice_classement = 0; indice_classement < nombre_joueurs; indice_classement++)
    {
        snprintf(
            texte_hud,
            sizeof(texte_hud),
            "%d. J%d (%d)",
            indice_classement + 1,
            classement[indice_classement] + 1,
            joueurs[classement[indice_classement]].cases_marquees
        );

        dessiner_texte(texte_hud, 20, 165 + indice_classement * 20, 255, 255, 255);
    }

    if (partie_terminee)
    {
        dessiner_texte("PARTIE TERMINEE", 20, 255, 255, 0, 0);
    }
    else
    {
        dessiner_texte("PARTIE EN COURS", 20, 255, 0, 255, 0);
    }

    if (partie_terminee && nombre_joueurs > 0)
    {
    SDL_Rect fond_central;
    char texte_gagnant[64];

    /* fond semi-transparent centré */
    SDL_SetRenderDrawBlendMode(rendu, SDL_BLENDMODE_BLEND);

    fond_central.w = 420;
    fond_central.h = 110;
    fond_central.x = (LARGEUR_FENETRE - fond_central.w) / 2;
    fond_central.y = (HAUTEUR_FENETRE - fond_central.h) / 2;

    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 170);
    SDL_RenderFillRect(rendu, &fond_central);

    SDL_SetRenderDrawColor(rendu, 255, 215, 0, 255);
    SDL_RenderDrawRect(rendu, &fond_central);

    snprintf(texte_gagnant, sizeof(texte_gagnant), "JOUEUR %d GAGNE !", classement[0] + 1);

    /* ombre légère */
    dessiner_texte_centre(texte_gagnant, fond_central.y + 24, 40, 0, 0, 0);

    /* texte principal */
    dessiner_texte_centre(texte_gagnant, fond_central.y + 20, 40, 255, 215, 0);
    }

    SDL_RenderPresent(rendu);
}

