#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

/* --- BUTTON CALLBACKS --- */
void on_button_entrer_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_cr__er_compte_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_actualiser_liste_membre_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_exporter_liste_membre_clicked(GtkWidget *objet_graphique, gpointer user_data);

void on_button_chercher_pour_modif_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_save_ajouts_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_delete_ajouts_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_calculer_statis_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_modif_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_chercher_membre_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_supprimer_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_annuler_demande_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_envoi_demande_clicked(GtkWidget *objet_graphique, gpointer user_data);
void on_button_imc_clicked(GtkWidget *objet_graphique, gpointer user_data);

/* --- CALENDAR CALLBACKS --- */
void on_calendar_date_naissance_modif_day_selected(GtkWidget *objet_graphique, gpointer user_data);
void on_calendar_inscrip_modif_day_selected(GtkWidget *objet_graphique, gpointer user_data);
void on_calendar_date_de_naiss_membre_day_selected(GtkWidget *objet_graphique, gpointer user_data);
void on_calendar_date_inscrip_day_selected(GtkWidget *objet_graphique, gpointer user_data);
void on_calendar_date_sport_day_selected(GtkWidget *objet_graphique, gpointer user_data);

/* --- CHECKBUTTON / RADIOBUTTON CALLBACKS --- */
void on_checkbutton1_toggled(GtkWidget *objet_graphique, gpointer user_data);
void on_checkbutton2_toggled(GtkWidget *objet_graphique, gpointer user_data);
void on_radiobutton2_toggled(GtkWidget *objet_graphique, gpointer user_data);
void on_radiobutton3_toggled(GtkWidget *objet_graphique, gpointer user_data);

#endif /* CALLBACKS_H */


void
on_treeview2_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void on_buttonfiltrer_clicked (GtkWidget *objet_graphique, gpointer user_data);

void
on_button_add_account_clicked (GtkWidget *objet_graphique, gpointer user_data);

void
on_button_reset_clicked (GtkWidget *objet_graphique, gpointer user_data);

void
on_button_chercher_abon_clicked (GtkWidget *objet_graphique, gpointer user_data);
