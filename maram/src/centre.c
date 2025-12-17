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

void ajouter_trainer(trainer x,char *train){
FILE * f3;
f3=fopen(train,"a");

  fprintf(f3,"%s %s %s \n ",x.id,x.nom,x.prenom);
  fclose(f3);
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
        
//afficher trainer

void afficher_trainer(GtkWidget *liste){
GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	
	GtkTreeIter iter;
	GtkListStore *store;
	char id[20];
    	char nom[20];
    	char prenom[20];
    	
	
	store=NULL;
	FILE *f;
	store=gtk_tree_view_get_model (liste);
	if (store==NULL)
	{


	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("id", renderer, "text",EIDT,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);




	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("nom", renderer, "text",ENOMT,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("prenom", renderer, "text",EPRNT,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (liste),column);

	

	
	
	

	}
	store=gtk_list_store_new (COLUMNSS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	f=fopen("trainer.txt","r");
	if(f==NULL)
	{
		return;
	}
	else
	{
		f= fopen("trainer.txt","a+");
		while(fscanf(f,"%s %s %s    \n",id,nom,prenom)!=EOF)
		{
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store,&iter, EIDT,id, ENOMT, nom, EPRNT, prenom,-1);
		}
		fclose(f);
		gtk_tree_view_set_model (GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
		g_object_unref (store);
	}
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








