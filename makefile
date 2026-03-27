CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -ldl -lSDL2 -lSDL2_ttf
DOSSIER_SRC = src
DOSSIER_JOUEURS = joueurs

SOURCES = $(DOSSIER_SRC)/main.c \
          $(DOSSIER_SRC)/plateau.c \
          $(DOSSIER_SRC)/joueur.c \
          $(DOSSIER_SRC)/chargement.c \
          $(DOSSIER_SRC)/affichage_sdl.c

OBJETS = $(SOURCES:.c=.o)

all: splash joueurs

splash: $(OBJETS)
	$(CC) $(OBJETS) -o splash $(LDFLAGS)

joueurs: joueur1.so joueur2.so joueur3.so joueur4.so

joueur1.so: $(DOSSIER_JOUEURS)/joueur1.c include/actions.h
	$(CC) -Wall -Wextra -fPIC -shared $(DOSSIER_JOUEURS)/joueur1.c -o joueur1.so

joueur2.so: $(DOSSIER_JOUEURS)/joueur2.c include/actions.h
	$(CC) -Wall -Wextra -fPIC -shared $(DOSSIER_JOUEURS)/joueur2.c -o joueur2.so

joueur3.so: $(DOSSIER_JOUEURS)/joueur3.c include/actions.h
	$(CC) -Wall -Wextra -fPIC -shared $(DOSSIER_JOUEURS)/joueur3.c -o joueur3.so

joueur4.so: $(DOSSIER_JOUEURS)/joueur4.c include/actions.h
	$(CC) -Wall -Wextra -fPIC -shared $(DOSSIER_JOUEURS)/joueur4.c -o joueur4.so

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o splash *.so