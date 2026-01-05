#include <gtk/gtk.h>

typedef struct
{ 
    char id[20];
    char nom[20];
    int jour;
    char  mois[20];
    int annee;
    char  ville[20];
    char type[20];
    
}centre;

typedef struct
{
	char id[20];
	char nom[20];
	char prenom[20];
        char type[30];  
        int classement;


}trainer;

void ajouter_centre(centre b,char *cent);
void afficher_centre(GtkWidget *liste);
void modifier_centre(char id[],centre b,char *cent);
void supprimer_centre(char id[],char *cent);
int rechercher_centre(char id[]);
void afficher_rechercher_centre(GtkWidget *liste);
int remplirtabRech (centre tab[],int nb) ;

void ajouter_trainer(trainer x,char *train);
void afficher_trainer(GtkWidget *liste);
void calcul_pourcentage_hf(int *nb_h, int *nb_f);







