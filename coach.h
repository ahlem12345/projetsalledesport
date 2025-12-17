#ifndef COACH_H
#define COACH_H

#include <gtk/gtk.h>

#define COACH_FILE "coach.txt"
#define MATERIEL_FILE "materiel.txt"
#define DISPONIBILITE_FILE "disponibilite.txt"
#define SALLE_FILE "salles.txt"
#define COURS_FILE "cours.txt"

typedef struct {
    char nom[30];
    char prenom[30];
    int id;
    char specialite[30];
    int nbr_cours;
    char coaching_prive[5];
} coach;

typedef struct {
    char nom[50];
    char type[20];  // "Privé", "Groupe", "Mixte"
    int participants;
    char materiels[200];
    char salle[50];
    char date[20];
} cours;

typedef struct {
    char nom[50];
    char type[30];
    int quantite;
} materiel;

typedef struct {
    char nom[50];
    char type[30];
    int capacite;
} salle;

// Fonctions pour les coachs (existantes)
int ajouter_dans_fichier_coach(coach c);
void afficher_coachs(GtkWidget *treeview);
void nettoyer_treeview(GtkWidget *treeview);
void charger_ids_dans_combo(GtkWidget *combo);
int modifier_coach(int id, coach nouv);
void afficher_contenu_fichier(void);
int supprimer_coach(char *filename, int id);
void enregistrer_suppression_log(int id, const char *nom, const char *prenom, 
                                 const char *raison, const char *commentaire);
void rechercher_coachs(const char *specialite, const char *coaching_prive, GtkWidget *treeview);

// Fonctions pour les cours
void charger_materiels_dans_combo(GtkWidget *combo);
void charger_salles_dans_combo(GtkWidget *combo);
int verifier_disponibilite(const char *nom, const char *type); // type: "materiel" ou "salle"
void afficher_materiels_dans_treeview(GtkWidget *treeview);
void afficher_salles_dans_treeview(GtkWidget *treeview);
void ajouter_element_cours_treeview(const char *nom, const char *type, GtkWidget *treeview);

void afficher_message_erreur(GtkWidget *parent, const char *message);
void afficher_message_erreur(GtkWidget *window, const char *message);
void initialiser_combo01(GtkWidget *window);
void initialiser_combo02(GtkWidget *window);
int sauvegarder_cours_fichier(cours c);
void marquer_indisponible(const char *nom, const char *type);

#endif
