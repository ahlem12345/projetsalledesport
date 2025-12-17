#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

extern int coaching_pr;
extern int coach_selected_id;
extern char coach_selected_nom[30];
extern char coach_selected_prenom[30];
extern int type_cours;
extern char cours_selected_materiel[50];
extern char cours_selected_salle[50];
void on_radiobutton5_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton6_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_buttonenregistrer_clicked(GtkWidget *button, gpointer user_data);
void on_buttonmodifier_clicked(GtkWidget *button, gpointer user_data);
void on_supprimer_clicked(GtkWidget *button, gpointer user_data);
void on_bouton_supprimer_clicked(GtkWidget *button, gpointer user_data);
void on_treeview3_row_activated(GtkTreeView *treeview, 
                               GtkTreePath *path, 
                               GtkTreeViewColumn *column, 
                               gpointer user_data);
void on_treeview3_cursor_changed(GtkTreeView *treeview, gpointer user_data);
void on_actualiser_clicked(GtkWidget *button, gpointer user_data);
void initialiser_combospe(GtkWidget *window);
void initialiser_combo9(GtkWidget *window);
void initialiser_comborechercher(GtkWidget *window);
void on_rechercher_clicked(GtkWidget *button, gpointer user_data);
void on_fenetre_principale_show(GtkWidget *widget, gpointer user_data);
void on_fenetre_modification_show(GtkWidget *widget, gpointer user_data);
void on_fenetre_suppression_show(GtkWidget *widget, gpointer user_data);
void on_fenetre_recherche_show(GtkWidget *widget, gpointer user_data);
void on_bouton_ouvrir_suppression_clicked(GtkWidget *button, gpointer user_data);
void on_bouton_ouvrir_recherche_clicked(GtkWidget *button, gpointer user_data);
void on_treeview8_cursor_changed(GtkTreeView *treeview, gpointer user_data);
void vider_champs_modification(GtkWidget *window);
void on_fenetre_cours_show(GtkWidget *widget, gpointer user_data);
void on_combocours_changed(GtkWidget *combo, gpointer user_data);
void on_type1_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_type2_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_type3_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_show_materiels_clicked(GtkWidget *button, gpointer user_data);
void on_show_salles_clicked(GtkWidget *button, gpointer user_data);
void on_ajouter_materiel_clicked(GtkWidget *button, gpointer user_data);
void on_ajouter_salle_clicked(GtkWidget *button, gpointer user_data);
void on_confirmer_cours_clicked(GtkWidget *button, gpointer user_data);
void on_verifier_salle_clicked(GtkWidget *button, gpointer user_data);
void on_verifier_materiel_clicked(GtkWidget *button, gpointer user_data);
void on_show_clicked(GtkWidget *button, gpointer user_data);
void on_buttonverifier_clicked(GtkButton *button, gpointer user_data);
void on_verifier_salle_clicked(GtkWidget *button, gpointer user_data);
void on_verifier_materiel_clicked(GtkWidget *button, gpointer user_data);
void on_show_clicked(GtkWidget *button, gpointer user_data);
void on_buttonverifier_clicked(GtkButton *button, gpointer user_data);
void on_terminer_cours_clicked(GtkWidget *button, gpointer user_data);
void liberer_elements_cours(GtkWidget *treeview);
void marquer_disponible(const char *nom, const char *type);
void ajouter_element_cours_treeview(const char *nom, const char *type, GtkWidget *treeview);
int verifier_disponibilite(const char *nom, const char *type);
void creer_fichier_par_defaut(const char *fichier, const char *type);
void ajouter_element_base(const char *nom, const char *type, const char *statut, const char *quantite);
int verifier_disponibilite(const char *nom, const char *type);
void marquer_indisponible(const char *nom, const char *type);
void initialiser_combo01(GtkWidget *window);
void initialiser_combo02(GtkWidget *window);
void initialiser_combocours(GtkWidget *window);
void test_combo01(GtkWidget *window);
void initialiser_treeview00(GtkWidget *window);
#endif

void
on_stat_clicked                        (GtkButton       *button,
                                        gpointer         user_data);
