#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"centre.h"
#include <gtk/gtk.h>

enum
{ 
	EID,
	ENOM,
	EJ,
	EM,
	EA,
	EVIL,
	ETYPE,
	
	
	COLUMNS,
};

enum
{ 
	EIDT,
	ENOMT,
	EPRNT,
	ETYPET,
	COLUMNSS,
};




//Ajouter
void ajouter_centre(centre b,char *cent){
FILE * f;
f=fopen(cent,"a");

  fprintf(f,"%s %s %d %s %d %s %s \n ",b.id,b.nom,b.jour,b.mois,b.annee,b.ville,b.type);
  fclose(f);
}

//add trainer
void ajouter_trainer(trainer x, char *train){
    FILE *f3 = fopen(train, "a");
    if(f3 != NULL){
        fprintf(f3, "%s %s %s %s\n", x.id, x.nom, x.prenom, x.type); // inclure le type si nécessaire
        fclose(f3);
    }
}





//Afficher

void afficher_centre(GtkWidget *liste){
GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	
	GtkTreeIter iter;
	GtkListStore *store;
	char id[20];
    	char nom[20];
    	char jour[20];
    	char  mois[20];
    	char annee[20];
    	char  ville[20];
    	char  type[20];
	
	store=NULL;
	FILE *f;
	store=gtk_tree_view_get_model (liste);
	if (store==NULL)
	{


	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("id", renderer, "text",EID,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);




	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("nom", renderer, "text",ENOM,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("jour", renderer, "text",EJ,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

	

	

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("mois", renderer, "text",EM,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);


	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("annee", renderer, "text",EA,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("ville", renderer, "text",EVIL,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("type", renderer, "text",ETYPE,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

	
	
	

	}
	store=gtk_list_store_new (COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,  G_TYPE_STRING,   G_TYPE_STRING, G_TYPE_STRING,  G_TYPE_STRING);
	f=fopen("centre.txt","r");
	if(f==NULL)
	{
		return;
	}
	else
	{
		f= fopen("centre.txt","a+");
		while(fscanf(f,"%s %s %s %s %s %s %s   \n",id,nom,jour,mois,annee,ville,type)!=EOF)
		{
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store,&iter, EID,id, ENOM, nom, EJ, jour, EM, mois, EA, annee ,EVIL ,ville,ETYPE ,type,-1);
		}
		fclose(f);
		gtk_tree_view_set_model (GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
		g_object_unref (store);
	}
}

//Modifier

void modifier_centre(char id[],centre b1,char *cent)
{
centre b;
  FILE* f=fopen(cent, "r");
  FILE* f2=fopen("centmod.txt", "w");
  
  if((f!=NULL) ||( f2!=NULL))
 {
while(fscanf(f,"%s %s %d %s %d %s %s \n ",b.id,b.nom,&b.jour,b.mois,&b.annee,b.ville,b.type)!=EOF)
{
if(strcmp(b.id,b1.id)==0)
  fprintf(f2,"%s %s %d %s %d %s %s \n ",b1.id,b1.nom,b1.jour,b1.mois,b1.annee,b1.ville,b1.type);
else
 fprintf(f2,"%s %s %d %s %d %s %s \n ",b.id,b.nom,b.jour,b.mois,b.annee,b.ville,b.type);
}
   fclose(f);
   fclose(f2);
remove(cent);
rename("centmod.txt",cent);
 }
}

//supprimer

void supprimer_centre(char id[],char *cent)
{
centre b;     
FILE * f=fopen(cent,"r");
FILE * f2=fopen("centsupp.txt","w");

while (fscanf(f,"%s %s %d %s %d %s %s \n  ",b.id,b.nom,&b.jour,b.mois,&b.annee,b.ville,b.type)!=EOF)
{

if(strcmp(b.id,id)!=0)

  fprintf(f2,"%s %s %d %s %d %s %s \n  ",b.id,b.nom,b.jour,b.mois,b.annee,b.ville,b.type);
}
  fclose(f);

  fclose(f2);

remove(cent);
rename("centsupp.txt",cent);

 
}

//fonction remplir tab
int remplirtabRech (centre tab[],int nb)
{
      	char id[20];
    	char nom[20];
    	int jour;
    	char  mois[20];
    	int annee;
	char ville[20];
    	char  type[20];
  
    FILE* fichier ;
    int i;

    fichier = fopen("centre.txt", "a+");

        while (fscanf(fichier,"%s %s %d %s %d %s %s \n ",id,nom,&jour,mois,&annee,ville,type)!=EOF)
        {
            
            strcpy(tab[i].id,id);
	    strcpy(tab[i].nom,nom);
	    tab[i].jour=jour;
	    strcpy(tab[i].mois,mois);
	    tab[i].annee=annee;	
	    strcpy(tab[i].ville,ville);
	    strcpy(tab[i].type,type);
	
	    
           
            nb++;
	    i++;    
        }
        fclose(fichier);
	return(nb);
}

//rechercher 

int rechercher_centre(char id[])
{
centre tab[100];
int nb;
int ce,i;
FILE*f;
ce=0;
nb=remplirtabRech (tab,nb);
for (i=0;i<nb;i++)
	{if ((strcmp(id,tab[i].id)==0) ||(strcmp(id,tab[i].nom)==0))
		{
		ce=1;
		f=fopen("rechcentre.txt", "w+");
		if("f!=NULL")			
	{fprintf(f,"%s %s %d %s %d %s %s \n ",tab[i].id,tab[i].nom,tab[i].jour,tab[i].mois,tab[i].annee,tab[i].ville,tab[i].type);}
		fclose(f);		
		}
	
	}
return(ce);		
}

//AFF RECH
void afficher_rechercher_centre(GtkWidget *liste){
GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	
	GtkTreeIter iter;
	GtkListStore *store;
	char id[20];
    	char nom[20];
    	char jour[20];
    	char  mois[20];
    	char annee[20];
    	char  ville[20];
	char  type[20];
	
	store=NULL;
	FILE *f;
	store=gtk_tree_view_get_model (liste);
	if (store==NULL)
	{


	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("id", renderer, "text",EID,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);




	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("nom", renderer, "text",ENOM,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("jour", renderer, "text",EJ,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

	

	

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("mois", renderer, "text",EM,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);


	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("annee", renderer, "text",EA,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("ville", renderer, "text",EVIL,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("type", renderer, "text",ETYPE,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

	
	
	

	}
	store=gtk_list_store_new (COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,  G_TYPE_STRING,   G_TYPE_STRING, G_TYPE_STRING,  G_TYPE_STRING);
	f=fopen("rechcentre.txt","r");
	if(f==NULL)
	{
		return;
	}
	else
	{
		f= fopen("rechcentre.txt","a+");
		while(fscanf(f,"%s %s %s %s %s %s %s   \n",id,nom,jour,mois,annee,ville,type)!=EOF)
		{
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store,&iter, EID,id, ENOM, nom, EJ, jour, EM, mois, EA, annee ,EVIL ,ville,ETYPE ,type,-1);
		}
		fclose(f);
		gtk_tree_view_set_model (GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
		g_object_unref (store);
	}
}
        



// Afficher les trainers
void afficher_trainer(GtkWidget *liste){
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkListStore *store;
    char id[20], nom[20], prenom[20], type[30];

    // Création des colonnes si elles n'existent pas
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(liste)));
    if (store == NULL) {
        // ID
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        // Nom
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        // Prenom
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Prenom", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        // Type
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    }

    // Création du modèle
    store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    // Lecture du fichier trainer.txt
    FILE *f = fopen("trainer.txt", "r");
    if (f == NULL) return;

    while (fscanf(f, "%s %s %s %s\n", id, prenom, nom, type) != EOF) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, id,
                           1, nom,
                           2, prenom,
                           3, type,
                           -1);
    }
    fclose(f);

    gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
    g_object_unref(store);
}








	
	




// statistique : calcul HG et TR
void calcul_pourcentage_hg_tr(int *nb_hg, int *nb_tr)
{
    *nb_hg = 0;
    *nb_tr = 0;

    FILE *f = fopen("centre.txt", "r");
    if (f == NULL)
        return;

    char id[20], nom[30], mois[20], ville[20], type[30];
    int jour, annee;

    while (fscanf(f, "%s %s %d %s %d %s %s\n", id, nom, &jour, mois, &annee, ville, type) != EOF)
    {
        if (strcmp(type, "Gym-Haute-Gamme") == 0)
            (*nb_hg)++;
        else if (strcmp(type, "Gym-Traditionel") == 0)
            (*nb_tr)++;
    }

    fclose(f);
}





// afficher GtkCList pour les trainers
void afficher_clist_trainer(GtkCList *clist)
{
    FILE *f = fopen("trainer.txt", "r");
    if (!f) return;

    gtk_clist_clear(clist);

    int nb_CP = 0, nb_AC = 0, nb_SF = 0;
    int nb_CPe = 0, nb_N = 0, nb_CS = 0;

    char id[20], nom[20], prenom[20], type[30];

    typedef struct {
        char id[20];
        char type[30];
    } Tmp;

    Tmp tab[100];
    int n = 0;

    while (fscanf(f, "%s %s %s %s\n", id, nom, prenom, type) != EOF)
    {
        strcpy(tab[n].id, id);
        strcpy(tab[n].type, type);

        if (strcmp(type, "CoachPrincipal") == 0) nb_CP++;
        else if (strcmp(type, "AssistantCoach") == 0) nb_AC++;
        else if (strcmp(type, "SpecialisteFitness") == 0) nb_SF++;
        else if (strcmp(type, "CoachPersonnel") == 0) nb_CPe++;
        else if (strcmp(type, "Nutritionniste") == 0) nb_N++;
        else if (strcmp(type, "CoachSanté") == 0) nb_CS++;

        n++;
    }
    fclose(f);

    for (int i = 0; i < n; i++)
    {
        gchar *row[3];
        char classement[10];

        row[0] = tab[i].id;
        row[1] = tab[i].type;

        if (strcmp(tab[i].type, "CoachPrincipal") == 0)
            sprintf(classement, "%d", nb_CP);
        else if (strcmp(tab[i].type, "AssistantCoach") == 0)
            sprintf(classement, "%d", nb_AC);
        else if (strcmp(tab[i].type, "SpecialisteFitness") == 0)
            sprintf(classement, "%d", nb_SF);
        else if (strcmp(tab[i].type, "CoachPersonnel") == 0)
            sprintf(classement, "%d", nb_CPe);
        else if (strcmp(tab[i].type, "Nutritionniste") == 0)
            sprintf(classement, "%d", nb_N);
        else if (strcmp(tab[i].type, "CoachSanté") == 0)
            sprintf(classement, "%d", nb_CS);
        else
            strcpy(classement, "0");

        row[2] = classement;
        gtk_clist_append(clist, row);
    }
}




















