#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include"centre.h"
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"




int type_centre = 0;   // 1 = Haut gamme, 2 = Traditionnel
GdkColor centre_color;


int type_trainer = 0; // global pour radio buttons


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



// Vérification des champs obligatoires
if ((strcmp(b.nom,"") == 0) || (strcmp(b.id,"") == 0)) {
    GtkWidget *output1 = lookup_widget(button, "label_nn");
    gtk_label_set_text(GTK_LABEL(output1),
        "Données manquantes.\nVeuillez remplir tous les champs !");
    
    GtkWidget *output = lookup_widget(button, "label_succ");
    gtk_label_set_text(GTK_LABEL(output), "");

    return; // 🔴 Empêche l’ajout si un champ est vide
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
on_button_supp_clicked(GtkButton *button, gpointer user_data)
{
    char iddd[20];
    GtkWidget *windowgestionfiche, *tree, *idd, *output;
    GtkWidget *textview_raison, *label_raison;
    gchar *texte_raison;

    // Récupérer l'ID du centre à supprimer
    idd = lookup_widget(button, "entry_suppc");
    strcpy(iddd, gtk_entry_get_text(GTK_ENTRY(idd)));

    // Supprimer le centre
    supprimer_centre(iddd, "centre.txt");

    // Mettre à jour le TreeView
    windowgestionfiche = lookup_widget(button, "Gestion_centre");
    tree = lookup_widget(windowgestionfiche, "treeview1");
    afficher_centre(tree);

    // Afficher le message de succès
    output = lookup_widget(button, "label_supp");
    gtk_label_set_text(GTK_LABEL(output), "Supprimé avec succès");

    // --- Gestion de l'Expander pour la raison ---
    textview_raison = lookup_widget(button, "textview_raison"); // TextView dans l'Expander
    label_raison = lookup_widget(button, "label_raison");       // Label pour afficher la raison

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview_raison));
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);

    texte_raison = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    // Afficher la raison dans le label
    gtk_label_set_text(GTK_LABEL(label_raison), texte_raison);

    // Rafraîchir la fenêtre
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


    
       



// Ajouter un trainer
void on_button_tradd_clicked(GtkButton *button, gpointer user_data){
    trainer x;
    GtkWidget *idd, *nm, *prnm, *combobox, *output, *output1;

    idd = lookup_widget(button, "entry_idctr");
    nm = lookup_widget(button, "entry_nomtr");
    prnm = lookup_widget(button, "entry_prtr");
    combobox = lookup_widget(button, "combobox_trainer_type");
    output1 = lookup_widget(button, "label_trnn");
    output = lookup_widget(button, "label_trsucc");

    // Récupérer les données
    strcpy(x.id, gtk_entry_get_text(GTK_ENTRY(idd)));
    strcpy(x.nom, gtk_entry_get_text(GTK_ENTRY(nm)));
    strcpy(x.prenom, gtk_entry_get_text(GTK_ENTRY(prnm)));

    // Récupérer le type sélectionné dans le combobox
    GtkTreeIter iter_cb;
    GtkTreeModel *model_cb = gtk_combo_box_get_model(GTK_COMBO_BOX(combobox));
    if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combobox), &iter_cb)) {
        gchar *type_selected;
        gtk_tree_model_get(model_cb, &iter_cb, 0, &type_selected, -1);
        strcpy(x.type, type_selected);
        g_free(type_selected);
    } else {
        strcpy(x.type, "Inconnu");
    }

    // Vérification des champs obligatoires
    if (strcmp(x.id, "") == 0 || strcmp(x.nom, "") == 0) {
        gtk_label_set_text(GTK_LABEL(output1), "Données manquantes. Veuillez remplir tous les champs !");
        gtk_label_set_text(GTK_LABEL(output), "");
        return;
    }

    // Ajouter le trainer dans le fichier
    ajouter_trainer(x, "trainer.txt");

    gtk_label_set_text(GTK_LABEL(output), "Trainer ajouté avec succès !");
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






    











   


void on_buttoncalcul_clicked(GtkButton *button, gpointer user_data)
{
    /* ===== Récupération des widgets ===== */
    GtkWidget *calendarD = lookup_widget(button, "calendar2");
    GtkWidget *calendarF = lookup_widget(button, "calendar1");
    GtkWidget *textview  = lookup_widget(button, "textviewcalcul");
    GtkWidget *clist     = lookup_widget(button, "clist_stat");

    /* ===== Récupération des dates ===== */
    unsigned int annee1, mois1, jour1;
    unsigned int annee2, mois2, jour2;

    gtk_calendar_get_date(GTK_CALENDAR(calendarD), &annee1, &mois1, &jour1);
    gtk_calendar_get_date(GTK_CALENDAR(calendarF), &annee2, &mois2, &jour2);

    mois1 += 1;  // correction GTK
    mois2 += 1;

    /* ===== CALCUL HG / TR ===== */
    int nb_hg = 0, nb_tr = 0, total = 0;
    FILE *f = fopen("centre.txt", "r");

    if (f != NULL)
    {
        char id[20], nom[30], mois[20], ville[20], type[30];
        int jour, annee;

        while (fscanf(f, "%s %s %d %s %d %s %s\n",
                      id, nom, &jour, mois, &annee, ville, type) != EOF)
        {
            if (strcmp(type, "Gym-Haute-Gamme") == 0) nb_hg++;
            else if (strcmp(type, "Gym-Traditionel") == 0) nb_tr++;
        }
        fclose(f);
    }

    total = nb_hg + nb_tr;
    float pour_hg = (total > 0) ? (nb_hg * 100.0 / total) : 0;
    float pour_tr = (total > 0) ? (nb_tr * 100.0 / total) : 0;

    /* ===== Affichage TextView ===== */
    char affichage[400];
    sprintf(affichage,
            "Date début : %02d/%02d/%04d\n"
            "Date fin   : %02d/%02d/%04d\n\n"
            "Statistiques du centre :\n"
            "- HG : %.2f%% (%d)\n"
            "- TR : %.2f%% (%d)\n",
            jour1, mois1, annee1,
            jour2, mois2, annee2,
            pour_hg, nb_hg,
            pour_tr, nb_tr);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_set_text(buffer, affichage, -1);

    /* ===== CLASSEMENT DES TRAINERS (PAR TYPE) ===== */

    int nb_coachp = 0, nb_assistant = 0;
    int nb_personnel = 0, nb_nutrition = 0, nb_sante = 0;

    /* --- 1er passage : compter --- */
    f = fopen("trainer.txt", "r");
    if (f != NULL)
    {
        char id[20], nom[20], prenom[20], type[30];

        while (fscanf(f, "%s %s %s %s\n", id, nom, prenom, type) != EOF)
        {
            if (strcmp(type, "CoachPrincipal") == 0) nb_coachp++;
            else if (strcmp(type, "AssistantCoach") == 0) nb_assistant++;
            else if (strcmp(type, "CoachPersonnel") == 0) nb_personnel++;
            else if (strcmp(type, "Nutritionniste") == 0) nb_nutrition++;
            else if (strcmp(type, "CoachSanté") == 0) nb_sante++;
        }
        fclose(f);
    }

    /* --- 2e passage : affichage GtkCList --- */
    gtk_clist_clear(GTK_CLIST(clist));

    f = fopen("trainer.txt", "r");
    if (f != NULL)
    {
        char id[20], nom[20], prenom[20], type[30];
        char classement[10];

        while (fscanf(f, "%s %s %s %s\n", id, nom, prenom, type) != EOF)
        {
            if (strcmp(type, "CoachPrincipal") == 0)
                sprintf(classement, "%d", nb_coachp);
            else if (strcmp(type, "AssistantCoach") == 0)
                sprintf(classement, "%d", nb_assistant);
           
            else if (strcmp(type, "CoachPersonnel") == 0)
                sprintf(classement, "%d", nb_personnel);
            else if (strcmp(type, "Nutritionniste") == 0)
                sprintf(classement, "%d", nb_nutrition);
            else if (strcmp(type, "CoachSanté") == 0)
                sprintf(classement, "%d", nb_sante);
            else
                strcpy(classement, "0");

            gchar *row[3] = { id, type, classement };
            gtk_clist_append(GTK_CLIST(clist), row);
        }
        fclose(f);
    }
}







void
on_combobox_trainer_type_changed       (GtkComboBox     *combobox,
                                        gpointer         user_data)
{

}











// Signal pour le ToolButton "A propos"
void
on_toolbutton_about_clicked            (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{


    GtkWidget *about_dialog;

    // Créer le dialog "About"
    about_dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about_dialog), "Iron Core");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about_dialog), "Application pour gérer les centres et les trainers.");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(about_dialog), "© 2025 IronCore");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about_dialog), "https://www.ironcore.tn");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(about_dialog), "Site officiel Iron Core");
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about_dialog), (const gchar*[]){"Maram Riahi", NULL});
    gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(about_dialog), TRUE);
    gtk_about_dialog_set_translator_credits(GTK_ABOUT_DIALOG(about_dialog), "Maram Riahi");

    // Afficher le dialog
    gtk_widget_show(about_dialog);

    // Connecter la fermeture au signal response
    g_signal_connect(about_dialog, "response",
                     G_CALLBACK(on_about_dialog_centre_response), NULL);


}


void
on_about_dialog_centre_response        (GtkDialog       *dialog,
                                        gint             response_id,
                                        gpointer         user_data)
{


    gtk_widget_destroy(GTK_WIDGET(dialog));

}








void
on_button_update_progress_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{



    GtkWidget *window_aff = lookup_widget(button, "Gestion_centre");
    if (!window_aff) return;

    GtkWidget *progress_hg = lookup_widget(window_aff, "progressbar_hg");
    GtkWidget *progress_tr = lookup_widget(window_aff, "progressbar_tr");

    // Calcul des pourcentages HG/TR
    int nb_hg = 0, nb_tr = 0;
    FILE *f = fopen("centre.txt", "r");
    if(f != NULL) {
        char id[20], nom[30], mois[20], ville[20], type[30];
        int jour, annee;
        while(fscanf(f,"%s %s %d %s %d %s %s\n", id, nom, &jour, mois, &annee, ville, type) != EOF) {
            if(strcmp(type, "Gym-Haute-Gamme") == 0) nb_hg++;
            else if(strcmp(type, "Gym-Traditionel") == 0) nb_tr++;
        }
        fclose(f);
    }

    int total = nb_hg + nb_tr;
    gdouble fraction_hg = (total > 0) ? ((gdouble)nb_hg / total) : 0;
    gdouble fraction_tr = (total > 0) ? ((gdouble)nb_tr / total) : 0;

    // Mettre à jour les ProgressBars
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_hg), fraction_hg);
    char txt_hg[30]; sprintf(txt_hg, "Haute Gamme %.0f%%", fraction_hg*100);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_hg), txt_hg);

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_tr), fraction_tr);
    char txt_tr[30]; sprintf(txt_tr, "Traditionnel %.0f%%", fraction_tr*100);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_tr), txt_tr);



}



















void on_radiobutton_coach_principal_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton))
        type_trainer = 1;
}

void on_radiobutton_assistant_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton))
        type_trainer = 2;
}

void on_radiobutton_personnel_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton))
        type_trainer = 3;
}

void on_radiobutton_nutrition_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton))
        type_trainer = 4;
}

void on_radiobutton_sante_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton))
        type_trainer = 5;
}

void on_button_afficher_expander_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *expander = lookup_widget(GTK_WIDGET(button), "expander_trainer");

    GtkWidget *label_salaire_val = lookup_widget(expander, "label_salaire_val");
    GtkWidget *label_heure_val   = lookup_widget(expander, "label_heure_val");
    GtkWidget *label_repos_val   = lookup_widget(expander, "label_repos_val");
    GtkWidget *label_taches_val  = lookup_widget(expander, "label_val_taches");
    switch(type_trainer) {
        case 1:
            gtk_label_set_text(GTK_LABEL(label_salaire_val), "2500 TND / mois");
            gtk_label_set_text(GTK_LABEL(label_heure_val), "40h / semaine");
            gtk_label_set_text(GTK_LABEL(label_repos_val), "Samedi-Dimanche");
            gtk_label_set_text(GTK_LABEL(label_taches_val), "Encadrer les coachs, planifier les programmes");
            break;
        case 2:
            gtk_label_set_text(GTK_LABEL(label_salaire_val), "1800 TND / mois");
            gtk_label_set_text(GTK_LABEL(label_heure_val), "35h / semaine");
            gtk_label_set_text(GTK_LABEL(label_repos_val), "Dimanche");
            gtk_label_set_text(GTK_LABEL(label_taches_val), "Aider le coach principal, suivre les membres"); 
            break;
        case 3:
            gtk_label_set_text(GTK_LABEL(label_salaire_val), "2000 TND / mois");
            gtk_label_set_text(GTK_LABEL(label_heure_val), "30h / semaine");
            gtk_label_set_text(GTK_LABEL(label_repos_val), "Lundi");
            gtk_label_set_text(GTK_LABEL(label_taches_val), "Suivre individuellement les clients"); 
            break;
        case 4:
            gtk_label_set_text(GTK_LABEL(label_salaire_val), "2200 TND / mois");
            gtk_label_set_text(GTK_LABEL(label_heure_val), "35h / semaine");
            gtk_label_set_text(GTK_LABEL(label_repos_val), "Samedi");
            gtk_label_set_text(GTK_LABEL(label_taches_val), "Conseiller sur alimentation et diètes"); 
            break;
        case 5:
            gtk_label_set_text(GTK_LABEL(label_salaire_val), "2400 TND / mois");
            gtk_label_set_text(GTK_LABEL(label_heure_val), "40h / semaine");
            gtk_label_set_text(GTK_LABEL(label_repos_val), "Vendredi");
            gtk_label_set_text(GTK_LABEL(label_taches_val), "Suivi santé et prévention des blessures"); 
            break;
        default:
            gtk_label_set_text(GTK_LABEL(label_salaire_val), "-");
            gtk_label_set_text(GTK_LABEL(label_heure_val), "-");
            gtk_label_set_text(GTK_LABEL(label_repos_val), "-");
            gtk_label_set_text(GTK_LABEL(label_taches_val), "-"); 
            break;
    }

    gtk_expander_set_expanded(GTK_EXPANDER(expander), TRUE); // ouvre l'expander
}











void
on_radiobutton_haut_gamme_toggled(GtkToggleButton *togglebutton,
                                  gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton))
        type_centre = 1;
}


void
on_radiobutton_traditionnel_toggled(GtkToggleButton *togglebutton,
                                    gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton))
        type_centre = 2;
}



void
on_color_centre_color_set(GtkColorButton *colorbutton,
                          gpointer user_data)
{
    gtk_color_button_get_color(colorbutton, &centre_color);
}





void
on_button_afficher_proprietes_clicked(GtkButton *button,
                                      gpointer user_data)
{
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));

    GtkWidget *expander = lookup_widget(window, "expander_proprietes");

    GtkWidget *label_capacite      = lookup_widget(window, "label_capacite_val");
    GtkWidget *label_salles        = lookup_widget(window, "label_salles_val");
    GtkWidget *label_equipements   = lookup_widget(window, "label_equipements_val");
    GtkWidget *label_horaires      = lookup_widget(window, "label_horaires_val");
    GtkWidget *label_securite      = lookup_widget(window, "label_securite_val");
    GtkWidget *label_hygiene       = lookup_widget(window, "label_hygiene_val");
    GtkWidget *label_prix          = lookup_widget(window, "label_prix_val");
    GtkWidget *label_services      = lookup_widget(window, "label_services_val");
    GtkWidget *label_accessibilite = lookup_widget(window, "label_accessibilite_val");
    GtkWidget *label_parking       = lookup_widget(window, "label_parking_val");
    GtkWidget *label_climatisation = lookup_widget(window, "label_climatisation_val");

    if (type_centre == 1) {  // Haut gamme
        gtk_label_set_text(GTK_LABEL(label_capacite), "50 personnes");
        gtk_label_set_text(GTK_LABEL(label_salles), "5 salles");
        gtk_label_set_text(GTK_LABEL(label_equipements), "Équipements complets");
        gtk_label_set_text(GTK_LABEL(label_horaires), "08h - 20h");
        gtk_label_set_text(GTK_LABEL(label_securite), "Sécurité élevée");
        gtk_label_set_text(GTK_LABEL(label_hygiene), "Hygiène impeccable");
        gtk_label_set_text(GTK_LABEL(label_prix), "2000 TND / mois");
        gtk_label_set_text(GTK_LABEL(label_services), "Spa, Bar, Coaching VIP");
        gtk_label_set_text(GTK_LABEL(label_accessibilite), "Très accessible");
        gtk_label_set_text(GTK_LABEL(label_parking), "Parking privé");
        gtk_label_set_text(GTK_LABEL(label_climatisation), "Oui");
    }
    else if (type_centre == 2) {  // Traditionnel
        gtk_label_set_text(GTK_LABEL(label_capacite), "30 personnes");
        gtk_label_set_text(GTK_LABEL(label_salles), "3 salles");
        gtk_label_set_text(GTK_LABEL(label_equipements), "Équipements standards");
        gtk_label_set_text(GTK_LABEL(label_horaires), "06h - 18h");
        gtk_label_set_text(GTK_LABEL(label_securite), "Sécurité normale");
        gtk_label_set_text(GTK_LABEL(label_hygiene), "Hygiène correcte");
        gtk_label_set_text(GTK_LABEL(label_prix), "1000 TND / mois");
        gtk_label_set_text(GTK_LABEL(label_services), "Aucun");
        gtk_label_set_text(GTK_LABEL(label_accessibilite), "Moyenne");
        gtk_label_set_text(GTK_LABEL(label_parking), "Non");
        gtk_label_set_text(GTK_LABEL(label_climatisation), "Non");
    }

    // Appliquer la couleur choisie
    gtk_widget_modify_bg(expander, GTK_STATE_NORMAL, &centre_color);

    // Ouvrir l’expander
    gtk_expander_set_expanded(GTK_EXPANDER(expander), TRUE);
}






