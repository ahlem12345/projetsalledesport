#include <stdio.h>
#include <string.h>
#include "cours.h"

#define FICHIER "cours.txt"

#include <stdio.h>
#include <string.h>
#include "cours.h"

#define FICHIER "cours.txt"

/* AJOUTER */
void ajouter_cours(Cours c) {
    FILE *f = fopen(FICHIER, "a");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // Initialiser nb_inscrits à 0 pour un nouveau cours
    c.nb_inscrits = 0;
    
    // Écrire avec nb_inscrits
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
    int nb_inscrits;
    
    while (fscanf(f, "%s %s %s %s %s %d %d",
                  x.id, x.nom, x.coach, x.jour, x.heure,
                  &x.places, &nb_inscrits) != EOF)
    {
        if (strcmp(x.id, c.id) == 0) {
            // Garder le nombre d'inscrits existant lors de la modification
            c.nb_inscrits = nb_inscrits;
            fprintf(tmp, "%s %s %s %s %s %d %d\n",
                    c.id, c.nom, c.coach, c.jour, c.heure, c.places, c.nb_inscrits);
        } else {
            fprintf(tmp, "%s %s %s %s %s %d %d\n",
                    x.id, x.nom, x.coach, x.jour, x.heure, x.places, nb_inscrits);
        }
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
    int nb_inscrits;
    
    while (fscanf(f, "%s %s %s %s %s %d %d",
                  c.id, c.nom, c.coach, c.jour, c.heure,
                  &c.places, &nb_inscrits) != EOF)
    {
        if (strcmp(c.id, id) != 0) {
            fprintf(tmp, "%s %s %s %s %s %d %d\n",
                    c.id, c.nom, c.coach, c.jour, c.heure, c.places, nb_inscrits);
        }
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


int inscrire_membre(char *id)
{
    FILE *f = fopen(FICHIER, "r");
    FILE *tmp = fopen("tmp.txt", "w");
    if (!f || !tmp) {
        printf("Erreur: Impossible d'ouvrir les fichiers\n");
        return 0;
    }

    int inscription_reussie = 0;
    Cours c;

    while (fscanf(f, "%s %s %s %s %s %d %d",
                  c.id, c.nom, c.coach, c.jour, c.heure,
                  &c.places, &c.nb_inscrits) != EOF)
    {
        if (strcmp(c.id, id) == 0) {
            if (c.nb_inscrits < c.places) {
                c.nb_inscrits++;  // Ajouter un inscrit
                inscription_reussie = 1;
            } else {
                printf("Cours %s complet (%d/%d)\n", c.nom, c.nb_inscrits, c.places);
            }
        }

        fprintf(tmp, "%s %s %s %s %s %d %d\n",
                c.id, c.nom, c.coach, c.jour, c.heure, c.places, c.nb_inscrits);
    }

    fclose(f);
    fclose(tmp);

    // Remplacer l'ancien fichier
    remove(FICHIER);
    rename("tmp.txt", FICHIER);

    return inscription_reussie;
}
//fonction desinscrire

int desinscrire_membre(char *id)
{
    FILE *f = fopen(FICHIER, "r");
    FILE *tmp = fopen("tmp.txt", "w");
    if (!f || !tmp) {
        printf("Erreur: Impossible d'ouvrir les fichiers\n");
        return 0;
    }

    int desinscription_reussie = 0;
    Cours c;

    while (fscanf(f, "%s %s %s %s %s %d %d",
                  c.id, c.nom, c.coach, c.jour, c.heure,
                  &c.places, &c.nb_inscrits) != EOF)
    {
        if (strcmp(c.id, id) == 0) {
            if (c.nb_inscrits > 0) {
                c.nb_inscrits--;  // Ajouter un inscrit
                desinscription_reussie = 1;
            } else {
                printf("Cours %s vide (%d/%d)\n", c.nom, c.nb_inscrits, c.places);
            }
        }

        fprintf(tmp, "%s %s %s %s %s %d %d\n",
                c.id, c.nom, c.coach, c.jour, c.heure, c.places, c.nb_inscrits);
    }

    fclose(f);
    fclose(tmp);

    // Remplacer l'ancien fichier
    remove(FICHIER);
    rename("tmp.txt", FICHIER);

    return desinscription_reussie;
}

/* STATISTIQUES */
int nombre_total_cours()
{
    FILE *f = fopen(FICHIER, "r");
    if (!f) return 0;

    int count = 0;
    char line[256];
    
    // Compter les lignes non vides
    while (fgets(line, sizeof(line), f)) {
        if (strlen(line) > 1) { // Ligne non vide
            count++;
        }
    }

    fclose(f);
    return count;
}

Cours cours_le_plus_actif()
{
    FILE *f = fopen(FICHIER, "r");
    Cours c, plus_actif;
    strcpy(plus_actif.id, "NOT_FOUND");
    int max_inscrits = -1;

    if (!f) return plus_actif;

    while (fscanf(f, "%s %s %s %s %s %d %d",
                  c.id, c.nom, c.coach, c.jour, c.heure,
                  &c.places, &c.nb_inscrits) != EOF)
    {
        if (c.nb_inscrits > max_inscrits) {
            max_inscrits = c.nb_inscrits;
            plus_actif = c;
        }
    }

    fclose(f);
    return plus_actif;
}



