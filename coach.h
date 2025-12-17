#ifndef COACH_H
#define COACH_H

#include <gtk/gtk.h>

typedef struct {
    int id;
    char nom[30];
    char prenom[30];
    char specialite[30];
    int nbr_cours;
    char coaching_prive[10];
} coach;

#define COACH_FILE "coach.txt"

// Prototypes des fonctions
int ajouter_dans_fichier_coach(coach c);
int modifier_coach(int id, coach nouv);
void afficher_coachs(GtkWidget *treeview);
void charger_ids_dans_combo(GtkWidget *combo);
void initialiser_combospe(GtkWidget *window);
void initialiser_combo9(GtkWidget *window);  // Ajoutez cette ligne

#endif
