#include "../include/chargement.h"
#include <stdio.h>
#include <dlfcn.h>

int charger_joueur(Joueur *joueur, const char *chemin_bibliotheque)
{
    joueur->bibliotheque = dlopen(chemin_bibliotheque, RTLD_LAZY);
    if (joueur->bibliotheque == NULL)
    {
        fprintf(stderr, "Erreur dlopen pour %s : %s\n", chemin_bibliotheque, dlerror());
        return 0;
    }

    dlerror(); // remise à zéro

    joueur->obtenir_action = (fonction_action_t)dlsym(joueur->bibliotheque, "get_action");

    if (dlerror() != NULL)
    {
        fprintf(stderr, "Erreur dlsym : impossible de trouver get_action dans %s\n", chemin_bibliotheque);
        dlclose(joueur->bibliotheque);
        joueur->bibliotheque = NULL;
        return 0;
    }

    return 1;
}

void liberer_joueur(Joueur *joueur)
{
    if (joueur->bibliotheque != NULL)
    {
        dlclose(joueur->bibliotheque);
        joueur->bibliotheque = NULL;
    }
}