#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include"centre.h"
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

int x,y,z;
int l,m,n;
void
on_radiobutton_hg_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON (togglebutton)))
{x=2;}
}


void
on_radiobutton_tr_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON (togglebutton)))
{x=1;}
}


void
on_checkbutton_conf_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON (togglebutton)))
{y=1;}
}


void
on_button_addc_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
centre b;
GtkWidget *windowajout,*idd,*nm,*jr,*vil,*ann,*typ,*combobox1, *output, *output1;
windowajout=create_Gestion_centre();

idd= lookup_widget(button,"entry_addid");
nm= lookup_widget(button,"entry_addn");
vil= lookup_widget(button,"entry_addv");


jr = lookup_widget(button,"spinbutton_addj");
ann = lookup_widget(button,"spinbutton_addy");

combobox1=lookup_widget(button,"comboboxentry_addm");

strcpy(b.id,gtk_entry_get_text(GTK_ENTRY(idd)));
strcpy(b.nom,gtk_entry_get_text(GTK_ENTRY(nm)));

strcpy(b.ville,gtk_entry_get_text(GTK_ENTRY(vil)));



b.jour=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (jr));
b.annee=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (ann));




if(x==1)
{strcpy(b.type,"Gym-traditionel");}
else if(x==2)
{strcpy(b.type,"Gym-Haute-Gamme");}



if ((strcmp(b.nom,"")==0) || (strcmp(b.id,"")==0) ){
output1=lookup_widget(button,"label_nn");
gtk_label_set_text(GTK_LABEL(output1), "Données manquantes. \n Veuillez remplir tout les champs!");
output=lookup_widget(button,"label_succ");
gtk_label_set_text(GTK_LABEL(output), "");
}
if (combobox1 != NULL) {
        gchar *selected_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox1));

        if (selected_text != NULL) {
            // Affiche le mois sélectionné dans la console (debug)
            g_print("mois sélectionné : %s\n", selected_text);

            // Copie le genre sélectionné dans la structure c
            strcpy(b.mois, selected_text);

            g_free(selected_text);
        } else {
            // Aucune option sélectionnée dans la combobox
            // Faites quelque chose en conséquence
           z=0;
        }


if(y==1){
ajouter_centre(b,"centre.txt");

output1=lookup_widget(button,"label_nn");
gtk_label_set_text(GTK_LABEL(output1), "");
output=lookup_widget(button,"label_succ");
gtk_label_set_text(GTK_LABEL(output), "Ajouter avec Succes");
}
x=0;
y=0;
z=0;
}
}


void
on_radiobutton_trm_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON (togglebutton)))
{l=1;}
}


void
on_radiobutton_hgm_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON (togglebutton)))
{l=2;}
}


void
on_button_mod_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
centre b1;
GtkWidget *windowajout,*idd1,*nm1,*jr1,*vil1,*ann1,*typ1,*combobox2,*output2;
windowajout=create_Gestion_centre();

idd1= lookup_widget(button,"entry_idcm");
nm1= lookup_widget(button,"entry_ncm");
vil1= lookup_widget(button,"entry_pm");


jr1 = lookup_widget(button,"spinbutton_jcm");
ann1 = lookup_widget(button,"spinbutton_ycm");

combobox2=lookup_widget(button,"comboboxentry_cmm");
strcpy(b1.id,gtk_entry_get_text(GTK_ENTRY(idd1)));
strcpy(b1.nom,gtk_entry_get_text(GTK_ENTRY(nm1)));

strcpy(b1.ville,gtk_entry_get_text(GTK_ENTRY(vil1)));



b1.jour=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (jr1));
b1.annee=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (ann1));



if(l==1)
{strcpy(b1.type,"Gym-Traditionel");}
else if(l==2)
{strcpy(b1.type,"Gym-Haute-Gamme");}




if (combobox2 != NULL) {
        gchar *selected_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox2));

        if (selected_text != NULL) {
            // Affiche le genre sélectionné dans la console (debug)
            g_print("mois sélectionné : %s\n", selected_text);

            // Copie le genre sélectionné dans la structure c
            strcpy(b1.mois, selected_text);

            g_free(selected_text);
        } else {
            // Aucune option sélectionnée dans la combobox
            // Faites quelque chose en conséquence
           n=0;
        }


if(m==1){
modifier_centre(b1.id,b1,"centre.txt");
output2=lookup_widget(button,"label_mod");
gtk_label_set_text(GTK_LABEL(output2), "Modifier avec Succes");
}
l=0;
m=0;
n=0;
}
}


void
on_checkbutton_confmod_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON (togglebutton)))
{m=1;}
}


void
on_button_supp_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
char iddd[20];
GtkWidget *windowgestionfiche,*tree, *idd,*output;
idd = lookup_widget(button,"entry_suppc");
strcpy(iddd,gtk_entry_get_text(GTK_ENTRY(idd)));
supprimer_centre(iddd,"centre.txt");
windowgestionfiche=lookup_widget(button,"Gestion_centre");
tree=lookup_widget(windowgestionfiche,"treeview1");
afficher_centre(tree);
output=lookup_widget(button,"label_supp");
gtk_label_set_text(GTK_LABEL(output), "Supprimer avec Succes");
gtk_widget_hide(windowgestionfiche);
gtk_widget_show(windowgestionfiche);
}


void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
GtkTreeIter iter;
gchar* id;
gchar* nom;
gchar* jour;

gchar* mois;
gchar* annee;
gchar* ville;
gchar* type;


centre b;
GtkTreeModel *model= gtk_tree_view_get_model(treeview);
}


void
on_button_rechc_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
char id[20];
int c; 
GtkWidget *idd,*tree,*windowrech,*output;
windowrech=lookup_widget(button,"Gestion_centre");
idd = lookup_widget(button,"entry_rechc");
strcpy(id,gtk_entry_get_text(GTK_ENTRY(idd)));
c=rechercher_centre(id);
if (c==0)
{
	output=lookup_widget(button,"label_rech");
	gtk_label_set_text(GTK_LABEL(output), "fiche INTROUVABLE");
}
if (c==1)
{	
tree=lookup_widget(windowrech,"treeview1");
afficher_rechercher_centre(tree);
}
}


void
on_button_affc_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *tree,*window_aff;
window_aff=lookup_widget(button,"Gestion_centre");
gtk_widget_destroy(window_aff);
window_aff=create_Gestion_centre();
tree=lookup_widget(window_aff,"treeview1");

afficher_centre(tree);

gtk_widget_hide(window_aff);
gtk_widget_show(window_aff);
}


void
on_button_tradd_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
trainer x;
GtkWidget *windowajout,*idd,*nm,*prnm,*output,*output1;
windowajout=create_Gestion_centre();

idd= lookup_widget(button,"entry_idctr");
nm= lookup_widget(button,"entry_nomtr");
prnm= lookup_widget(button,"entry_prtr");





strcpy(x.id,gtk_entry_get_text(GTK_ENTRY(idd)));
strcpy(x.nom,gtk_entry_get_text(GTK_ENTRY(nm)));

strcpy(x.prenom,gtk_entry_get_text(GTK_ENTRY(prnm)));










if ((strcmp(x.nom,"")==0) || (strcmp(x.id,"")==0) ){
output1=lookup_widget(button,"label_trnn");
gtk_label_set_text(GTK_LABEL(output1), "Données manquantes. \n Veuillez remplir tout les champs!");
output=lookup_widget(button,"label_trsucc");
gtk_label_set_text(GTK_LABEL(output), "");
}


ajouter_trainer(x,"trainer.txt");



}





void
on_button_searchtr_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *tree,*window_aff;
window_aff=lookup_widget(button,"window_afftrainer");
gtk_widget_destroy(window_aff);
window_aff=create_window_afftrainer();
tree=lookup_widget(window_aff,"treeview2");

afficher_trainer(tree);

gtk_widget_hide(window_aff);
gtk_widget_show(window_aff);
}


void
on_button_afftr_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window1,*window2;
	window1 = lookup_widget(button,"Gestion_centre");
	window2 = create_window_afftrainer ();
	
	gtk_widget_show(window2);
	gtk_widget_destroy(window1);
}


void
on_treeview2_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
GtkTreeIter iter;
gchar* id;
gchar* nom;
gchar* prenom;


trainer x;
GtkTreeModel *model= gtk_tree_view_get_model(treeview);
}


void
on_buttoncalcul_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *calendarD = lookup_widget(button , "calendar2");
    GtkWidget *calendarF = lookup_widget(button , "calendar1");
    GtkWidget *textview = lookup_widget(button , "textviewcalcul");

    // récupérer les dates 
    unsigned int annee1, mois1, jour1;
    unsigned int annee2, mois2, jour2;

    gtk_calendar_get_date(GTK_CALENDAR(calendarD), &annee1, &mois1, &jour1);
    gtk_calendar_get_date(GTK_CALENDAR(calendarF), &annee2, &mois2, &jour2);

    /* gtk_calendar retourne mois = 0 à 11 → on ajoute 1 */
    mois1 += 1;
    mois2 += 1;

    GDate date1;
    GDate date2;
    g_date_set_dmy(&date1, jour1, mois1, annee1);
    g_date_set_dmy(&date2, jour2, mois2, annee2);

    /* --- CALCUL pourcentages HG / TR --- */
    int nb_hg = 0, nb_tr = 0, total = 0;
    FILE *f = fopen("centre.txt", "r");
    if(f != NULL) {
        char id[20], nom[20], mois[20], ville[20], type[30];
        int jour, annee;
        while(fscanf(f,"%s %s %d %s %d %s %s\n", id, nom, &jour, mois, &annee, ville, type) != EOF) {
            if(strcmp(type, "Gym-Haute-Gamme") == 0) nb_hg++;
            else if(strcmp(type, "Gym-Traditionel") == 0) nb_tr++;
        }
        fclose(f);
    }
    total = nb_hg + nb_tr;
    float pour_hg = (total > 0) ? (nb_hg * 100.0 / total) : 0;
    float pour_tr = (total > 0) ? (nb_tr * 100.0 / total) : 0;

    /* Construction du texte d’affichage */
    char affichage[200];
    sprintf(affichage,
            "Date début : %02d/%02d/%04d\n"
            "Date fin   : %02d/%02d/%04d\n\n",
            jour1, mois1, annee1,
            jour2, mois2, annee2);

    /* Texte final */
    char texte_final[200];
    sprintf(texte_final,
            "Statistiques du centre :\n"
            "- HG : %.2f%% (%d)\n"
            "- TR : %.2f%% (%d)\n",
            pour_hg, nb_hg,
            pour_tr, nb_tr);

    /* Fusion affichage + texte_final */
    char affichage2[400];
    strcpy(affichage2, affichage);
    strcat(affichage2, texte_final);

    /* Affichage dans TextView */
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_set_text(buffer, affichage2, -1);
}



