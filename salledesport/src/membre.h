#ifndef MEMBRE_H
#define MEMBRE_H

#include <gtk/gtk.h>

typedef struct {
    char id[20];
    char nom[50];
    char prenom[50];
    char tel[50];
    char email[100];
    float longueur;
    float poids;
    float frais_paye;
    char type_abonnement[30];
    char duree_abonnement[20];
    guint jour_naissance;
    guint mois_naissance;
    guint annee_naissance;
    guint jour_inscription;
    guint mois_inscription;
    guint annee_inscription;
} membre;

typedef struct {
    char type[50];               
    char acces[100];             
    char sauna_hamem[10];       
    char coaching_personnalise[10];
    char duree[20];              
    char prix[20];              
} abonnement;

typedef struct {
    char nom_membre[64];
    char type_entrainement[64];
    char coach_choisi[64];
    char heure[16];
    int jour_souhaite;
    int mois_souhaite;
    int annee_souhaitee;
    char type_seance[32];
    char materiel[8];
    char commentaire[256];
} demande_coach;


/* Member functions */
int ajouter_membre(membre *m, const char *filename);
int modifier_membre(const char *id_recherche, membre *m, const char *filename);
int supprimer_membre(const char *id_recherche, const char *filename);
int charger_membres(GtkListStore *store, const char *filename);
int calculer_statistiques(const char *filename, int *total, int *actifs, int *inactifs, float *pourcentage);
float calculer_imc(const char *id_recherche, const char *filename);
int charger_abonnements(GtkListStore *store, const char *filename);
int ajouter_demande_coach(demande_coach d, const char *filename);

int supprimer_abonnement(const char *type, const char *filename);

int rechercher_abonnement(GtkWidget *treeview, const char *type_recherche);

void lire_date_selectionnee(GtkWidget *calendar, int *j, int *m, int *a);
void get_type_seance(GtkWidget *c1, GtkWidget *c2, char *out);
void get_materiel(GtkWidget *r2, GtkWidget *r3, char *out);

#endif

