#include <stdio.h>
#include <string.h>
#include "cours.h"

#define FICHIER "cours.txt"

/* AJOUTER */
void ajouter_cours(Cours c)
{
    FILE *f = fopen(FICHIER, "a");
    if (!f) return;

    fprintf(f, "%s %s %s %s %s %d %d\n",
            c.id, c.nom, c.coach, c.jour, c.heure, c.places, c.nb_inscrits);

    fclose(f);
}

/* MODIFIER */
void modifier_cours(Cours c)
{
    FILE *f = fopen(FICHIER, "r");
    FILE *tmp = fopen("tmp.txt", "w");
    if (!f || !tmp) return;

    Cours x;
    while (fscanf(f, "%s %s %s %s %s %d %d",
                  x.id, x.nom, x.coach, x.jour, x.heure,
                  &x.places, &x.nb_inscrits) != EOF)
    {
        if (strcmp(x.id, c.id) == 0)
            x = c;

        fprintf(tmp, "%s %s %s %s %s %d %d\n",
                x.id, x.nom, x.coach, x.jour, x.heure, x.places, x.nb_inscrits);
    }

    fclose(f);
    fclose(tmp);

    remove(FICHIER);
    rename("tmp.txt", FICHIER);
}

/* SUPPRIMER */
void supprimer_cours(char *id)
{
    FILE *f = fopen(FICHIER, "r");
    FILE *tmp = fopen("tmp.txt", "w");
    if (!f || !tmp) return;

    Cours c;
    while (fscanf(f, "%s %s %s %s %s %d %d",
                  c.id, c.nom, c.coach, c.jour, c.heure,
                  &c.places, &c.nb_inscrits) != EOF)
    {
        if (strcmp(c.id, id) != 0)
            fprintf(tmp, "%s %s %s %s %s %d %d\n",
                    c.id, c.nom, c.coach, c.jour, c.heure, c.places, c.nb_inscrits);
    }

    fclose(f);
    fclose(tmp);

    remove(FICHIER);
    rename("tmp.txt", FICHIER);
}

/* RECHERCHER */
Cours rechercher_cours(char *id)
{
    FILE *f = fopen(FICHIER, "r");
    Cours c;
    strcpy(c.id, "NOT_FOUND");

    if (!f) return c;

    while (fscanf(f, "%s %s %s %s %s %d %d",
                  c.id, c.nom, c.coach, c.jour, c.heure,
                  &c.places, &c.nb_inscrits) != EOF)
    {
        if (strcmp(c.id, id) == 0)
        {
            fclose(f);
            return c;
        }
    }

    fclose(f);
    strcpy(c.id, "NOT_FOUND");
    return c;
}

/* INSCRIRE UN MEMBRE */
void inscrire_membre(char *id)
{
    FILE *f = fopen(FICHIER, "r");
    FILE *tmp = fopen("tmp.txt", "w");
    if (!f || !tmp) return;

    Cours c;
    while (fscanf(f, "%s %s %s %s %s %d %d",
                  c.id, c.nom, c.coach, c.jour, c.heure,
                  &c.places, &c.nb_inscrits) != EOF)
    {
        if (strcmp(c.id, id) == 0)
        {
            if (c.nb_inscrits < c.places)
                c.nb_inscrits++;
        }

        fprintf(tmp, "%s %s %s %s %s %d %d\n",
                c.id, c.nom, c.coach, c.jour, c.heure, c.places, c.nb_inscrits);
    }

    fclose(f);
    fclose(tmp);

    remove(FICHIER);
    rename("tmp.txt", FICHIER);
}

/* STATISTIQUES */
int nombre_total_cours()
{
    FILE *f = fopen(FICHIER, "r");
    if (!f) return 0;

    int count = 0;
    Cours c;
    while (fscanf(f, "%s %s %s %s %s %d %d",
                  c.id, c.nom, c.coach, c.jour, c.heure,
                  &c.places, &c.nb_inscrits) != EOF)
        count++;

    fclose(f);
    return count;
}

int nombre_cours_actifs()
{
    FILE *f = fopen(FICHIER, "r");
    if (!f) return 0;

    int count = 0;
    Cours c;
    while (fscanf(f, "%s %s %s %s %s %d %d",
                  c.id, c.nom, c.coach, c.jour, c.heure,
                  &c.places, &c.nb_inscrits) != EOF)
        if (c.nb_inscrits < c.places)
            count++;

    fclose(f);
    return count;
}
