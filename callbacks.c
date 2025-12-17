#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "coach.h"

int coaching_pr = 1;
int coach_selected_id = -1;
char coach_selected_nom[30] = "";
char coach_selected_prenom[30] = "";
int type_cours = 1; // 1=Privé, 2=Groupe, 3=Mixte
char cours_selected_materiel[50] = "";
char cours_selected_salle[50] = "";

// ============================================================================
// FONCTIONS DE BASE
// ============================================================================

void on_radiobutton5_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        coaching_pr = 1;
        printf("Coaching privé: OUI\n");
    }
}

void on_radiobutton6_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        coaching_pr = 0;
        printf("Coaching privé: NON\n");
    }
}

void on_buttonenregistrer_clicked(GtkWidget *button, gpointer user_data)
{
    printf("✅ on_buttonenregistrer_clicked\n");
    
    coach nouveau_coach;
    GtkWidget *window = GTK_WIDGET(gtk_widget_get_toplevel(button));
    
    GtkWidget *entrynom = lookup_widget(window, "entrynom1");
    GtkWidget *entryprenom = lookup_widget(window, "entryprenom1");
    GtkWidget *entryid = lookup_widget(window, "entryid1");
    GtkWidget *spincours = lookup_widget(window, "spinbutton1");
    GtkWidget *combospe = lookup_widget(window, "combospe");
    GtkWidget *sortie = lookup_widget(window, "ros");
    
    if (!entrynom || !entryprenom || !entryid || !combospe || !spincours || !sortie) {
        printf("❌ Widgets non trouvés\n");
        return;
    }
    
    const gchar *nom_text = gtk_entry_get_text(GTK_ENTRY(entrynom));
    const gchar *prenom_text = gtk_entry_get_text(GTK_ENTRY(entryprenom));
    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entryid));
    
    if (strlen(nom_text) == 0 || strlen(prenom_text) == 0 || strlen(id_text) == 0) {
        gtk_label_set_text(GTK_LABEL(sortie), "Tous les champs sont obligatoires!");
        return;
    }
    
    int id = atoi(id_text);
    if (id <= 0) {
        gtk_label_set_text(GTK_LABEL(sortie), "ID invalide!");
        return;
    }
    
    const gchar *spe_text = "";
    if (GTK_IS_COMBO(combospe)) {
        GtkWidget *entry = GTK_COMBO(combospe)->entry;
        spe_text = gtk_entry_get_text(GTK_ENTRY(entry));
    } else if (GTK_IS_COMBO_BOX(combospe)) {
        GtkTreeIter iter;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combospe), &iter)) {
            GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combospe));
            gchar *specialite_temp = NULL;
            gtk_tree_model_get(model, &iter, 0, &specialite_temp, -1);
            if (specialite_temp) {
                spe_text = specialite_temp;
                g_free(specialite_temp);
            }
        }
    }
    
    strcpy(nouveau_coach.nom, nom_text);
    strcpy(nouveau_coach.prenom, prenom_text);
    strcpy(nouveau_coach.specialite, spe_text);
    nouveau_coach.id = id;
    nouveau_coach.nbr_cours = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spincours));
    
    if (coaching_pr == 1)
        strcpy(nouveau_coach.coaching_prive, "oui");
    else
        strcpy(nouveau_coach.coaching_prive, "non");
    
    printf("Nouveau coach: %s %s, ID: %d, Spécialité: %s\n", 
           nouveau_coach.nom, nouveau_coach.prenom, nouveau_coach.id, nouveau_coach.specialite);
    
    if (ajouter_dans_fichier_coach(nouveau_coach)) {
        gtk_label_set_text(GTK_LABEL(sortie), "✅ Coach sauvegardé !");
        
        // Actualiser la liste principale
        GtkWidget *treeview = lookup_widget(window, "treeview3");
        if (treeview) afficher_coachs(treeview);
        
        // Vider les champs
        gtk_entry_set_text(GTK_ENTRY(entrynom), "");
        gtk_entry_set_text(GTK_ENTRY(entryprenom), "");
        gtk_entry_set_text(GTK_ENTRY(entryid), "");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spincours), 0);
        
    } else {
        gtk_label_set_text(GTK_LABEL(sortie), "❌ Erreur de sauvegarde !");
    }
}

void on_buttonmodifier_clicked(GtkWidget *button, gpointer user_data)
{
    printf("✅ on_buttonmodifier_clicked\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    GtkWidget *entry_id = lookup_widget(window, "entryid2");
    GtkWidget *entry_nom = lookup_widget(window, "entrynom2");
    GtkWidget *entry_prenom = lookup_widget(window, "entryprenom2");
    GtkWidget *combo9 = lookup_widget(window, "combo9");
    GtkWidget *spin_cours = lookup_widget(window, "coursmodifier");
    GtkWidget *radio_oui = lookup_widget(window, "oui22");
    GtkWidget *radio_non = lookup_widget(window, "non22");
    GtkWidget *label_status = lookup_widget(window, "modifier");
    
    if (!entry_id || !entry_nom || !entry_prenom || !combo9 || !spin_cours || !radio_oui || !radio_non || !label_status) {
        printf("❌ Widgets manquants\n");
        return;
    }
    
    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
    int nbr_cours = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_cours));
    gboolean coaching_prive = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_oui));
    
    const gchar *specialite = "";
    if (GTK_IS_COMBO(combo9)) {
        GtkWidget *entry = GTK_COMBO(combo9)->entry;
        specialite = gtk_entry_get_text(GTK_ENTRY(entry));
    } else if (GTK_IS_COMBO_BOX(combo9)) {
        GtkTreeIter iter;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo9), &iter)) {
            GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo9));
            gchar *specialite_temp = NULL;
            gtk_tree_model_get(model, &iter, 0, &specialite_temp, -1);
            if (specialite_temp) {
                specialite = specialite_temp;
                g_free(specialite_temp);
            }
        }
    }
    
    if (strlen(id_text) == 0 || strlen(nom) == 0 || strlen(prenom) == 0 || strlen(specialite) == 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Tous les champs sont obligatoires!");
        return;
    }
    
    int id = atoi(id_text);
    if (id <= 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "ID invalide!");
        return;
    }
    
    coach nouv;
    nouv.id = id;
    strcpy(nouv.nom, nom);
    strcpy(nouv.prenom, prenom);
    strcpy(nouv.specialite, specialite);
    nouv.nbr_cours = nbr_cours;
    strcpy(nouv.coaching_prive, coaching_prive ? "oui" : "non");
    
    if (modifier_coach(id, nouv)) {
        gtk_label_set_text(GTK_LABEL(label_status), "✅ Coach modifié avec succès!");
        
        // Actualiser les listes
        GtkWidget *treeview = lookup_widget(window, "treeview3");
        if (treeview) afficher_coachs(treeview);
        
    } else {
        gtk_label_set_text(GTK_LABEL(label_status), "❌ Erreur lors de la modification!");
    }
}

void on_supprimer_clicked(GtkWidget *button, gpointer user_data)
{
    printf("🗑️ on_supprimer_clicked\n");
    
    if (coach_selected_id == -1) {
        printf("❌ Aucun coach sélectionné\n");
        
        GtkWidget *window = gtk_widget_get_toplevel(button);
        GtkWidget *label_status = lookup_widget(window, "ros");
        if (label_status) {
            gtk_label_set_text(GTK_LABEL(label_status), "❌ Aucun coach sélectionné!");
        }
        return;
    }
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    char message[200];
    sprintf(message, "Supprimer le coach %s %s (ID: %d)?", 
            coach_selected_nom, coach_selected_prenom, coach_selected_id);
    
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "%s", message);
    
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    if (response == GTK_RESPONSE_YES) {
        if (supprimer_coach(COACH_FILE, coach_selected_id)) {
            enregistrer_suppression_log(coach_selected_id, 
                                       coach_selected_nom, 
                                       coach_selected_prenom,
                                       "Suppression directe",
                                       "");
            
            GtkWidget *label_status = lookup_widget(window, "ros");
            if (label_status) {
                sprintf(message, "✅ Coach %s %s supprimé!", 
                        coach_selected_nom, coach_selected_prenom);
                gtk_label_set_text(GTK_LABEL(label_status), message);
            }
            
            // Actualiser la liste
            GtkWidget *treeview = lookup_widget(window, "treeview3");
            if (treeview) afficher_coachs(treeview);
            
            // Réinitialiser la sélection
            coach_selected_id = -1;
            strcpy(coach_selected_nom, "");
            strcpy(coach_selected_prenom, "");
            
        } else {
            GtkWidget *label_status = lookup_widget(window, "ros");
            if (label_status) {
                gtk_label_set_text(GTK_LABEL(label_status), "❌ Erreur lors de la suppression!");
            }
        }
    } else {
        GtkWidget *label_status = lookup_widget(window, "ros");
        if (label_status) {
            gtk_label_set_text(GTK_LABEL(label_status), "Suppression annulée");
        }
    }
}

void on_bouton_supprimer_clicked(GtkWidget *button, gpointer user_data)
{
    printf("🗑️ on_bouton_supprimer_clicked\n");
    
    if (coach_selected_id == -1) {
        printf("❌ Aucun coach sélectionné\n");
        
        GtkWidget *window = gtk_widget_get_toplevel(button);
        GtkWidget *label_status = lookup_widget(window, "ressupp");
        if (label_status) {
            gtk_label_set_text(GTK_LABEL(label_status), "❌ Aucun coach sélectionné!");
        }
        return;
    }
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    GtkWidget *check1 = lookup_widget(window, "checkbutton1");
    GtkWidget *check2 = lookup_widget(window, "checkbutton2");
    GtkWidget *check3 = lookup_widget(window, "checkbutton3");
    GtkWidget *check4 = lookup_widget(window, "checkbutton4");
    GtkWidget *text_view = lookup_widget(window, "textview1");
    GtkWidget *label_status = lookup_widget(window, "ressupp");
    
    if (!check1 || !check2 || !check3 || !check4 || !text_view || !label_status) {
        printf("❌ Widgets manquants\n");
        return;
    }
    
    gboolean check1_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check1));
    gboolean check2_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check2));
    gboolean check3_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check3));
    gboolean check4_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check4));
    
    if (!check1_active && !check2_active && !check3_active && !check4_active) {
        gtk_label_set_text(GTK_LABEL(label_status), "Veuillez sélectionner une raison!");
        return;
    }
    
    char raison[256] = "";
    if (check1_active) strcat(raison, "Départ, ");
    if (check2_active) strcat(raison, "Licenciement, ");
    if (check3_active) strcat(raison, "Retraite, ");
    if (check4_active) strcat(raison, "Autre, ");
    
    if (strlen(raison) > 0) {
        raison[strlen(raison) - 2] = '\0';
    }
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *commentaire = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    
    char message[200];
    sprintf(message, "Supprimer le coach %s %s (ID: %d)?", 
            coach_selected_nom, coach_selected_prenom, coach_selected_id);
    
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "%s", message);
    
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    if (response == GTK_RESPONSE_YES) {
        if (supprimer_coach(COACH_FILE, coach_selected_id)) {
            enregistrer_suppression_log(coach_selected_id, 
                                       coach_selected_nom, 
                                       coach_selected_prenom,
                                       raison,
                                       commentaire ? commentaire : "");
            
            sprintf(message, "✅ Coach %s %s supprimé!", coach_selected_nom, coach_selected_prenom);
            gtk_label_set_text(GTK_LABEL(label_status), message);
            
            // Actualiser la liste
            GtkWidget *treeview = lookup_widget(window, "treeview3");
            if (treeview) afficher_coachs(treeview);
            
            // Réinitialiser
            coach_selected_id = -1;
            strcpy(coach_selected_nom, "");
            strcpy(coach_selected_prenom, "");
            
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check1), FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check2), FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check3), FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check4), FALSE);
            gtk_text_buffer_set_text(buffer, "", -1);
            
        } else {
            gtk_label_set_text(GTK_LABEL(label_status), "❌ Erreur de suppression!");
        }
    } else {
        gtk_label_set_text(GTK_LABEL(label_status), "Suppression annulée");
    }
    
    if (commentaire) g_free(commentaire);
}

void on_treeview3_row_activated(GtkTreeView *treeview, 
                               GtkTreePath *path, 
                               GtkTreeViewColumn *column, 
                               gpointer user_data)
{
    printf("👆 on_treeview3_row_activated\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(treeview));
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    
    if (!selection) {
        printf("❌ Pas de sélection\n");
        return;
    }
    
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        int id = 0;
        gchar *nom = NULL;
        gchar *prenom = NULL;
        gchar *specialite = NULL;
        int nbr_cours = 0;
        gchar *coaching_prive = NULL;
        
        gtk_tree_model_get(model, &iter,
                           0, &id,
                           1, &nom,
                           2, &prenom,
                           3, &specialite,
                           4, &nbr_cours,
                           5, &coaching_prive,
                           -1);
        
        printf("Coach sélectionné: %s %s (ID: %d)\n", nom ? nom : "", prenom ? prenom : "", id);
        
        coach_selected_id = id;
        if (nom) strcpy(coach_selected_nom, nom);
        if (prenom) strcpy(coach_selected_prenom, prenom);
        
        GtkWidget *entry_id = lookup_widget(window, "entryid2");
        GtkWidget *entry_nom = lookup_widget(window, "entrynom2");
        GtkWidget *entry_prenom = lookup_widget(window, "entryprenom2");
        GtkWidget *combo9 = lookup_widget(window, "combo9");
        GtkWidget *spin_cours = lookup_widget(window, "coursmodifier");
        GtkWidget *radio_oui = lookup_widget(window, "oui22");
        GtkWidget *radio_non = lookup_widget(window, "non22");
        
        if (entry_id && entry_nom && entry_prenom && combo9 && spin_cours && radio_oui && radio_non) {
            char id_str[20];
            sprintf(id_str, "%d", id);
            gtk_entry_set_text(GTK_ENTRY(entry_id), id_str);
            gtk_entry_set_text(GTK_ENTRY(entry_nom), nom ? nom : "");
            gtk_entry_set_text(GTK_ENTRY(entry_prenom), prenom ? prenom : "");
            
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_cours), nbr_cours);
            
            if (specialite) {
                if (GTK_IS_COMBO(combo9)) {
                    GtkWidget *entry = GTK_COMBO(combo9)->entry;
                    gtk_entry_set_text(GTK_ENTRY(entry), specialite);
                } else if (GTK_IS_COMBO_BOX(combo9)) {
                    GtkTreeModel *model_combo = gtk_combo_box_get_model(GTK_COMBO_BOX(combo9));
                    GtkTreeIter iter_combo;
                    gboolean found = FALSE;
                    
                    if (gtk_tree_model_get_iter_first(model_combo, &iter_combo)) {
                        do {
                            gchar *spe_temp = NULL;
                            gtk_tree_model_get(model_combo, &iter_combo, 0, &spe_temp, -1);
                            
                            if (spe_temp && strcasecmp(spe_temp, specialite) == 0) {
                                gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo9), &iter_combo);
                                found = TRUE;
                            }
                            
                            if (spe_temp) g_free(spe_temp);
                            
                        } while (!found && gtk_tree_model_iter_next(model_combo, &iter_combo));
                    }
                    
                    if (!found && GTK_IS_COMBO_BOX_ENTRY(combo9)) {
                        GtkWidget *entry = gtk_bin_get_child(GTK_BIN(combo9));
                        gtk_entry_set_text(GTK_ENTRY(entry), specialite);
                    }
                }
            }
            
            if (coaching_prive && strcasecmp(coaching_prive, "oui") == 0) {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_oui), TRUE);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_non), FALSE);
            } else {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_oui), FALSE);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_non), TRUE);
            }
            
            printf("✅ Champs remplis pour ID %d\n", id);
        }
        
        GtkWidget *label_nom_supp = lookup_widget(window, "label_nom_supp");
        GtkWidget *label_prenom_supp = lookup_widget(window, "label_prenom_supp");
        GtkWidget *label_id_supp = lookup_widget(window, "label_id_supp");
        GtkWidget *label_spe_supp = lookup_widget(window, "label_spe_supp");
        
        if (label_nom_supp && nom) gtk_label_set_text(GTK_LABEL(label_nom_supp), nom);
        if (label_prenom_supp && prenom) gtk_label_set_text(GTK_LABEL(label_prenom_supp), prenom);
        if (label_id_supp) {
            char id_str[20];
            sprintf(id_str, "%d", id);
            gtk_label_set_text(GTK_LABEL(label_id_supp), id_str);
        }
        if (label_spe_supp && specialite) gtk_label_set_text(GTK_LABEL(label_spe_supp), specialite);
        
        if (nom) g_free(nom);
        if (prenom) g_free(prenom);
        if (specialite) g_free(specialite);
        if (coaching_prive) g_free(coaching_prive);
    }
}

void on_treeview3_cursor_changed(GtkTreeView *treeview, gpointer user_data)
{
    printf("📋 on_treeview3_cursor_changed\n");
    on_treeview3_row_activated(treeview, NULL, NULL, user_data);
}

void on_actualiser_clicked(GtkWidget *button, gpointer user_data)
{
    printf("🔄 on_actualiser_clicked\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    GtkWidget *treeview = lookup_widget(window, "treeview3");
    
    if (!treeview) treeview = lookup_widget(window, "treeview1");
    if (!treeview) treeview = lookup_widget(window, "treeview");
    
    if (treeview) {
        afficher_coachs(treeview);
        printf("✅ TreeView actualisé\n");
    }
}

// ============================================================================
// INITIALISATION
// ============================================================================

void initialiser_combospe(GtkWidget *window) {
    GtkWidget *combospe = lookup_widget(window, "combospe");
    if (!combospe) return;
    
    printf("Initialisation combospe\n");
    
    if (GTK_IS_COMBO(combospe)) {
        const gchar *specialites[] = {
            "Boxe", "Fitness Général", "Yoga", "Pilates", 
            "Préparation Physique", "Musculation", NULL
        };
        
        GList *specialites_list = NULL;
        for (int i = 0; specialites[i] != NULL; i++) {
            specialites_list = g_list_append(specialites_list, (gpointer)specialites[i]);
        }
        
        gtk_combo_set_popdown_strings(GTK_COMBO(combospe), specialites_list);
        g_list_free(specialites_list);
        
    } else if (GTK_IS_COMBO_BOX(combospe)) {
        GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
        GtkTreeIter iter;
        
        const gchar *specialites[] = {
            "Boxe", "Fitness Général", "Yoga", "Pilates", 
            "Préparation Physique", "Musculation", NULL
        };
        
        for (int i = 0; specialites[i] != NULL; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, specialites[i], -1);
        }
        
        gtk_combo_box_set_model(GTK_COMBO_BOX(combospe), GTK_TREE_MODEL(store));
        
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        gtk_cell_layout_clear(GTK_CELL_LAYOUT(combospe));
        gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combospe), renderer, TRUE);
        gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combospe), renderer, "text", 0, NULL);
        
        gtk_combo_box_set_active(GTK_COMBO_BOX(combospe), 0);
        g_object_unref(store);
    }
}

void initialiser_combo9(GtkWidget *window) {
    GtkWidget *combo9 = lookup_widget(window, "combo9");
    if (!combo9) return;
    
    printf("Initialisation combo9\n");
    
    if (GTK_IS_COMBO(combo9)) {
        const gchar *specialites[] = {
            "Boxe", "Fitness Général", "Yoga", "Pilates", 
            "Préparation Physique", "Musculation", NULL
        };
        
        GList *specialites_list = NULL;
        for (int i = 0; specialites[i] != NULL; i++) {
            specialites_list = g_list_append(specialites_list, (gpointer)specialites[i]);
        }
        
        gtk_combo_set_popdown_strings(GTK_COMBO(combo9), specialites_list);
        g_list_free(specialites_list);
        
    } else if (GTK_IS_COMBO_BOX(combo9)) {
        GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
        GtkTreeIter iter;
        
        const gchar *specialites[] = {
            "Boxe", "Fitness Général", "Yoga", "Pilates", 
            "Préparation Physique", "Musculation", NULL
        };
        
        for (int i = 0; specialites[i] != NULL; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, specialites[i], -1);
        }
        
        gtk_combo_box_set_model(GTK_COMBO_BOX(combo9), GTK_TREE_MODEL(store));
        
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        gtk_cell_layout_clear(GTK_CELL_LAYOUT(combo9));
        gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo9), renderer, TRUE);
        gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo9), renderer, "text", 0, NULL);
        
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo9), 0);
        g_object_unref(store);
    }
}

void initialiser_comborechercher(GtkWidget *window) {
    GtkWidget *comborechercher = lookup_widget(window, "comborechercher");
    if (!comborechercher) return;
    
    printf("Initialisation comborechercher\n");
    
    if (GTK_IS_COMBO(comborechercher)) {
        const gchar *options[] = {
            "Toutes",
            "Boxe", "Fitness Général", "Yoga", "Pilates", 
            "Préparation Physique", "Musculation", NULL
        };
        
        GList *options_list = NULL;
        for (int i = 0; options[i] != NULL; i++) {
            options_list = g_list_append(options_list, (gpointer)options[i]);
        }
        
        gtk_combo_set_popdown_strings(GTK_COMBO(comborechercher), options_list);
        g_list_free(options_list);
        
        GtkWidget *entry = GTK_COMBO(comborechercher)->entry;
        gtk_entry_set_text(GTK_ENTRY(entry), "Toutes");
        
    } else if (GTK_IS_COMBO_BOX(comborechercher)) {
        GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
        GtkTreeIter iter;
        
        const gchar *options[] = {
            "Toutes",
            "Boxe", "Fitness Général", "Yoga", "Pilates", 
            "Préparation Physique", "Musculation", NULL
        };
        
        for (int i = 0; options[i] != NULL; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, options[i], -1);
        }
        
        gtk_combo_box_set_model(GTK_COMBO_BOX(comborechercher), GTK_TREE_MODEL(store));
        
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        gtk_cell_layout_clear(GTK_CELL_LAYOUT(comborechercher));
        gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(comborechercher), renderer, TRUE);
        gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(comborechercher), renderer, "text", 0, NULL);
        
        gtk_combo_box_set_active(GTK_COMBO_BOX(comborechercher), 0);
        g_object_unref(store);
    }
    
    printf("✅ comborechercher initialisé\n");
}

// ============================================================================
// RECHERCHE - VERSION CORRIGÉE
// ============================================================================

void on_rechercher_clicked(GtkWidget *button, gpointer user_data)
{
    printf("🔍 on_rechercher_clicked - VERSION CORRIGÉE\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    // Récupérer les widgets avec plusieurs tentatives de noms
    GtkWidget *comborechercher = NULL;
    GtkWidget *radio_oui33 = NULL;
    GtkWidget *radio_non33 = NULL;
    GtkWidget *radio_tous33 = NULL;
    GtkWidget *treeview8 = NULL;
    GtkWidget *label_resultat = NULL;
    
    // Essayer différents noms pour les widgets
    const char *possible_combos[] = {"comborechercher", "combo_recherche", "specialite_combo", NULL};
    const char *possible_trees[] = {"treeview8", "treeview_recherche", "treeview2", NULL};
    
    for (int i = 0; possible_combos[i] != NULL; i++) {
        comborechercher = lookup_widget(window, possible_combos[i]);
        if (comborechercher) {
            printf("✅ Combo trouvé: %s\n", possible_combos[i]);
            break;
        }
    }
    
    for (int i = 0; possible_trees[i] != NULL; i++) {
        treeview8 = lookup_widget(window, possible_trees[i]);
        if (treeview8) {
            printf("✅ TreeView trouvé: %s\n", possible_trees[i]);
            break;
        }
    }
    
    // Chercher les radio buttons avec différents noms
    radio_oui33 = lookup_widget(window, "oui33");
    if (!radio_oui33) radio_oui33 = lookup_widget(window, "radio_oui");
    if (!radio_oui33) radio_oui33 = lookup_widget(window, "oui");
    
    radio_non33 = lookup_widget(window, "non33");
    if (!radio_non33) radio_non33 = lookup_widget(window, "radio_non");
    if (!radio_non33) radio_non33 = lookup_widget(window, "non");
    
    radio_tous33 = lookup_widget(window, "tous33");
    if (!radio_tous33) radio_tous33 = lookup_widget(window, "radio_tous");
    if (!radio_tous33) radio_tous33 = lookup_widget(window, "tous");
    
    label_resultat = lookup_widget(window, "resrechercher");
    if (!label_resultat) label_resultat = lookup_widget(window, "label_resultat");
    
    printf("Statut widgets:\n");
    printf("  Combo: %s\n", comborechercher ? "Trouvé" : "Non trouvé");
    printf("  TreeView: %s\n", treeview8 ? "Trouvé" : "Non trouvé");
    printf("  Radio Oui: %s\n", radio_oui33 ? "Trouvé" : "Non trouvé");
    printf("  Radio Non: %s\n", radio_non33 ? "Trouvé" : "Non trouvé");
    printf("  Radio Tous: %s\n", radio_tous33 ? "Trouvé" : "Non trouvé");
    
    // Si treeview8 n'est pas trouvé, essayer avec treeview3
    if (!treeview8) {
        treeview8 = lookup_widget(window, "treeview3");
        printf("  TreeView3: %s\n", treeview8 ? "Trouvé" : "Non trouvé");
    }
    
    if (!treeview8) {
        printf("❌ Aucun TreeView trouvé pour la recherche!\n");
        return;
    }
    
    // Récupérer la spécialité
    const gchar *specialite = "";
    
    if (comborechercher) {
        if (GTK_IS_COMBO(comborechercher)) {
            GtkWidget *entry = GTK_COMBO(comborechercher)->entry;
            specialite = gtk_entry_get_text(GTK_ENTRY(entry));
        } else if (GTK_IS_COMBO_BOX(comborechercher)) {
            GtkTreeIter iter;
            if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(comborechercher), &iter)) {
                GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(comborechercher));
                gchar *specialite_temp = NULL;
                gtk_tree_model_get(model, &iter, 0, &specialite_temp, -1);
                if (specialite_temp) {
                    specialite = specialite_temp;
                    g_free(specialite_temp);
                }
            }
        }
    }
    
    // Récupérer le coaching privé
    const gchar *coaching_prive = "";
    
    if (radio_tous33 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_tous33))) {
        coaching_prive = "Tous";
    } else if (radio_oui33 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_oui33))) {
        coaching_prive = "oui";
    } else if (radio_non33 && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_non33))) {
        coaching_prive = "non";
    } else {
        coaching_prive = "Tous"; // Par défaut
    }
    
    // Si "Toutes" est sélectionné, mettre chaîne vide
    if (specialite && strcasecmp(specialite, "Toutes") == 0) {
        specialite = "";
    }
    
    printf("Critères de recherche:\n");
    printf("  Spécialité: '%s'\n", specialite);
    printf("  Coaching privé: '%s'\n", coaching_prive);
    
    // Lancer la recherche
    rechercher_coachs(specialite, coaching_prive, treeview8);
    
    // Afficher un message de résultat
    if (label_resultat) {
        char message[200];
        
        // Compter les résultats
        GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview8));
        int nb_resultats = 0;
        if (model) {
            GtkTreeIter iter;
            if (gtk_tree_model_get_iter_first(model, &iter)) {
                do {
                    gchar *nom = NULL;
                    gtk_tree_model_get(model, &iter, 1, &nom, -1);
                    if (nom && strcmp(nom, "Aucun résultat") != 0) {
                        nb_resultats++;
                    }
                    if (nom) g_free(nom);
                } while (gtk_tree_model_iter_next(model, &iter));
            }
        }
        
        if (nb_resultats == 0) {
            sprintf(message, "❌ Aucun résultat trouvé");
        } else {
            sprintf(message, "✅ %d résultat(s) trouvé(s)", nb_resultats);
        }
        
        gtk_label_set_text(GTK_LABEL(label_resultat), message);
    }
    
    printf("✅ Recherche terminée\n");
}

// ============================================================================
// AFFICHAGE FENÊTRES
// ============================================================================

void on_fenetre_principale_show(GtkWidget *widget, gpointer user_data) {
    printf("🚀 Fenêtre principale affichée\n");
    initialiser_combospe(widget);
    
    GtkWidget *treeview = lookup_widget(widget, "treeview3");
    if (!treeview) treeview = lookup_widget(widget, "treeview1");
    if (treeview) {
        printf("Chargement initial du TreeView\n");
        afficher_coachs(treeview);
    }
}

void on_fenetre_modification_show(GtkWidget *widget, gpointer user_data) {
    printf("🔄 Fenêtre modification affichée\n");
    initialiser_combo9(widget);
}

void on_fenetre_suppression_show(GtkWidget *widget, gpointer user_data) {
    printf("🗑️ Fenêtre suppression affichée\n");
    
    coach_selected_id = -1;
    strcpy(coach_selected_nom, "");
    strcpy(coach_selected_prenom, "");
    
    GtkWidget *label_nom = lookup_widget(widget, "label_nom_supp");
    GtkWidget *label_prenom = lookup_widget(widget, "label_prenom_supp");
    GtkWidget *label_id = lookup_widget(widget, "label_id_supp");
    GtkWidget *label_specialite = lookup_widget(widget, "label_spe_supp");
    GtkWidget *label_status = lookup_widget(widget, "ressupp");
    
    if (label_nom) gtk_label_set_text(GTK_LABEL(label_nom), "--");
    if (label_prenom) gtk_label_set_text(GTK_LABEL(label_prenom), "--");
    if (label_id) gtk_label_set_text(GTK_LABEL(label_id), "--");
    if (label_specialite) gtk_label_set_text(GTK_LABEL(label_specialite), "--");
    if (label_status) gtk_label_set_text(GTK_LABEL(label_status), "Sélectionnez un coach dans la liste");
}

void on_fenetre_recherche_show(GtkWidget *widget, gpointer user_data) {
    printf("🔍 Fenêtre recherche affichée\n");
    
    // Initialiser la combo de recherche
    initialiser_comborechercher(widget);
    
    // Activer le bouton "Tous" par défaut
    GtkWidget *radio_tous33 = lookup_widget(widget, "tous33");
    if (!radio_tous33) radio_tous33 = lookup_widget(widget, "radio_tous");
    if (!radio_tous33) radio_tous33 = lookup_widget(widget, "tous");
    
    if (radio_tous33) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_tous33), TRUE);
        printf("✅ Bouton 'Tous' activé par défaut\n");
    }
    
    // Initialiser le treeview de recherche
    GtkWidget *treeview8 = lookup_widget(widget, "treeview8");
    if (!treeview8) treeview8 = lookup_widget(widget, "treeview_recherche");
    
    if (treeview8) {
        // Nettoyer le treeview
        nettoyer_treeview(treeview8);
        
        // Créer un store vide
        GtkListStore *store = gtk_list_store_new(6, 
            G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, 
            G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING);
        
        // Créer les colonnes
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview8), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview8), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Prénom", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview8), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Spécialité", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview8), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Cours", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview8), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Privé", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview8), column);
        
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview8), GTK_TREE_MODEL(store));
        g_object_unref(store);
        
        printf("✅ TreeView recherche initialisé\n");
    }
    
    GtkWidget *label_resultat = lookup_widget(widget, "resrechercher");
    if (label_resultat) {
        gtk_label_set_text(GTK_LABEL(label_resultat), "Sélectionnez des critères et cliquez sur Rechercher");
    }
}

// ============================================================================
// AUTRES
// ============================================================================

void on_bouton_ouvrir_suppression_clicked(GtkWidget *button, gpointer user_data)
{
    printf("🚪 Ouverture fenêtre suppression\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    if (coach_selected_id == -1) {
        printf("❌ Aucun coach sélectionné\n");
        
        GtkWidget *label_status = lookup_widget(window, "ros");
        if (label_status) {
            gtk_label_set_text(GTK_LABEL(label_status), "❌ Sélectionnez d'abord un coach!");
        }
        return;
    }
    
    GtkWidget *fenetre_supp = lookup_widget(window, "fenetre_suppression");
    if (fenetre_supp) {
        gtk_widget_show(fenetre_supp);
        printf("✅ Fenêtre suppression ouverte\n");
    }
}

void on_bouton_ouvrir_recherche_clicked(GtkWidget *button, gpointer user_data)
{
    printf("🔍 Ouverture fenêtre recherche\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    GtkWidget *fenetre_rech = lookup_widget(window, "fenetre_recherche");
    if (fenetre_rech) {
        gtk_widget_show(fenetre_rech);
        printf("✅ Fenêtre recherche ouverte\n");
    }
}

void on_treeview8_cursor_changed(GtkTreeView *treeview, gpointer user_data)
{
    printf("📋 on_treeview8_cursor_changed (recherche)\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(treeview));
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    
    if (!selection) return;
    
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        int id = 0;
        gchar *nom = NULL;
        gchar *prenom = NULL;
        
        gtk_tree_model_get(model, &iter,
                           0, &id,
                           1, &nom,
                           2, &prenom,
                           -1);
        
        // Ne pas sélectionner la ligne "Aucun résultat"
        if (nom && strcmp(nom, "Aucun résultat") != 0) {
            coach_selected_id = id;
            if (nom) strcpy(coach_selected_nom, nom);
            if (prenom) strcpy(coach_selected_prenom, prenom);
            
            printf("Coach sélectionné dans recherche: %s %s (ID: %d)\n", 
                   coach_selected_nom, coach_selected_prenom, coach_selected_id);
        }
        
        if (nom) g_free(nom);
        if (prenom) g_free(prenom);
    }
}

void vider_champs_modification(GtkWidget *window) {
    GtkWidget *entry_id = lookup_widget(window, "entryid2");
    GtkWidget *entry_nom = lookup_widget(window, "entrynom2");
    GtkWidget *entry_prenom = lookup_widget(window, "entryprenom2");
    GtkWidget *combo9 = lookup_widget(window, "combo9");
    GtkWidget *spin_cours = lookup_widget(window, "coursmodifier");
    GtkWidget *radio_oui = lookup_widget(window, "oui22");
    GtkWidget *radio_non = lookup_widget(window, "non22");
    GtkWidget *label_status = lookup_widget(window, "modifier");
    
    if (entry_id) gtk_entry_set_text(GTK_ENTRY(entry_id), "");
    if (entry_nom) gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
    if (entry_prenom) gtk_entry_set_text(GTK_ENTRY(entry_prenom), "");
    if (spin_cours) gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_cours), 0);
    if (radio_oui) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_oui), FALSE);
    if (radio_non) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_non), FALSE);
    if (label_status) gtk_label_set_text(GTK_LABEL(label_status), "");
    
    if (combo9 && GTK_IS_COMBO(combo9)) {
        GtkWidget *entry = GTK_COMBO(combo9)->entry;
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    } else if (combo9 && GTK_IS_COMBO_BOX(combo9)) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo9), -1);
    }
    
    printf("✅ Champs de modification vidés\n");
}
// ============================================================================
// FONCTIONS POUR LES COURS
// ============================================================================

// Fonction quand la combo cours change
void on_combocours_changed(GtkWidget *combo, gpointer user_data) {
    printf("📚 Cours sélectionné changé\n");
    
    if (GTK_IS_COMBO_BOX(combo)) {
        GtkTreeIter iter;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo), &iter)) {
            GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo));
            gchar *nom_cours = NULL;
            gtk_tree_model_get(model, &iter, 0, &nom_cours, -1);
            
            if (nom_cours) {
                printf("Cours sélectionné: %s\n", nom_cours);
                g_free(nom_cours);
            }
        }
    }
}

// Fonctions pour les types de cours
void on_type1_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        type_cours = 1;
        printf("Type de cours: Privé\n");
    }
}

void on_type2_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        type_cours = 2;
        printf("Type de cours: Groupe\n");
    }
}

void on_type3_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        type_cours = 3;
        printf("Type de cours: Mixte\n");
    }
}

// Fonction pour afficher les matériels dans le TreeView
void on_show_materiels_clicked(GtkWidget *button, gpointer user_data) {
    printf("🔧 Bouton Show matériels cliqué\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    // Essayer différents noms pour le treeview
    GtkWidget *treeview00 = lookup_widget(window, "treeview00");
    if (!treeview00) treeview00 = lookup_widget(window, "treeview_materiels");
    if (!treeview00) treeview00 = lookup_widget(window, "treeview1");
    
    if (!treeview00) {
        printf("❌ TreeView pour matériels non trouvé\n");
        return;
    }
    
    afficher_materiels_dans_treeview(treeview00);
    printf("✅ Liste des matériels affichée\n");
}

// Fonction pour afficher les salles dans le TreeView
void on_show_salles_clicked(GtkWidget *button, gpointer user_data) {
    printf("🏫 Bouton Show salles cliqué\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    // Essayer différents noms
    GtkWidget *treeview_salles = lookup_widget(window, "treeview_salles");
    if (!treeview_salles) treeview_salles = lookup_widget(window, "treeview3");
    if (!treeview_salles) treeview_salles = lookup_widget(window, "treeview4");
    
    if (!treeview_salles) {
        printf("❌ TreeView pour salles non trouvé\n");
        return;
    }
    
    afficher_salles_dans_treeview(treeview_salles);
    printf("✅ Liste des salles affichée\n");
}

// Fonction pour ajouter un matériel au cours
void on_ajouter_materiel_clicked(GtkWidget *button, gpointer user_data)
{
    printf("➕ Ajout matériel au cours\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    if (!window) {
        printf("❌ Fenêtre non trouvée\n");
        return;
    }
    
    GtkWidget *combo01 = lookup_widget(window, "combo01");
    GtkWidget *treeview00 = lookup_widget(window, "treeview00");
    
    printf("  - combo01: %p, treeview00: %p\n", combo01, treeview00);
    
    if (!combo01) {
        printf("❌ combo01 non trouvé\n");
        afficher_message_erreur(window, "Combo matériel non trouvé");
        return;
    }
    
    if (!treeview00) {
        printf("❌ treeview00 non trouvé\n");
        afficher_message_erreur(window, "TreeView non trouvé");
        return;
    }
    
    printf("  - Type combo01: %s\n", G_OBJECT_TYPE_NAME(combo01));
    printf("  - Type treeview00: %s\n", G_OBJECT_TYPE_NAME(treeview00));
    
    // Récupérer le matériel sélectionné
    const gchar *nom_materiel = "";
    gchar *nom_temp = NULL;
    
    if (GTK_IS_COMBO(combo01)) {
        printf("  - Détection: GtkCombo\n");
        GtkWidget *entry = GTK_COMBO(combo01)->entry;
        if (entry) {
            nom_materiel = gtk_entry_get_text(GTK_ENTRY(entry));
            printf("  - Texte dans combo: '%s'\n", nom_materiel);
        }
    } else if (GTK_IS_COMBO_BOX(combo01)) {
        printf("  - Détection: GtkComboBox\n");
        GtkTreeIter iter;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo01), &iter)) {
            GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo01));
            gtk_tree_model_get(model, &iter, 0, &nom_temp, -1);
            if (nom_temp) {
                nom_materiel = nom_temp;
            }
        }
    }
    
    if (nom_temp) {
        g_free(nom_temp);
    }
    
    if (strlen(nom_materiel) == 0) {
        printf("❌ Aucun matériel sélectionné\n");
        afficher_message_erreur(window, "Veuillez sélectionner un matériel");
        return;
    }
    
    printf("✅ Matériel à ajouter: %s\n", nom_materiel);
    
    // Vérifier la disponibilité
    if (!verifier_disponibilite(nom_materiel, "materiel")) {
        printf("❌ Matériel non disponible\n");
        afficher_message_erreur(window, "Matériel non disponible");
        return;
    }
    
    // Vérifier si déjà dans la liste
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview00));
    if (model) {
        GtkTreeIter iter;
        gboolean valide = gtk_tree_model_get_iter_first(model, &iter);
        
        while (valide) {
            gchar *nom_existant = NULL;
            gchar *type_existant = NULL;
            
            gtk_tree_model_get(model, &iter, 
                               0, &nom_existant,
                               1, &type_existant,
                               -1);
            
            if (nom_existant && strcmp(nom_existant, nom_materiel) == 0 &&
                type_existant && strcmp(type_existant, "Matériel") == 0) {
                printf("❌ Matériel déjà ajouté: %s\n", nom_materiel);
                afficher_message_erreur(window, "Ce matériel est déjà dans la liste");
                
                if (nom_existant) g_free(nom_existant);
                if (type_existant) g_free(type_existant);
                return;
            }
            
            if (nom_existant) g_free(nom_existant);
            if (type_existant) g_free(type_existant);
            valide = gtk_tree_model_iter_next(model, &iter);
        }
    }
    
    // Ajouter au TreeView
    ajouter_element_cours_treeview(nom_materiel, "Matériel", treeview00);
    
    printf("✅ Matériel ajouté avec succès\n");
}
void on_ajouter_salle_clicked(GtkWidget *button, gpointer user_data) {
    printf("➕ Ajout salle au cours\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    if (!window) {
        printf("❌ Fenêtre non trouvée\n");
        return;
    }
    
    GtkWidget *combo02 = lookup_widget(window, "combo02");
    GtkWidget *treeview00 = lookup_widget(window, "treeview00");
    
    printf("  - combo02: %p, treeview00: %p\n", combo02, treeview00);
    
    if (!combo02) {
        printf("❌ combo02 non trouvé\n");
        afficher_message_erreur(window, "Combo salle non trouvé");
        return;
    }
    
    if (!treeview00) {
        printf("❌ treeview00 non trouvé\n");
        afficher_message_erreur(window, "TreeView non trouvé");
        return;
    }
    
    // Récupérer la salle sélectionnée
    const gchar *nom_salle = "";
    gchar *nom_temp = NULL;
    
    if (GTK_IS_COMBO(combo02)) {
        GtkWidget *entry = GTK_COMBO(combo02)->entry;
        if (entry) {
            nom_salle = gtk_entry_get_text(GTK_ENTRY(entry));
            printf("  - Salle sélectionnée: %s\n", nom_salle);
        }
    } else if (GTK_IS_COMBO_BOX(combo02)) {
        GtkTreeIter iter;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo02), &iter)) {
            GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo02));
            gtk_tree_model_get(model, &iter, 0, &nom_temp, -1);
            if (nom_temp) {
                nom_salle = nom_temp;
            }
        }
    }
    
    if (nom_temp) {
        g_free(nom_temp);
    }
    
    if (strlen(nom_salle) == 0) {
        printf("❌ Aucune salle sélectionnée\n");
        afficher_message_erreur(window, "Veuillez sélectionner une salle");
        return;
    }
    
    // Vérifier si une salle est déjà dans la liste (une seule autorisée)
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview00));
    if (model) {
        GtkTreeIter iter;
        gboolean valide = gtk_tree_model_get_iter_first(model, &iter);
        
        while (valide) {
            gchar *type_existant = NULL;
            gtk_tree_model_get(model, &iter, 1, &type_existant, -1);
            
            if (type_existant && strcmp(type_existant, "Salle") == 0) {
                printf("❌ Une salle est déjà sélectionnée\n");
                afficher_message_erreur(window, "Une seule salle autorisée");
                
                if (type_existant) g_free(type_existant);
                return;
            }
            
            if (type_existant) g_free(type_existant);
            valide = gtk_tree_model_iter_next(model, &iter);
        }
    }
    
    // Vérifier la disponibilité
    if (!verifier_disponibilite(nom_salle, "salle")) {
        printf("❌ Salle non disponible\n");
        afficher_message_erreur(window, "Salle non disponible");
        return;
    }
    
    // Ajouter au TreeView
    ajouter_element_cours_treeview(nom_salle, "Salle", treeview00);
    
    printf("✅ Salle ajoutée avec succès\n");
}
// Fonction pour confirmer le cours
void on_confirmer_cours_clicked(GtkWidget *button, gpointer user_data) {
    printf("✅ Confirmation du cours\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    if (!window) {
        printf("❌ Fenêtre non trouvée\n");
        return;
    }
    
    // Récupérer tous les champs
    GtkWidget *combocours = lookup_widget(window, "combocours");
    GtkWidget *spn66 = lookup_widget(window, "spin66");
    GtkWidget *treeview00 = lookup_widget(window, "treeview00");
    
    printf("  - combocours: %p\n", combocours);
    printf("  - spn66: %p\n", spn66);
    printf("  - treeview00: %p\n", treeview00);
    
    if (!combocours || !spn66 || !treeview00) {
        printf("❌ Widgets essentiels non trouvés\n");
        afficher_message_erreur(window, "Données incomplètes");
        return;
    }
    
    // 1. Récupérer le nom du cours
    const gchar *nom_cours = "";
    gchar *nom_cours_temp = NULL;
    
    if (GTK_IS_COMBO(combocours)) {
        printf("  - combocours est un GtkCombo\n");
        GtkWidget *entry = GTK_COMBO(combocours)->entry;
        if (entry) {
            nom_cours = gtk_entry_get_text(GTK_ENTRY(entry));
            printf("  - Cours sélectionné: %s\n", nom_cours);
        }
    } else if (GTK_IS_COMBO_BOX(combocours)) {
        printf("  - combocours est un GtkComboBox\n");
        GtkTreeIter iter;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combocours), &iter)) {
            GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combocours));
            gtk_tree_model_get(model, &iter, 0, &nom_cours_temp, -1);
            if (nom_cours_temp) {
                nom_cours = nom_cours_temp;
                printf("  - Cours sélectionné: %s\n", nom_cours);
            }
        }
    }
    
    if (strlen(nom_cours) == 0) {
        printf("❌ Aucun cours sélectionné\n");
        afficher_message_erreur(window, "Veuillez sélectionner un cours");
        if (nom_cours_temp) g_free(nom_cours_temp);
        return;
    }
    
    // 2. Récupérer le nombre de participants
    int participants = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spn66));
    printf("  - Nombre de participants: %d\n", participants);
    
    if (participants <= 0) {
        printf("❌ Nombre de participants invalide\n");
        afficher_message_erreur(window, "Nombre de participants invalide");
        if (nom_cours_temp) g_free(nom_cours_temp);
        return;
    }
    
    // 3. Récupérer le type de cours
    const char *type_str = "";
    switch (type_cours) {
        case 1: type_str = "Privé"; break;
        case 2: type_str = "Groupe"; break;
        case 3: type_str = "Mixte"; break;
        default: type_str = "Inconnu";
    }
    printf("  - Type de cours: %s\n", type_str);
    
    // 4. Récupérer la liste des matériels et la salle depuis treeview00
    char materiels[500] = "";  // Augmenté à 500 caractères
    char salle[100] = "";
    int has_materiels = 0;
    int has_salle = 0;
    int count_materiels = 0;
    int count_salles = 0;
    
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview00));
    if (model) {
        printf("  - Récupération des éléments du TreeView...\n");
        GtkTreeIter iter;
        gboolean valide = gtk_tree_model_get_iter_first(model, &iter);
        
        if (!valide) {
            printf("    ❌ TreeView vide!\n");
        }
        
        while (valide) {
            gchar *nom = NULL;
            gchar *type = NULL;
            
            gtk_tree_model_get(model, &iter,
                               0, &nom,
                               1, &type,
                               -1);
            
            if (nom && type) {
                printf("    - Élément: %s (%s)\n", nom, type);
                
                if (strcmp(type, "Matériel") == 0) {
                    if (count_materiels > 0) {
                        strcat(materiels, ";");
                    }
                    strcat(materiels, nom);
                    has_materiels = 1;
                    count_materiels++;
                } else if (strcmp(type, "Salle") == 0) {
                    strcpy(salle, nom);
                    has_salle = 1;
                    count_salles++;
                }
            }
            
            if (nom) g_free(nom);
            if (type) g_free(type);
            valide = gtk_tree_model_iter_next(model, &iter);
        }
        
        printf("  - Total matériels: %d\n", count_materiels);
        printf("  - Total salles: %d\n", count_salles);
    } else {
        printf("❌ Modèle TreeView non trouvé\n");
    }
    
    printf("  - Matériels récupérés: %s\n", materiels);
    printf("  - Salle récupérée: %s\n", salle);
    
    // 5. Vérifier qu'on a au moins un matériel et une salle
    if (!has_materiels) {
        printf("❌ Aucun matériel sélectionné\n");
        afficher_message_erreur(window, "Ajoutez au moins un matériel");
        if (nom_cours_temp) g_free(nom_cours_temp);
        return;
    }
    
    if (!has_salle) {
        printf("❌ Aucune salle sélectionnée\n");
        afficher_message_erreur(window, "Ajoutez une salle");
        if (nom_cours_temp) g_free(nom_cours_temp);
        return;
    }
    
    // 6. Vérifier qu'on n'a qu'une seule salle
    if (count_salles > 1) {
        printf("❌ Plus d'une salle sélectionnée\n");
        afficher_message_erreur(window, "Une seule salle autorisée");
        if (nom_cours_temp) g_free(nom_cours_temp);
        return;
    }
    
    // 7. Créer la structure cours
    cours nouveau_cours;
    
    // Copier les données
    strncpy(nouveau_cours.nom, nom_cours, sizeof(nouveau_cours.nom) - 1);
    nouveau_cours.nom[sizeof(nouveau_cours.nom) - 1] = '\0';
    
    strncpy(nouveau_cours.type, type_str, sizeof(nouveau_cours.type) - 1);
    nouveau_cours.type[sizeof(nouveau_cours.type) - 1] = '\0';
    
    nouveau_cours.participants = participants;
    
    strncpy(nouveau_cours.materiels, materiels, sizeof(nouveau_cours.materiels) - 1);
    nouveau_cours.materiels[sizeof(nouveau_cours.materiels) - 1] = '\0';
    
    strncpy(nouveau_cours.salle, salle, sizeof(nouveau_cours.salle) - 1);
    nouveau_cours.salle[sizeof(nouveau_cours.salle) - 1] = '\0';
    
    printf("📋 Résumé du cours:\n");
    printf("   - Nom: %s\n", nouveau_cours.nom);
    printf("   - Type: %s\n", nouveau_cours.type);
    printf("   - Participants: %d\n", nouveau_cours.participants);
    printf("   - Matériels: %s\n", nouveau_cours.materiels);
    printf("   - Salle: %s\n", nouveau_cours.salle);
    
    // 8. Sauvegarder le cours
    if (sauvegarder_cours_fichier(nouveau_cours)) {
        printf("✅ Cours sauvegardé avec succès\n");
        
        // Afficher message de succès
        char message[1000];
        snprintf(message, sizeof(message),
                 "✅ Cours '%s' confirmé avec succès!\n\n"
                 "Type: %s\n"
                 "Participants: %d\n"
                 "Salle: %s\n"
                 "Matériels: %s\n\n"
                 "Le cours a été sauvegardé dans la base de données.",
                 nouveau_cours.nom, 
                 nouveau_cours.type, 
                 nouveau_cours.participants,
                 nouveau_cours.salle,
                 nouveau_cours.materiels);
        
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "%s", message);
        
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        // 9. Vider le TreeView des cours
        if (model) {
            GtkListStore *store = GTK_LIST_STORE(model);
            gtk_list_store_clear(store);
            printf("✅ TreeView vidé\n");
        }
        
    } else {
        printf("❌ Erreur lors de la sauvegarde du cours\n");
        afficher_message_erreur(window, "Erreur lors de la sauvegarde");
    }
    
    // 10. Libérer la mémoire
    if (nom_cours_temp) g_free(nom_cours_temp);
    
    printf("✅ Processus de confirmation terminé\n");
}

void on_verifier_salle_clicked(GtkWidget *button, gpointer user_data)
{
    printf("🔍 on_verifier_salle_clicked\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    // Recherche des widgets
    GtkWidget *combo02 = NULL;
    const char *possible_names[] = {"combo02", "combobox_salle", "salle_combo", NULL};
    
    for (int i = 0; possible_names[i] != NULL; i++) {
        combo02 = lookup_widget(window, possible_names[i]);
        if (combo02) {
            printf("✅ Combo salle trouvé: %s\n", possible_names[i]);
            break;
        }
    }
    
    GtkWidget *label_resultat = lookup_widget(window, "ressalle");
    if (!label_resultat) label_resultat = lookup_widget(window, "label_resultat");
    
    if (!combo02) {
        printf("❌ Combo salle non trouvé\n");
        if (label_resultat) {
            gtk_label_set_text(GTK_LABEL(label_resultat), "❌ Combo salle non trouvé");
        }
        return;
    }
    
    // Récupérer la salle sélectionnée
    const gchar *nom_salle = "";
    
    if (GTK_IS_COMBO_BOX(combo02)) {
        GtkTreeIter iter;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo02), &iter)) {
            GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo02));
            gchar *nom_temp = NULL;
            gtk_tree_model_get(model, &iter, 0, &nom_temp, -1);
            if (nom_temp) {
                nom_salle = nom_temp;
                g_free(nom_temp);
            }
        }
    }
    
    printf("Salle à vérifier: %s\n", nom_salle);
    
    if (label_resultat) {
        if (strlen(nom_salle) == 0) {
            gtk_label_set_text(GTK_LABEL(label_resultat), "❌ Aucune salle sélectionnée");
        } else {
            char message[100];
            sprintf(message, "✅ Salle sélectionnée: %s", nom_salle);
            gtk_label_set_text(GTK_LABEL(label_resultat), message);
        }
    }
}

void on_verifier_materiel_clicked(GtkWidget *button, gpointer user_data)
{
    printf("🔍 on_verifier_materiel_clicked (version GtkCombo)\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    if (!window) {
        printf("❌ Fenêtre non trouvée\n");
        return;
    }
    
    GtkWidget *combo01 = lookup_widget(window, "combo01");
    GtkWidget *label_resultat = lookup_widget(window, "resmat");
    
    if (!combo01 || !label_resultat) {
        printf("❌ Widgets non trouvés\n");
        return;
    }
    
    printf("  - Type de combo01: %s\n", G_OBJECT_TYPE_NAME(combo01));
    
    const gchar *nom_materiel = "";
    
    // POUR GtkCombo (ancien style)
    if (GTK_IS_COMBO(combo01)) {
        printf("  - Détection: GtkCombo (ancien)\n");
        
        GtkWidget *entry = GTK_COMBO(combo01)->entry;
        if (entry) {
            nom_materiel = gtk_entry_get_text(GTK_ENTRY(entry));
            printf("  - Texte dans l'entry: '%s'\n", nom_materiel);
        } else {
            printf("❌ Entry non trouvée dans GtkCombo\n");
        }
    }
    // POUR GtkComboBox (nouveau style)
    else if (GTK_IS_COMBO_BOX(combo01)) {
        printf("  - Détection: GtkComboBox (nouveau)\n");
        
        GtkTreeIter iter;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo01), &iter)) {
            GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo01));
            gchar *nom_temp = NULL;
            gtk_tree_model_get(model, &iter, 0, &nom_temp, -1);
            if (nom_temp) {
                nom_materiel = nom_temp;
                g_free(nom_temp);
            }
        }
    }
    else {
        printf("❌ Type de widget inconnu: %s\n", G_OBJECT_TYPE_NAME(combo01));
    }
    
    if (strlen(nom_materiel) == 0 || strcmp(nom_materiel, "") == 0) {
        printf("❌ Aucun matériel sélectionné (texte vide)\n");
        gtk_label_set_text(GTK_LABEL(label_resultat), "❌ Sélectionnez un matériel");
        return;
    }
    
    printf("✅ Matériel sélectionné: %s\n", nom_materiel);
    
    // Vérifier la disponibilité
    if (verifier_disponibilite(nom_materiel, "materiel")) {
        char message[100];
        sprintf(message, "✅ %s est disponible", nom_materiel);
        gtk_label_set_text(GTK_LABEL(label_resultat), message);
    } else {
        char message[100];
        sprintf(message, "❌ %s n'est pas disponible", nom_materiel);
        gtk_label_set_text(GTK_LABEL(label_resultat), message);
    }
}
void initialiser_treeview00(GtkWidget *window) {
    printf("🔄 Initialisation de treeview00\n");
    
    GtkWidget *treeview00 = lookup_widget(window, "treeview00");
    if (!treeview00) {
        printf("❌ treeview00 non trouvé\n");
        return;
    }
    
    printf("  - TreeView trouvé: %p, Type: %s\n", treeview00, G_OBJECT_TYPE_NAME(treeview00));
    
    // Créer un store vide
    GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview00), GTK_TREE_MODEL(store));
    
    // Créer les colonnes
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    // Supprimer les colonnes existantes
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview00));
    if (columns) {
        g_list_free(columns);
    }
    
    // Colonne Élément
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Élément", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_min_width(column, 150);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview00), column);
    
    // Colonne Type
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_min_width(column, 100);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview00), column);
    
    g_object_unref(store);
    
    printf("✅ treeview00 initialisé\n");
}
void on_show_clicked(GtkWidget *button, gpointer user_data)
{
    printf("👁️ on_show_clicked\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    if (!window) {
        printf("❌ Fenêtre non trouvée\n");
        return;
    }
    
    // Initialiser les combos
    printf("  - Initialisation combo01...\n");
    initialiser_combo01(window);
    
    printf("  - Initialisation combo02...\n");
    initialiser_combo02(window);
    
    printf("  - Initialisation combocours...\n");
    initialiser_combocours(window);
    
    // Initialiser le TreeView
    printf("  - Initialisation treeview00...\n");
    initialiser_treeview00(window);
    
    // Afficher un message
    GtkWidget *label_status = lookup_widget(window, "ros");
    if (label_status) {
        gtk_label_set_text(GTK_LABEL(label_status), "✅ Combos et TreeView initialisés");
    }
    
    // Afficher tous les coachs
    GtkWidget *treeview = lookup_widget(window, "treeview3");
    if (!treeview) treeview = lookup_widget(window, "treeview1");
    if (!treeview) treeview = lookup_widget(window, "treeview");
    
    if (treeview) {
        afficher_coachs(treeview);
        printf("✅ Liste des coachs affichée\n");
    }
}
void initialiser_combocours(GtkWidget *window) {
    GtkWidget *combocours = lookup_widget(window, "combocours");
    if (!combocours) {
        printf("❌ combocours non trouvé\n");
        return;
    }
    
    printf("Initialisation combocours\n");
    
    if (GTK_IS_COMBO_BOX(combocours)) {
        GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
        GtkTreeIter iter;
        
        // Liste des cours
        const gchar *cours[] = {
            "Boxe",
            "Fitness Général", 
            "Yoga",
            "Pilates",
            "Préparation Physique",
            "Musculation",
            NULL
        };
        
        for (int i = 0; cours[i] != NULL; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, cours[i], -1);
        }
        
        gtk_combo_box_set_model(GTK_COMBO_BOX(combocours), GTK_TREE_MODEL(store));
        
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        gtk_cell_layout_clear(GTK_CELL_LAYOUT(combocours));
        gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combocours), renderer, TRUE);
        gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combocours), renderer, "text", 0, NULL);
        
        gtk_combo_box_set_active(GTK_COMBO_BOX(combocours), 0);
        g_object_unref(store);
        
        printf("✅ combocours initialisé\n");
    }
}
// ============================================================================
// FENÊTRE COURS - INITIALISATION
// ============================================================================

void on_fenetre_cours_show(GtkWidget *widget, gpointer user_data) {
    printf("📚📚📚 FENÊTRE COURS AFFICHÉE - INITIALISATION DES COMBOS 📚📚📚\n");
    
    // Vérifier si widget est valide
    if (!widget) {
        printf("❌ Widget NULL!\n");
        return;
    }
    
    printf("  - Nom du widget: %s\n", gtk_widget_get_name(widget));
    printf("  - Type: %s\n", G_OBJECT_TYPE_NAME(widget));
    
    // Initialiser tous les combos
    printf("  - Initialisation combo01...\n");
    initialiser_combo01(widget);
    
    printf("  - Initialisation combo02...\n");
    initialiser_combo02(widget);
    
    printf("  - Initialisation combocours...\n");
    initialiser_combocours(widget);
    
    // Initialiser le treeview pour les éléments du cours (treeview00)
    GtkWidget *treeview00 = lookup_widget(widget, "treeview00");
    if (treeview00) {
        printf("✅ TreeView00 trouvé\n");
        
        // Nettoyer le treeview
        GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview00), GTK_TREE_MODEL(store));
        
        // Ajouter les colonnes
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Élément", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview00), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview00), column);
        
        g_object_unref(store);
        printf("✅ TreeView00 initialisé pour les éléments du cours\n");
    } else {
        printf("❌ TreeView00 non trouvé! Noms possibles:\n");
        
        // Essayer différents noms
        const char *possible_names[] = {
            "treeview00", "treeview_cours", "treeview1", "treeview2", 
            "treeview3", "treeview4", NULL
        };
        
        for (int i = 0; possible_names[i] != NULL; i++) {
            GtkWidget *tv = lookup_widget(widget, possible_names[i]);
            if (tv) {
                printf("  - Trouvé: %s\n", possible_names[i]);
                break;
            }
        }
    }
    
    printf("📚 FIN INITIALISATION FENÊTRE COURS 📚\n");
}

void marquer_disponible(const char *nom, const char *type) {
    printf("✅ Marquage %s comme disponible\n", nom);
    
    char *fichier_source = NULL;
    char *fichier_temp = "temp.txt";
    
    if (strcmp(type, "materiel") == 0) {
        fichier_source = "materiel.txt";
    } else if (strcmp(type, "salle") == 0) {
        fichier_source = "salles.txt";
    } else {
        return;
    }
    
    FILE *source = fopen(fichier_source, "r");
    if (!source) {
        printf("❌ Fichier source non trouvé: %s\n", fichier_source);
        return;
    }
    
    FILE *temp = fopen(fichier_temp, "w");
    if (!temp) {
        printf("❌ Impossible de créer fichier temporaire\n");
        fclose(source);
        return;
    }
    
    char ligne[200];
    int modifie = 0;
    
    while (fgets(ligne, sizeof(ligne), source)) {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) == 0) {
            fprintf(temp, "\n");
            continue;
        }
        
        char nom_fichier[100];
        char quantite[20];
        char statut[20];
        
        if (sscanf(ligne, "%[^;];%[^;];%s", nom_fichier, quantite, statut) == 3) {
            if (strcasecmp(nom_fichier, nom) == 0) {
                // Changer le statut
                fprintf(temp, "%s;%s;disponible\n", nom_fichier, quantite);
                modifie = 1;
                printf("  - %s marqué comme disponible\n", nom);
            } else {
                fprintf(temp, "%s\n", ligne);
            }
        } else {
            fprintf(temp, "%s\n", ligne);
        }
    }
    
    fclose(source);
    fclose(temp);
    
    if (modifie) {
        // Remplacer l'ancien fichier par le nouveau
        remove(fichier_source);
        rename(fichier_temp, fichier_source);
        printf("✅ Fichier %s mis à jour\n", fichier_source);
    } else {
        remove(fichier_temp);
        printf("❌ %s non trouvé dans %s\n", nom, fichier_source);
    }
}
// Libérer tous les éléments d'un cours
void liberer_elements_cours(GtkWidget *treeview) {
    printf("🔄 Libération des éléments du cours\n");
    
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    if (!model) {
        printf("❌ Modèle non trouvé\n");
        return;
    }
    
    GtkTreeIter iter;
    gboolean valide = gtk_tree_model_get_iter_first(model, &iter);
    
    while (valide) {
        gchar *nom = NULL;
        gchar *type = NULL;
        
        gtk_tree_model_get(model, &iter,
                           0, &nom,
                           1, &type,
                           -1);
        
        if (nom && type) {
            printf("  - Libération: %s (%s)\n", nom, type);
            marquer_disponible(nom, type);
        }
        
        if (nom) g_free(nom);
        if (type) g_free(type);
        valide = gtk_tree_model_iter_next(model, &iter);
    }
    
    printf("✅ Tous les éléments libérés\n");
}
void on_terminer_cours_clicked(GtkWidget *button, gpointer user_data) {
    printf("🏁 Terminaison du cours - Libération des ressources\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    GtkWidget *treeview00 = lookup_widget(window, "treeview00");
    
    if (treeview00) {
        liberer_elements_cours(treeview00);
        
        // Afficher message
        GtkWidget *label_status = lookup_widget(window, "ros");
        if (label_status) {
            gtk_label_set_text(GTK_LABEL(label_status), "✅ Ressources libérées");
        }
    }
}
void test_combo01(GtkWidget *window) {
    printf("🧪 TEST combo01\n");
    
    GtkWidget *combo01 = lookup_widget(window, "combo01");
    if (!combo01) {
        printf("❌ combo01 non trouvé\n");
        return;
    }
    
    printf("  - Adresse: %p\n", combo01);
    printf("  - Type: %s\n", G_OBJECT_TYPE_NAME(combo01));
    printf("  - GTK_IS_COMBO: %d\n", GTK_IS_COMBO(combo01));
    printf("  - GTK_IS_WIDGET: %d\n", GTK_IS_WIDGET(combo01));
    
    if (GTK_IS_COMBO(combo01)) {
        printf("  - C'est un GtkCombo!\n");
        GtkWidget *entry = GTK_COMBO(combo01)->entry;
        if (entry) {
            printf("  - Entry trouvée: %p\n", entry);
            const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
            printf("  - Texte actuel: '%s'\n", text);
        } else {
            printf("❌ Entry non trouvée\n");
        }
    }
}
