#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "equipement.h"

#define FICHIER_EQUIPEMENTS "equipements.txt"
#define FICHIER_RESERVATIONS "reservations.txt"

// Fonction pour convertir l'état en chaîne (pour affichage)
const char* etat_to_string(int etat) {
    switch(etat) {
        case DISPO: return "DISPO";
        case RESERVE: return "RESERVE";
        case MAINT: return "MAINT";
        default: return "INCONNU";
    }
}
// Fonction pour convertir la localisation en chaîne (pour affichage)
const char* localisation_to_string(int loc) {
    switch(loc) {
        case DANS_LA_SALLE: return "Dans la salle";
        case EN_STOCK: return "En stock";
        default: return "INCONNU";
    }
}

// Fonction pour convertir l'état en chaîne pour le fichier
const char* etat_to_file_string(int etat) {
    switch(etat) {
        case DISPO: return "DISPO";
        case RESERVE: return "RESERVE";
        case MAINT: return "MAINT";
        default: return "DISPO";
    }
}

// Fonction pour convertir la localisation en chaîne pour le fichier
const char* localisation_to_file_string(int loc) {
    switch(loc) {
        case DANS_LA_SALLE: return "DANS_LA_SALLE";
        case EN_STOCK: return "EN_STOCK";
        default: return "DANS_LA_SALLE";
    }
}

// Fonction pour convertir une chaîne en état
int string_to_etat(const char* str) {
    if (strcmp(str, "DISPO") == 0) return DISPO;
    if (strcmp(str, "RESERVE") == 0) return RESERVE;
    if (strcmp(str, "MAINT") == 0) return MAINT;
    return DISPO;
}

// Fonction pour convertir une chaîne en localisation
int string_to_localisation(const char* str) {
    if (strcmp(str, "EN_STOCK") == 0) return EN_STOCK;
    if (strcmp(str, "DANS_LA_SALLE") == 0) return DANS_LA_SALLE;
    return DANS_LA_SALLE;
}


void show_error_message(const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_info_message(const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_warning_message(const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_WARNING,
        GTK_BUTTONS_OK,
        "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void get_current_date(char *date_str) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date_str, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void rafraichir_treeview(GtkWidget *treeview) {
    if (!treeview) return;

    GtkListStore *store = gtk_list_store_new(6,
        G_TYPE_STRING,
        G_TYPE_STRING,
        G_TYPE_STRING,
        G_TYPE_INT,
        G_TYPE_STRING,
        G_TYPE_STRING);

    FILE *f = fopen(FICHIER_EQUIPEMENTS, "r");
    if (!f) return;

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f)) {
        Equipement e;
        char etat_str[50], loc_str[50];
        if (sscanf(ligne, "%[^,],%[^,],%[^,],%d,%[^,],%s",
                   e.reference, e.nom, e.type, &e.quantite,
                   etat_str, loc_str) == 6) {
            e.etat = string_to_etat(etat_str);
            e.localisation = string_to_localisation(loc_str);

            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                0, e.reference,
                1, e.nom,
                2, e.type,
                3, e.quantite,
                4, etat_to_string(e.etat),
                5, localisation_to_string(e.localisation),
                -1);
        }
    }
    fclose(f);

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

void on_treeview1_row_activated(GtkTreeView *treeview, GtkTreePath *path,
                               GtkTreeViewColumn *column, gpointer user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;

    model = gtk_tree_view_get_model(treeview);

    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gchar *ref;
        gtk_tree_model_get(model, &iter, 0, &ref, -1); 

        GtkWidget *dialog;
        GtkWindow *parent = GTK_WINDOW(user_data); 
        gchar *message = g_strdup_printf("Voulez-vous vraiment supprimer l'équipement %s ?", ref);
        dialog = gtk_message_dialog_new(parent,
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_QUESTION,
                                        GTK_BUTTONS_YES_NO,
                                        "%s", message);
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(message);

        if (response == GTK_RESPONSE_YES) {
            gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

            FILE *f = fopen("equipements.txt", "r");
            FILE *temp = fopen("equipements_temp.txt", "w");
            char line[256];
            if (f && temp) {
                while (fgets(line, sizeof(line), f)) {

                    if (strncmp(line, ref, strlen(ref)) != 0) {
                        fputs(line, temp);
                    }
                }
                fclose(f);
                fclose(temp);

                remove("equipements.txt");
                rename("equipements_temp.txt", "equipements.txt");
            } else {
                g_print("Erreur lors de l'ouverture du fichier.\n");
            }
        }

        g_free(ref);
    }
}

void on_buttonRech_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "yh_window");
    GtkWidget *entryRef = lookup_widget(window, "entryRef");
    GtkWidget *entryNom = lookup_widget(window, "entryNom");
    GtkWidget *combo_entry2 = lookup_widget(window, "combo_entry2");
    GtkWidget *spinbutton2 = lookup_widget(window, "spinbutton2");
    GtkWidget *radiobutton1 = lookup_widget(window, "radiobutton1");
    GtkWidget *radiobutton2 = lookup_widget(window, "radiobutton2");
    GtkWidget *radiobutton3 = lookup_widget(window, "radiobutton3");
    GtkWidget *checkbutton1 = lookup_widget(window, "checkbutton1");
    GtkWidget *checkbutton2 = lookup_widget(window, "checkbutton2");
    
    if (!entryRef) {
        show_error_message("Widget entryRef non trouvé");
        return;
    }
    
    const gchar *reference = gtk_entry_get_text(GTK_ENTRY(entryRef));
    if (strlen(reference) == 0) {
        show_warning_message("Veuillez entrer une référence à rechercher");
        return;
    }
    
    FILE *f = fopen(FICHIER_EQUIPEMENTS, "r");
    if (!f) {
        show_error_message("Impossible d'ouvrir le fichier des équipements");
        return;
    }
    
    Equipement e;
    char ligne[512];
    char etat_str[50], loc_str[50];
    int trouve = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%[^,],%[^,],%[^,],%d,%[^,],%s",
                   e.reference, e.nom, e.type, &e.quantite,
                   etat_str, loc_str) == 6) {
            if (strcmp(e.reference, reference) == 0) {
                trouve = 1;
                
                e.etat = string_to_etat(etat_str);
                e.localisation = string_to_localisation(loc_str);
                
                // Mettre à jour les champs de l'interface
                if (entryNom)
                    gtk_entry_set_text(GTK_ENTRY(entryNom), e.nom);
                
                if (combo_entry2)
                    gtk_entry_set_text(GTK_ENTRY(combo_entry2), e.type);
                
                if (spinbutton2)
                    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton2), e.quantite);
                
                // Mettre à jour les boutons radio
                if (radiobutton1 && radiobutton2 && radiobutton3) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton1), FALSE);
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton2), FALSE);
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton3), FALSE);
                    
                    switch(e.etat) {
                        case MAINT:
                            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton1), TRUE);
                            break;
                        case DISPO:
                            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton2), TRUE);
                            break;
                        case RESERVE:
                            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton3), TRUE);
                            break;
                    }
                }
                
                // Mettre à jour les cases à cocher
                if (checkbutton1 && checkbutton2) {
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), FALSE);
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton2), FALSE);
                    
                    if (e.localisation == EN_STOCK) {
                        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), TRUE);
                    } else if (e.localisation == DANS_LA_SALLE) {
                        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton2), TRUE);
                    }
                }
                
                break;
            }
        }
    }
    fclose(f);
    
    if (trouve) {
        show_info_message("Équipement trouvé et chargé");
    } else {
        show_info_message("Équipement non trouvé");
    }
}

void on_buttonModif_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "yh_window");
    GtkWidget *entryRef = lookup_widget(window, "entryRef");
    GtkWidget *entryNom = lookup_widget(window, "entryNom");
    GtkWidget *combo_entry2 = lookup_widget(window, "combo_entry2");
    GtkWidget *spinbutton2 = lookup_widget(window, "spinbutton2");
    GtkWidget *radiobutton1 = lookup_widget(window, "radiobutton1");
    GtkWidget *radiobutton2 = lookup_widget(window, "radiobutton2");
    GtkWidget *radiobutton3 = lookup_widget(window, "radiobutton3");
    GtkWidget *checkbutton1 = lookup_widget(window, "checkbutton1");
    GtkWidget *checkbutton2 = lookup_widget(window, "checkbutton2");
    
    if (!entryRef) return;
    
    const gchar *reference = gtk_entry_get_text(GTK_ENTRY(entryRef));
    if (strlen(reference) == 0) {
        show_warning_message("Veuillez d'abord rechercher l'équipement à modifier");
        return;
    }
    
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entryNom));
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(combo_entry2));
    gint quantite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton2));
    
    if (strlen(nom) == 0 || strlen(type) == 0) {
        show_warning_message("Veuillez remplir tous les champs");
        return;
    }
    
    if (quantite <= 0) {
        show_warning_message("La quantité doit être positive");
        return;
    }
    
    int etat = DISPO;
    if (radiobutton1 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton1))) {
        etat = MAINT;
    } else if (radiobutton3 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton3))) {
        etat = RESERVE;
    }

    int localisation = DANS_LA_SALLE;
    if (checkbutton1 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton1))) {
        localisation = EN_STOCK;
    }
    
    FILE *f = fopen(FICHIER_EQUIPEMENTS, "r");
    FILE *f_temp = fopen("temp_equip.txt", "w");
    
    if (!f || !f_temp) {
        if (f) fclose(f);
        if (f_temp) fclose(f_temp);
        show_error_message("Erreur lors de l'ouverture des fichiers");
        return;
    }
    
    char ligne[512];
    Equipement e;
    char etat_str[50], loc_str[50];
    int trouve = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%[^,],%[^,],%[^,],%d,%[^,],%s",
                   e.reference, e.nom, e.type, &e.quantite,
                   etat_str, loc_str) == 6) {
            if (strcmp(e.reference, reference) == 0) {
                // Écrire les nouvelles valeurs
                fprintf(f_temp, "%s,%s,%s,%d,%s,%s\n",
                        reference, nom, type, quantite,
                        etat_to_file_string(etat),
                        localisation_to_file_string(localisation));
                trouve = 1;
            } else {
                // Réécrire la ligne telle quelle
                fprintf(f_temp, "%s", ligne);
            }
        }
    }
    
    fclose(f);
    fclose(f_temp);
    
    if (trouve) {
    remove(FICHIER_EQUIPEMENTS);
    rename("temp_equip.txt", FICHIER_EQUIPEMENTS);
    show_info_message("Équipement modifié avec succès");

    GtkWidget *treeview = lookup_widget(window, "treeview1");
    if (treeview)
        rafraichir_treeview(treeview);

    } else {
    remove("temp_equip.txt");
    show_error_message("Équipement non trouvé");
    }
   
}


void on_buttonAjout_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "yh_window");
    GtkWidget *entryRef = lookup_widget(window, "entryRef");
    GtkWidget *entryNom = lookup_widget(window, "entryNom");
    GtkWidget *combo_entry2 = lookup_widget(window, "combo_entry2");
    GtkWidget *spinbutton2 = lookup_widget(window, "spinbutton2");
    GtkWidget *radiobutton1 = lookup_widget(window, "radiobutton1");
    GtkWidget *radiobutton2 = lookup_widget(window, "radiobutton2");
    GtkWidget *radiobutton3 = lookup_widget(window, "radiobutton3");
    GtkWidget *checkbutton1 = lookup_widget(window, "checkbutton1");
    GtkWidget *checkbutton2 = lookup_widget(window, "checkbutton2");
    
    if (!entryRef || !entryNom || !combo_entry2 || !spinbutton2) {
        show_error_message("Widgets non trouvés");
        return;
    }
    
    const gchar *reference = gtk_entry_get_text(GTK_ENTRY(entryRef));
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entryNom));
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(combo_entry2));
    gint quantite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton2));
    
    if (strlen(reference) == 0 || strlen(nom) == 0 || strlen(type) == 0) {
        show_warning_message("Veuillez remplir tous les champs obligatoires");
        return;
    }
    
    if (quantite <= 0) {
        show_warning_message("La quantité doit être positive");
        return;
    }
    
    FILE *f_check = fopen(FICHIER_EQUIPEMENTS, "r");
    if (f_check) {
        char ligne[512];
        char ref_existante[100];
        while (fgets(ligne, sizeof(ligne), f_check)) {
            if (sscanf(ligne, "%[^,]", ref_existante) == 1) {
                if (strcmp(ref_existante, reference) == 0) {
                    fclose(f_check);
                    show_warning_message("Un équipement avec cette référence existe déjà");
                    return;
                }
            }
        }
        fclose(f_check);
    }
    
    int etat = DISPO;
    if (radiobutton1 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton1))) {
        etat = MAINT;
    } else if (radiobutton3 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton3))) {
        etat = RESERVE;
    }
    
    int localisation = DANS_LA_SALLE;
    if (checkbutton1 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton1))) {
        localisation = EN_STOCK;
    }
    
    FILE *f = fopen(FICHIER_EQUIPEMENTS, "a");
    if (!f) {
        show_error_message("Impossible d'ouvrir le fichier des équipements");
        return;
    }
    
    fprintf(f, "%s,%s,%s,%d,%s,%s\n",
            reference, nom, type, quantite,
            etat_to_file_string(etat),
            localisation_to_file_string(localisation));
    
    fclose(f);
    
    // Réinitialiser les champs
    gtk_entry_set_text(GTK_ENTRY(entryRef), "");
    gtk_entry_set_text(GTK_ENTRY(entryNom), "");
    gtk_entry_set_text(GTK_ENTRY(combo_entry2), "");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton2), 1);
    
    // Réinitialiser les boutons radio et cases à cocher
    if (radiobutton2)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton2), TRUE);
    
    if (checkbutton2)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton2), TRUE);

    GtkWidget *treeview = lookup_widget(window, "treeview1");
    if (treeview)
        rafraichir_treeview(treeview);
    
    show_info_message("Équipement ajouté avec succès");



}

void on_buttonSupp_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "yh_window");
    GtkWidget *entryRef = lookup_widget(window, "entryRef");
    
    if (!entryRef) return;
    
    const gchar *reference = gtk_entry_get_text(GTK_ENTRY(entryRef));
    if (strlen(reference) == 0) {
        show_warning_message("Veuillez entrer une référence à supprimer");
        return;
    }
    
    // Vérifier si l'équipement existe
    FILE *f_check = fopen(FICHIER_EQUIPEMENTS, "r");
    int existe = 0;
    if (f_check) {
        char ligne[512];
        char ref[100];
        while (fgets(ligne, sizeof(ligne), f_check)) {
            if (sscanf(ligne, "%[^,]", ref) == 1) {
                if (strcmp(ref, reference) == 0) {
                    existe = 1;
                    break;
                }
            }
        }
        fclose(f_check);
    }
    
    if (!existe) {
        show_error_message("Équipement non trouvé");
        return;
    }
    
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "Voulez-vous vraiment supprimer l'équipement %s ?", reference);
    
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    if (response == GTK_RESPONSE_YES) {
        FILE *f = fopen(FICHIER_EQUIPEMENTS, "r");
        FILE *f_temp = fopen("temp_supp.txt", "w");
        
        if (!f || !f_temp) {
            if (f) fclose(f);
            if (f_temp) fclose(f_temp);
            show_error_message("Erreur lors de l'ouverture des fichiers");
            return;
        }
        
        char ligne[512];
        char ref[100];
        
        while (fgets(ligne, sizeof(ligne), f)) {
            if (sscanf(ligne, "%[^,]", ref) == 1) {
                if (strcmp(ref, reference) != 0) {

                    fprintf(f_temp, "%s", ligne);
                }
            }
        }
        
        fclose(f);
        fclose(f_temp);
        
        remove(FICHIER_EQUIPEMENTS);
        rename("temp_supp.txt", FICHIER_EQUIPEMENTS);
        
        gtk_entry_set_text(GTK_ENTRY(entryRef), "");
        
        GtkWidget *entryNom = lookup_widget(window, "entryNom");
        GtkWidget *combo_entry2 = lookup_widget(window, "combo_entry2");
        GtkWidget *spinbutton2 = lookup_widget(window, "spinbutton2");
        
        if (entryNom) gtk_entry_set_text(GTK_ENTRY(entryNom), "");
        if (combo_entry2) gtk_entry_set_text(GTK_ENTRY(combo_entry2), "");
        if (spinbutton2) gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton2), 1);

        GtkWidget *treeview = lookup_widget(window, "treeview1");
        if (treeview)
        rafraichir_treeview(treeview);
        
        show_info_message("Équipement supprimé avec succès");

    }
}


void on_radiobutton3_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    // Bouton "RÉSERVÉ"
}

void on_radiobutton1_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    // Bouton "MAINT"
}

void on_radiobutton2_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    // Bouton "DISPO"
}

void on_checkbutton2_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    // "Dans La Salle" - Désactiver "En Stock" si coché
    GtkWidget *window = lookup_widget(GTK_WIDGET(togglebutton), "yh_window");
    GtkWidget *checkbutton1 = lookup_widget(window, "checkbutton1");
    
    if (checkbutton1 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), FALSE);
    }
}

void on_checkbutton1_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    // "En Stock" - Désactiver "Dans La Salle" si coché
    GtkWidget *window = lookup_widget(GTK_WIDGET(togglebutton), "yh_window");
    GtkWidget *checkbutton2 = lookup_widget(window, "checkbutton2");
    
    if (checkbutton2 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton2), FALSE);
    }
}

void on_calendar1_day_selected(GtkCalendar *calendar, gpointer user_data)
{
    guint year, month, day;
    gtk_calendar_get_date(calendar, &year, &month, &day);
}


void
on_buttonVd_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *combo_entry3 = lookup_widget(GTK_WIDGET(button), "combo_entry3");
    GtkWidget *calendar1 = lookup_widget(GTK_WIDGET(button), "calendar1");
    GtkWidget *spinbutton1 = lookup_widget(GTK_WIDGET(button), "spinbutton1");
    GtkWidget *labelAffich = lookup_widget(GTK_WIDGET(button), "labelAffich");
    
    if (!combo_entry3 || !calendar1 || !spinbutton1) return;
    
    const gchar *equipement = gtk_entry_get_text(GTK_ENTRY(combo_entry3));
    gint quantite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton1));
    
    if (strlen(equipement) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK, "Veuillez sélectionner un équipement");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    guint year, month, day;
    gtk_calendar_get_date(GTK_CALENDAR(calendar1), &year, &month, &day);
    
    Date date;
    date.jour = day;
    date.mois = month + 1;
    date.annee = year;
    
    if (verifierDisponibilite(FICHIER_RESERVATIONS, (char*)equipement, date, quantite)) {
        if (labelAffich) {
            char message[200];
            sprintf(message, "✓ %s est disponible en quantité %d pour le %02d/%02d/%d",
                    equipement, quantite, date.jour, date.mois, date.annee);
            gtk_label_set_text(GTK_LABEL(labelAffich), message);
        }
    } else {
        if (labelAffich) {
            char message[200];
            sprintf(message, "✗ %s n'est pas disponible en quantité %d pour le %02d/%02d/%d",
                    equipement, quantite, date.jour, date.mois, date.annee);
            gtk_label_set_text(GTK_LABEL(labelAffich), message);
        }
    }
}


void
on_buttonRes_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *entryCoach = lookup_widget(GTK_WIDGET(button), "entryCoach");
    GtkWidget *combo_entry3 = lookup_widget(GTK_WIDGET(button), "combo_entry3");
    GtkWidget *spinbutton1 = lookup_widget(GTK_WIDGET(button), "spinbutton1");
    GtkWidget *calendar1 = lookup_widget(GTK_WIDGET(button), "calendar1");
    GtkWidget *labelAffich = lookup_widget(GTK_WIDGET(button), "labelAffich");
    
    if (!entryCoach || !combo_entry3 || !spinbutton1 || !calendar1) return;
    
    const gchar *id_coach = gtk_entry_get_text(GTK_ENTRY(entryCoach));
    const gchar *equipement = gtk_entry_get_text(GTK_ENTRY(combo_entry3));
    gint quantite = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton1));
    
    if (strlen(id_coach) == 0 || strlen(equipement) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK, "Veuillez remplir tous les champs");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    guint year, month, day;
    gtk_calendar_get_date(GTK_CALENDAR(calendar1), &year, &month, &day);
    
    Date date;
    date.jour = day;
    date.mois = month + 1;
    date.annee = year;
    
    if (!verifierDisponibilite(FICHIER_RESERVATIONS, (char*)equipement, date, quantite)) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK, "Cet équipement n'est pas disponible pour cette date");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    
    
    Reservation r;
    strcpy(r.id_coach, id_coach);
    strcpy(r.equipement, equipement);
    r.quantite = quantite;
    r.dateReservation = date;
    r.active = 1;
    
    if (ajouterReservation(FICHIER_RESERVATIONS, r)) {
        Equipement e;
        FILE *f = fopen(FICHIER_EQUIPEMENTS, "r");
        FILE *f2 = fopen("temp.txt", "w");
        
        if (f && f2) {
            while (fscanf(f, "%s %s %s %d %d %d ",
                          e.reference, e.nom, e.type, &e.quantite,
                          &e.etat, &e.localisation) != EOF) {
                if (strcmp(e.reference, equipement) == 0) {
                    e.etat = RESERVE;
                }
                fprintf(f2, "%s %s %s %d %d %d \n",
                        e.reference, e.nom, e.type, e.quantite,
                        e.etat, e.localisation);
            }
            fclose(f);
            fclose(f2);
            remove(FICHIER_EQUIPEMENTS);
            rename("temp.txt", FICHIER_EQUIPEMENTS);
        }
        

        
        if (labelAffich) {
            char message[200];
            sprintf(message, "✓ Réservation effectuée : %s x%d pour %s le %02d/%02d/%d",
                    equipement, quantite, id_coach, date.jour, date.mois, date.annee);
            gtk_label_set_text(GTK_LABEL(labelAffich), message);
        }
        
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK, "Réservation effectuée avec succès");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK, "Erreur lors de la réservation");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_buttonAnnul_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "yh_window");
    GtkWidget *entryCoach = lookup_widget(window, "entryCoach");
    GtkWidget *combo_entry3 = lookup_widget(window, "combo_entry3");
    GtkWidget *labelAffich = lookup_widget(window, "labelAffich");
    
    if (!entryCoach || !combo_entry3) {
        show_error_message("Widgets non trouvés");
        return;
    }
    
    const gchar *id_coach = gtk_entry_get_text(GTK_ENTRY(entryCoach));
    const gchar *equipement = gtk_entry_get_text(GTK_ENTRY(combo_entry3));
    
    if (strlen(id_coach) == 0 || strlen(equipement) == 0) {
        show_warning_message("Veuillez remplir tous les champs");
        return;
    }
    
    if (annulerReservation(FICHIER_RESERVATIONS, (char*)id_coach, (char*)equipement)) {
        if (labelAffich) {
            char message[200];
            sprintf(message, "✓ Réservation annulée : %s pour %s",
                    equipement, id_coach);
            gtk_label_set_text(GTK_LABEL(labelAffich), message);
        }
        
        gtk_entry_set_text(GTK_ENTRY(entryCoach), "");
        show_info_message("Réservation annulée avec succès");

    } else {
        show_error_message("Aucune réservation active trouvée à annuler");
    }
}

void on_buttonCal_clicked(GtkButton *button, gpointer user_data)
{
    calculerStatistiques(FICHIER_EQUIPEMENTS, FICHIER_RESERVATIONS);


    char equipPlusRes[100] = "Aucun";
    char equipPlusMaint[100] = "Aucun";
    char coachPlusActif[100] = "Aucun";


    FILE *fs = fopen("statistiques.txt", "r");
    if (!fs) {
        show_error_message("Impossible d'ouvrir statistiques.txt");
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fs)) {
        // Chercher le séparateur ':'
        char *p = strchr(ligne, ':');
        if (!p) continue;
        p++; // avancer après ':'
        while (*p == ' ' || *p == '\t') p++; // supprimer espaces/tabs


        if (strstr(ligne, "Equipement le plus réservé")) {
            strncpy(equipPlusRes, p, sizeof(equipPlusRes)-1);
            equipPlusRes[sizeof(equipPlusRes)-1] = '\0';
            equipPlusRes[strcspn(equipPlusRes, "\n")] = '\0';
        }
        else if (strstr(ligne, "Equipement en maintenance")) {
            strncpy(equipPlusMaint, p, sizeof(equipPlusMaint)-1);
            equipPlusMaint[sizeof(equipPlusMaint)-1] = '\0';
            equipPlusMaint[strcspn(equipPlusMaint, "\n")] = '\0';
        }
        else if (strstr(ligne, "Coach le plus actif")) {
            strncpy(coachPlusActif, p, sizeof(coachPlusActif)-1);
            coachPlusActif[sizeof(coachPlusActif)-1] = '\0';
            coachPlusActif[strcspn(coachPlusActif, "\n")] = '\0';
        }
        
    }

    fclose(fs);


    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "yh_window");
    GtkWidget *labelEPR = lookup_widget(window, "labelEPR1");
    GtkWidget *labelEPM = lookup_widget(window, "labelEPM1");
    GtkWidget *labelCPA = lookup_widget(window, "labelCPA1");


    if (labelEPR) gtk_label_set_text(GTK_LABEL(labelEPR), equipPlusRes);
    if (labelEPM) gtk_label_set_text(GTK_LABEL(labelEPM), equipPlusMaint);
    if (labelCPA) gtk_label_set_text(GTK_LABEL(labelCPA), coachPlusActif);


    show_info_message("Statistiques calculées et affichées avec succès");
}


void on_buttonAff_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(GTK_WIDGET(button), "yh_window");
    GtkWidget *treeview = lookup_widget(window, "treeview1");

    if (!treeview) return;

    // Définir les colonnes si elles n'existent pas encore
    if (gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)) == NULL) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;


        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Référence", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);


        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);


        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);


        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Quantité", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);


        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("État", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);


        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Localisation", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }

    GtkListStore *store = gtk_list_store_new(6,
                                             G_TYPE_STRING, 
                                             G_TYPE_STRING, 
                                             G_TYPE_STRING, 
                                             G_TYPE_INT,    
                                             G_TYPE_STRING, 
                                             G_TYPE_STRING  
    );

    FILE *f = fopen(FICHIER_EQUIPEMENTS, "r");
    if (!f) {
        show_error_message("Impossible d'ouvrir le fichier des équipements");
        return;
    }

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f)) {
        Equipement e;
        char etat_str[50], loc_str[50];

        if (sscanf(ligne, "%[^,],%[^,],%[^,],%d,%[^,],%s",
                   e.reference, e.nom, e.type, &e.quantite,
                   etat_str, loc_str) == 6) {

            e.etat = string_to_etat(etat_str);
            e.localisation = string_to_localisation(loc_str);

            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               0, e.reference,
                               1, e.nom,
                               2, e.type,
                               3, e.quantite,
                               4, etat_to_string(e.etat),
                               5, localisation_to_string(e.localisation),
                               -1);
        }
    }
    fclose(f);

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);

    
    if (treeview != NULL) {
        g_signal_connect(treeview, "row-activated",
                     G_CALLBACK(on_treeview1_row_activated),
                     NULL);
}

}



