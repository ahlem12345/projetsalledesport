#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <gtk/gtk.h>


/* Fonctions pour les boutons */
void on_button_ajouter_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_modifier_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_supprimer_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_rechercher_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_inscrire_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_desinscrire_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_calculer_clicked(GtkWidget *objet_graphique, gpointer user_data);

/* Fonctions pour l'affichage et le rafraîchissement */
int desinscrire_membre(char *id);
void on_window_show(GtkWidget *widget, gpointer user_data);
void refresh_treeview(GtkWidget *treeview);
void load_cours_into_treeview(GtkWidget *treeview);
void remplir_combobox_cours(GtkWidget *combobox);
/*ANCIEN CODE*/
/*
//ajouter par moi

void on_window_show(GtkWidget *widget, gpointer user_data);
void refresh_treeview(GtkWidget *treeview);
void load_cours_into_treeview(GtkWidget *treeview);

// --- CALLBACKS DES BOUTONS ---
void on_button_ajouter_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_modifier_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_supprimer_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_rechercher_clicked(GtkWidget *objet_graphique, gpointer user_data);
//void on_button_inscrire_clicked(GtkWidget *objet_graphique, gpointer user_data);
//void on_button_stats_clicked(GtkWidget *objet_graphique, gpointer user_data);

*/

#endif


