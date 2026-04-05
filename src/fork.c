#include "../include/fork.h"
#include "../include/plateau.h"
#include "../include/joueur.h"

/*
 * Initialise les forks comme inactifs.
 */
void initialiser_forks(Fork forks[], int nombre_max_forks)
{
    int indice_fork;

    for (indice_fork = 0; indice_fork < nombre_max_forks; indice_fork++)
    {
        forks[indice_fork].actif = 0;
        forks[indice_fork].clone_demarre = 0;
        forks[indice_fork].proprietaire_original = -1;
        forks[indice_fork].x_depart = 0;
        forks[indice_fork].y_depart = 0;
        forks[indice_fork].x_clone = 0;
        forks[indice_fork].y_clone = 0;
        forks[indice_fork].tours_avant_depart = 0;
        forks[indice_fork].duree_restante = 0;
        forks[indice_fork].indice_action_historique_depart = 0;
    }
}

/*
 * Enregistre une action dans l'historique du joueur.
 */
void enregistrer_action_joueur(Joueur *joueur, Action action)
{
    int indice;

    if (joueur->nombre_actions_historique < TAILLE_HISTORIQUE_ACTIONS)
    {
        joueur->historique_actions[joueur->nombre_actions_historique] = action;
        joueur->nombre_actions_historique++;
    }
    else
    {
        for (indice = 1; indice < TAILLE_HISTORIQUE_ACTIONS; indice++)
        {
            joueur->historique_actions[indice - 1] = joueur->historique_actions[indice];
        }

        joueur->historique_actions[TAILLE_HISTORIQUE_ACTIONS - 1] = action;
    }
}

/*
 * Crée un fork différé.
 * Le clone apparaîtra 5 tours plus tard.
 */
int creer_fork(Fork forks[], int nombre_max_forks, Joueur *joueur)
{
    int indice_fork;

    for (indice_fork = 0; indice_fork < nombre_max_forks; indice_fork++)
    {
        if (!forks[indice_fork].actif)
        {
            forks[indice_fork].actif = 1;
            forks[indice_fork].clone_demarre = 0;
            forks[indice_fork].proprietaire_original = joueur->identifiant;

            forks[indice_fork].x_depart = joueur->x;
            forks[indice_fork].y_depart = joueur->y;

            forks[indice_fork].x_clone = joueur->x;
            forks[indice_fork].y_clone = joueur->y;

            forks[indice_fork].tours_avant_depart = DUREE_ATTENTE_FORK;
            forks[indice_fork].duree_restante = DUREE_CLONE_FORK;
            forks[indice_fork].indice_action_historique_depart = joueur->nombre_actions_historique;

            joueur->fork_actif = 1;
            joueur->tours_fork_restants = DUREE_CLONE_FORK;

            return 1;
        }
    }

    return 0;
}

static int coordonnee_boucle(int valeur, int taille_max)
{
    while (valeur < 0)
    {
        valeur += taille_max;
    }

    while (valeur >= taille_max)
    {
        valeur -= taille_max;
    }

    return valeur;
}

static void appliquer_action_clone(Joueur *clone, Action action)
{
    switch (action)
    {
        case ACTION_MOVE_L:
            clone->x = coordonnee_boucle(clone->x - 1, LARGEUR_PLATEAU);
            break;

        case ACTION_MOVE_R:
            clone->x = coordonnee_boucle(clone->x + 1, LARGEUR_PLATEAU);
            break;

        case ACTION_MOVE_U:
            clone->y = coordonnee_boucle(clone->y - 1, HAUTEUR_PLATEAU);
            break;

        case ACTION_MOVE_D:
            clone->y = coordonnee_boucle(clone->y + 1, HAUTEUR_PLATEAU);
            break;

        case ACTION_DASH_L:
            clone->x = coordonnee_boucle(clone->x - 8, LARGEUR_PLATEAU);
            break;

        case ACTION_DASH_R:
            clone->x = coordonnee_boucle(clone->x + 8, LARGEUR_PLATEAU);
            break;

        case ACTION_DASH_U:
            clone->y = coordonnee_boucle(clone->y - 8, HAUTEUR_PLATEAU);
            break;

        case ACTION_DASH_D:
            clone->y = coordonnee_boucle(clone->y + 8, HAUTEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_L:
            clone->x = coordonnee_boucle(clone->x - 8, LARGEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_R:
            clone->x = coordonnee_boucle(clone->x + 8, LARGEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_U:
            clone->y = coordonnee_boucle(clone->y - 8, HAUTEUR_PLATEAU);
            break;

        case ACTION_TELEPORT_D:
            clone->y = coordonnee_boucle(clone->y + 8, HAUTEUR_PLATEAU);
            break;

        case ACTION_BOMB:
        case ACTION_FORK:
        case ACTION_STILL:
        default:
            break;
    }
}

/*
 * Met à jour les forks.
 * Un clone rejoue les actions du joueur original à partir de l'instant de départ du clone.
 */
void mettre_a_jour_forks(Fork forks[], int nombre_max_forks, Plateau *plateau, Joueur joueurs[])
{
    int indice_fork;
    int indice_action;
    Joueur clone_fictif;
    Joueur *joueur_original;

    for (indice_fork = 0; indice_fork < nombre_max_forks; indice_fork++)
    {
        if (!forks[indice_fork].actif)
        {
            continue;
        }

        joueur_original = &joueurs[forks[indice_fork].proprietaire_original];

        if (forks[indice_fork].tours_avant_depart > 0)
        {
            forks[indice_fork].tours_avant_depart--;

            if (forks[indice_fork].tours_avant_depart == 0)
            {
                forks[indice_fork].clone_demarre = 1;
                forks[indice_fork].x_clone = forks[indice_fork].x_depart;
                forks[indice_fork].y_clone = forks[indice_fork].y_depart;
            }

            continue;
        }

        indice_action = forks[indice_fork].indice_action_historique_depart;

        if (indice_action >= joueur_original->nombre_actions_historique)
        {
            continue;
        }

        clone_fictif.x = forks[indice_fork].x_depart;
        clone_fictif.y = forks[indice_fork].y_depart;
        clone_fictif.identifiant = forks[indice_fork].proprietaire_original;
        clone_fictif.credit = 0;
        clone_fictif.cases_marquees = 0;
        clone_fictif.bibliotheque = NULL;
        clone_fictif.obtenir_action = NULL;
        clone_fictif.nombre_actions_historique = 0;
        clone_fictif.fork_actif = 0;
        clone_fictif.tours_fork_restants = 0;

        {
            int offset;
            int nombre_actions_clone;

            nombre_actions_clone = DUREE_CLONE_FORK - forks[indice_fork].duree_restante;

            for (offset = 0; offset <= nombre_actions_clone; offset++)
            {
                if (forks[indice_fork].indice_action_historique_depart + offset < joueur_original->nombre_actions_historique)
                {
                    appliquer_action_clone(
                        &clone_fictif,
                        joueur_original->historique_actions[forks[indice_fork].indice_action_historique_depart + offset]
                    );
                }
            }
        }

        /* Sauvegarde de la position visible du clone */
        forks[indice_fork].x_clone = clone_fictif.x;
        forks[indice_fork].y_clone = clone_fictif.y;

        marquer_case(plateau, joueurs, &clone_fictif);

        forks[indice_fork].duree_restante--;

        if (joueur_original->tours_fork_restants > 0)
        {
            joueur_original->tours_fork_restants--;

            if (joueur_original->tours_fork_restants == 0)
            {
                joueur_original->fork_actif = 0;
            }
        }

        if (forks[indice_fork].duree_restante <= 0)
        {
            forks[indice_fork].actif = 0;
            forks[indice_fork].clone_demarre = 0;
        }
    }
}