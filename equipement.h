#ifndef EQUIPEMENT_H_INCLUDED
#define EQUIPEMENT_H_INCLUDED

#include <gtk/gtk.h>

#define DISPO 0
#define RESERVE 1
#define MAINT 2

#define DANS_LA_SALLE 0
#define EN_STOCK 1
#define MAX_RESA 100
#define MAX_EQUIP 50
#define MAX_COACH 50

typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct {
    char reference[50];
    char nom[100];
    char type[50];
    int quantite;
    int etat;          // utiliser DISPO, RESERVE, MAINT
    int localisation;  // utiliser DANS_LA_SALLE, EN_STOCK
} Equipement;

typedef struct {
    char id_coach[20];
    char equipement[20];
    int quantite;
    Date dateReservation;
    int active; // 1 = réservée, 0 = annulée
} Reservation;

const char* etat_to_string(int etat);
const char* localisation_to_string(int loc);


int ajouterEquipement(char *filename, Equipement e);
int rechercherEquipement(char *filename, char *reference);
int modifierEquipement(char *filename, char *reference, Equipement nouv);
int supprimerEquipement(char *filename, char *reference);
void afficherEquipementsCTree(GtkWidget *ctree);
void afficherEquipements(char *filename);



int ajouterReservation(char *filename, Reservation r);
int annulerReservation(char *filename, char *id_coach, char *equipement);
int verifierDisponibilite(char *filename, char *equipement, Date date, int quantite);
void afficherReservations(char *filename);
void calculerStatistiques(char *fichierEquip, char *fichierReserv);


#endif
