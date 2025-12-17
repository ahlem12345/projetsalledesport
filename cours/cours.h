#ifndef COURS_H
#define COURS_H

#define MAX 50

typedef struct {
    char id[20];
    char nom[50];
    char coach[50];
    char jour[20];
    char heure[10];
    int places;
    int nb_inscrits;
} Cours;

/* Fonctions CRUD */
void ajouter_cours(Cours c);
void modifier_cours(Cours c);
void supprimer_cours(char *id);
Cours rechercher_cours(char *id);

/* Fonction pour inscrire un membre */
void inscrire_membre(char *id);

/* Fonctions statistiques */
int nombre_total_cours();
int nombre_cours_actifs();

#endif
