#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "coach.h"

int coaching_pr = 1;

void on_buttonenregistrer_clicked(GtkWidget *button, gpointer user_data)
{
    coach nouveau_coach;
    GtkWidget *entrynom, *entryprenom, *entryid, *combospe, *sortie, *spincours;
    
    // Récupérer la fenêtre parente
    GtkWidget *window = GTK_WIDGET(gtk_widget_get_toplevel(button));
    
    // Chercher les widgets
    entrynom = lookup_widget(window, "entrynom1");
    entryprenom = lookup_widget(window, "entryprenom1");
    entryid = lookup_widget(window, "entryid1");
    spincours = lookup_widget(window, "spinbutton1");
    combospe = lookup_widget(window, "combospe");
    sortie = lookup_widget(window, "ros");
    
    // Vérifier les widgets
    if (!entrynom || !entryprenom || !entryid || !combospe || !spincours || !sortie) {
        printf("❌ Erreur: Widgets non trouvés\n");
        return;
    }
    
    // Récupérer le texte des champs d'entrée
    const gchar *nom_text = gtk_entry_get_text(GTK_ENTRY(entrynom));
    const gchar *prenom_text = gtk_entry_get_text(GTK_ENTRY(entryprenom));
    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entryid));
    
    // Récupérer la spécialité sélectionnée
    const gchar *spe_text = "";
    
    // GtkCombo est différent de GtkComboBox
    if (GTK_IS_COMBO(combospe)) {
        // Pour GtkCombo (ancienne version GTK2)
        GtkWidget *entry = GTK_COMBO(combospe)->entry;
        spe_text = gtk_entry_get_text(GTK_ENTRY(entry));
        printf("Spécialité (GtkCombo): %s\n", spe_text);
    } else if (GTK_IS_COMBO_BOX(combospe)) {
        // Pour GtkComboBox (nouvelle version)
        GtkTreeIter iter;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combospe), &iter)) {
            GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combospe));
            gchar *specialite_temp = NULL;
            gtk_tree_model_get(model, &iter, 0, &specialite_temp, -1);
            if (specialite_temp) {
                spe_text = specialite_temp;
                printf("Spécialité (GtkComboBox): %s\n", spe_text);
                g_free(specialite_temp);
            }
        }
    }
    
    // Copier les données
    strcpy(nouveau_coach.nom, nom_text);
    strcpy(nouveau_coach.prenom, prenom_text);
    strcpy(nouveau_coach.specialite, spe_text);
    
    // Convertir ID
    nouveau_coach.id = atoi(id_text);
    
    // Récupérer nombre de cours
    nouveau_coach.nbr_cours = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spincours));
    
    // Coaching privé
    if (coaching_pr == 1)
        strcpy(nouveau_coach.coaching_prive, "oui");
    else
        strcpy(nouveau_coach.coaching_prive, "non");
    
    // Afficher toutes les données
    printf("Nouveau coach - Nom: %s, Prénom: %s, ID: %d, Spécialité: %s, Cours: %d, Privé: %s\n",
           nouveau_coach.nom, nouveau_coach.prenom, nouveau_coach.id,
           nouveau_coach.specialite, nouveau_coach.nbr_cours, nouveau_coach.coaching_prive);
    
    // Sauvegarder dans le fichier
    if (ajouter_dans_fichier_coach(nouveau_coach)) {
        printf("✅ Coach sauvegardé dans %s\n", COACH_FILE);
        gtk_label_set_text(GTK_LABEL(sortie), "✅ Coach sauvegardé !");
        
        // Rafraîchir le TreeView
        GtkWidget *treeview = lookup_widget(window, "treeview3");
        if (!treeview) treeview = lookup_widget(window, "treeview1");
        
        if (treeview) {
            printf("Rafraîchissement du TreeView...\n");
            afficher_coachs(treeview);
        }
    } else {
        printf("❌ Erreur de sauvegarde\n");
        gtk_label_set_text(GTK_LABEL(sortie), "❌ Erreur de sauvegarde !");
    }
}

// Fonction pour initialiser la combobox avec les spécialités (version GTK2)
void initialiser_combospe(GtkWidget *window) {
    GtkWidget *combospe = lookup_widget(window, "combospe");
    if (!combospe) {
        printf("❌ Combospe non trouvé pour initialisation\n");
        return;
    }
    
    printf("Initialisation de la combobox des spécialités\n");
    
    if (GTK_IS_COMBO(combospe)) {
        // Pour GtkCombo (GTK2) - méthode simplifiée
        const gchar *specialites[] = {
            "Boxe",
            "Fitness Général", 
            "Yoga",
            "Pilates",
            "Préparation Physique",
            "Musculation",
            NULL
        };
        
        // Créer une GList pour les spécialités
        GList *specialites_list = NULL;
        for (int i = 0; specialites[i] != NULL; i++) {
            specialites_list = g_list_append(specialites_list, (gpointer)specialites[i]);
            printf("Ajout spécialité: %s\n", specialites[i]);
        }
        
        // Définir les éléments dans la combo
        gtk_combo_set_popdown_strings(GTK_COMBO(combospe), specialites_list);
        
        // Libérer la liste
        g_list_free(specialites_list);
        
        printf("✅ Combobox initialisée avec %d spécialités\n", 6);
        
    } else if (GTK_IS_COMBO_BOX(combospe)) {
        // Pour GtkComboBox (GTK3+)
        GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
        GtkTreeIter iter;
        
        const gchar *specialites[] = {
            "Boxe",
            "Fitness Général", 
            "Yoga",
            "Pilates",
            "Préparation Physique",
            "Musculation",
            NULL
        };
        
        for (int i = 0; specialites[i] != NULL; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, specialites[i], -1);
        }
        
        gtk_combo_box_set_model(GTK_COMBO_BOX(combospe), GTK_TREE_MODEL(store));
        
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        gtk_cell_layout_clear(GTK_CELL_LAYOUT(combospe));
        gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combospe), renderer, TRUE);
        gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combospe), renderer,
                                       "text", 0, NULL);
        
        gtk_combo_box_set_active(GTK_COMBO_BOX(combospe), 0);
        g_object_unref(store);
    }
}
// Fonction pour initialiser la combobox des spécialités pour la modification
void initialiser_combo9(GtkWidget *window) {
    GtkWidget *combo9 = lookup_widget(window, "combo9");
    if (!combo9) {
        printf("❌ combo9 non trouvé pour initialisation\n");
        return;
    }
    
    printf("Initialisation de combo9 (modification)\n");
    
    if (GTK_IS_COMBO(combo9)) {
        // Pour GtkCombo (GTK2) - méthode simplifiée
        const gchar *specialites[] = {
            "Boxe",
            "Fitness Général", 
            "Yoga",
            "Pilates",
            "Préparation Physique",
            "Musculation",
            NULL
        };
        
        // Créer une GList pour les spécialités
        GList *specialites_list = NULL;
        for (int i = 0; specialites[i] != NULL; i++) {
            specialites_list = g_list_append(specialites_list, (gpointer)specialites[i]);
            printf("Ajout spécialité dans combo9: %s\n", specialites[i]);
        }
        
        // Définir les éléments dans la combo
        gtk_combo_set_popdown_strings(GTK_COMBO(combo9), specialites_list);
        
        // Libérer la liste
        g_list_free(specialites_list);
        
        printf("✅ combo9 initialisée avec %d spécialités\n", 6);
        
    } else if (GTK_IS_COMBO_BOX(combo9)) {
        // Pour GtkComboBox (GTK3+)
        GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
        GtkTreeIter iter;
        
        const gchar *specialites[] = {
            "Boxe",
            "Fitness Général", 
            "Yoga",
            "Pilates",
            "Préparation Physique",
            "Musculation",
            NULL
        };
        
        for (int i = 0; specialites[i] != NULL; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, specialites[i], -1);
        }
        
        gtk_combo_box_set_model(GTK_COMBO_BOX(combo9), GTK_TREE_MODEL(store));
        
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        gtk_cell_layout_clear(GTK_CELL_LAYOUT(combo9));
        gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo9), renderer, TRUE);
        gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo9), renderer,
                                       "text", 0, NULL);
        
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo9), 0);
        g_object_unref(store);
    }
}
// AJOUTEZ CETTE FONCTION MANQUANTE
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

void on_fenetre_principale_show(GtkWidget *widget, gpointer user_data) {
    printf("🚀 Fenêtre principale affichée\n");
    
    // Initialiser la combobox des spécialités
    initialiser_combospe(widget);
    
    // Charger les données dans le TreeView
    GtkWidget *treeview = lookup_widget(widget, "treeview3");
    if (!treeview) treeview = lookup_widget(widget, "treeview1");
    
    if (treeview) {
        printf("Chargement initial du TreeView\n");
        afficher_coachs(treeview);
    }
}

void on_treeview3_row_activated(GtkButton *button, gpointer user_data)
{
    // RÉCUPÉRER LES WIDGETS AVEC lookup_widget
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    // AFFICHER TOUS LES WIDGETS DISPONIBLES POUR DÉBOGUER
    printf("=== Recherche des widgets ===\n");
    
    // Essayer avec les noms que vous voyez dans vos logs
    GtkWidget *entry_nom = lookup_widget(window, "entry_nom");
    printf("entry_nom: %s\n", entry_nom ? "TROUVÉ" : "NON TROUVÉ");
    
    // Essayer d'autres noms possibles
    if (!entry_nom) entry_nom = lookup_widget(window, "entrynom1");
    printf("entrynom1: %s\n", entry_nom ? "TROUVÉ" : "NON TROUVÉ");
    
    GtkWidget *entry_prenom = lookup_widget(window, "entry_prenom");
    printf("entry_prenom: %s\n", entry_prenom ? "TROUVÉ" : "NON TROUVÉ");
    if (!entry_prenom) entry_prenom = lookup_widget(window, "entryprenom1");
    printf("entryprenom1: %s\n", entry_prenom ? "TROUVÉ" : "NON TROUVÉ");
    
    GtkWidget *entry_id = lookup_widget(window, "entry_id");
    printf("entry_id: %s\n", entry_id ? "TROUVÉ" : "NON TROUVÉ");
    if (!entry_id) entry_id = lookup_widget(window, "entryid1");
    printf("entryid1: %s\n", entry_id ? "TROUVÉ" : "NON TROUVÉ");
    
    GtkWidget *spin_cours = lookup_widget(window, "spin_cours");
    printf("spin_cours: %s\n", spin_cours ? "TROUVÉ" : "NON TROUVÉ");
    if (!spin_cours) spin_cours = lookup_widget(window, "spinbutton1");
    printf("spinbutton1: %s\n", spin_cours ? "TROUVÉ" : "NON TROUVÉ");
    
    GtkWidget *radio_oui = lookup_widget(window, "radio_oui");
    printf("radio_oui: %s\n", radio_oui ? "TROUVÉ" : "NON TROUVÉ");
    
    GtkWidget *label_status = lookup_widget(window, "label_status");
    printf("label_status: %s\n", label_status ? "TROUVÉ" : "NON TROUVÉ");
    if (!label_status) label_status = lookup_widget(window, "ros");
    printf("ros: %s\n", label_status ? "TROUVÉ" : "NON TROUVÉ");
    
    GtkWidget *treeview = lookup_widget(window, "treeview1");
    printf("treeview1: %s\n", treeview ? "TROUVÉ" : "NON TROUVÉ");
    if (!treeview) treeview = lookup_widget(window, "treeview3");
    printf("treeview3: %s\n", treeview ? "TROUVÉ" : "NON TROUVÉ");
    
    // Vérifier si les widgets existent
    if (!entry_nom || !entry_prenom || !entry_id || !spin_cours || !label_status) {
        printf("❌ Erreur: Certains widgets ne sont pas trouvés\n");
        return;
    }
    
    // LIRE LES VALEURS
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
    
    // VALIDATION
    if (strlen(nom) == 0 || strlen(prenom) == 0 || strlen(id_text) == 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Tous les champs sont obligatoires!");
        return;
    }
    
    int id = atoi(id_text);
    if (id <= 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "ID invalide!");
        return;
    }
    
    int nbr_cours = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_cours));
    
    // POUR LES RADIOBUTTONS
    gboolean coaching_prive = FALSE;
    if (radio_oui) {
        coaching_prive = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_oui));
    }
    
    // CRÉER LE COACH
    coach c;
    c.id = id;
    strcpy(c.nom, nom);
    strcpy(c.prenom, prenom);
    c.nbr_cours = nbr_cours;
    
    // Pour coaching_prive
    if (coaching_prive) {
        strcpy(c.coaching_prive, "oui");
    } else {
        strcpy(c.coaching_prive, "non");
    }
    
    // AJOUTER AU FICHIER
    if (ajouter_dans_fichier_coach(c)) {
        gtk_label_set_text(GTK_LABEL(label_status), "Coach ajouté avec succès!");
        
        // RAFRAÎCHIR LE TREEVIEW
        if (treeview) {
            afficher_coachs(treeview);
        }
        
        // RÉINITIALISER LES CHAMPS
        gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
        gtk_entry_set_text(GTK_ENTRY(entry_prenom), "");
        gtk_entry_set_text(GTK_ENTRY(entry_id), "");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_cours), 0);
        if (radio_oui) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_oui), FALSE);
        }
    } else {
        gtk_label_set_text(GTK_LABEL(label_status), "Erreur lors de l'ajout!");
    }
}

void on_actualiser_clicked(GtkWidget *button, gpointer user_data) {
    printf("🔄 Bouton Actualiser cliqué\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    // Chercher le TreeView
    GtkWidget *treeview = lookup_widget(window, "treeview3");
    if (!treeview) treeview = lookup_widget(window, "treeview1");
    if (!treeview) treeview = lookup_widget(window, "treeview");
    
    if (treeview != NULL) {
        printf("✅ TreeView trouvé, actualisation...\n");
        afficher_coachs(treeview);
        printf("✅ TreeView actualisé avec les données du fichier\n");
    } else {
        printf("❌ TreeView non trouvé!\n");
    }
}

void on_fenetre_modification_show(GtkWidget *widget, gpointer user_data) {
    printf("🔄 Fenêtre modification affichée\n");
    
    // Initialiser la combobox des spécialités pour modification
    initialiser_combo9(widget);
    
    // Charger les IDs
    GtkWidget *combo_id = lookup_widget(widget, "combobox_id");
    if (combo_id) {
        charger_ids_dans_combo(combo_id);
    }

}

// Callback pour remplir les champs quand on sélectionne un ID
void on_combobox_id_changed(GtkComboBox *combo, gpointer user_data) {
    printf("🔍 ID sélectionné dans le ComboBox\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(combo));
    
    // Récupérer l'ID sélectionné
    GtkTreeIter iter;
    GtkTreeModel *model;
    int id_selectionne = -1;
    
    if (gtk_combo_box_get_active_iter(combo, &iter)) {
        model = gtk_combo_box_get_model(combo);
        gtk_tree_model_get(model, &iter, 0, &id_selectionne, -1);
        
        printf("📌 ID sélectionné: %d\n", id_selectionne);
        
        if (id_selectionne > 0) {
            // Chercher le coach dans le fichier
            FILE *f = fopen(COACH_FILE, "r");
            if (f != NULL) {
                char ligne[256];
                int trouve = 0;
                
                while (fgets(ligne, sizeof(ligne), f) && !trouve) {
                    ligne[strcspn(ligne, "\n")] = 0;
                    
                    char ligne_copie[256];
                    strcpy(ligne_copie, ligne);
                    
                    char *nom = strtok(ligne_copie, ";");
                    char *prenom = strtok(NULL, ";");
                    char *id_str = strtok(NULL, ";");
                    char *specialite = strtok(NULL, ";");
                    char *cours_str = strtok(NULL, ";");
                    char *coaching_prive = strtok(NULL, ";");
                    
                    if (nom && prenom && id_str) {
                        int id = atoi(id_str);
                        
                        if (id == id_selectionne) {
                            trouve = 1;
                            
                            // Récupérer les widgets pour pré-remplir
                            GtkWidget *entry_nom = lookup_widget(window, "entrynom2");
                            GtkWidget *entry_prenom = lookup_widget(window, "entryprenom2");
                            GtkWidget *combo9 = lookup_widget(window, "combo9");  // Nouvelle combobox
                            GtkWidget *spin_cours = lookup_widget(window, "coursmodifier");
                            GtkWidget *radio_oui = lookup_widget(window, "oui22");
                            GtkWidget *radio_non = lookup_widget(window, "non22");
                            
                            if (entry_nom) gtk_entry_set_text(GTK_ENTRY(entry_nom), nom);
                            if (entry_prenom) gtk_entry_set_text(GTK_ENTRY(entry_prenom), prenom);
                            if (spin_cours && cours_str) gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_cours), atoi(cours_str));
                            
                            // Pré-remplir la combobox spécialité (combo9)
                            if (combo9 && specialite) {
                                printf("Spécialité à sélectionner: %s\n", specialite);
                                
                                if (GTK_IS_COMBO(combo9)) {
                                    // Pour GtkCombo
                                    GtkWidget *entry = GTK_COMBO(combo9)->entry;
                                    gtk_entry_set_text(GTK_ENTRY(entry), specialite);
                                    printf("combo9 pré-remplie avec: %s\n", specialite);
                                } else if (GTK_IS_COMBO_BOX(combo9)) {
                                    // Pour GtkComboBox - chercher la spécialité dans la liste
                                    GtkTreeModel *model_combo = gtk_combo_box_get_model(GTK_COMBO_BOX(combo9));
                                    GtkTreeIter iter_combo;
                                    gboolean found = FALSE;
                                    
                                    if (gtk_tree_model_get_iter_first(model_combo, &iter_combo)) {
                                        do {
                                            gchar *spe_temp = NULL;
                                            gtk_tree_model_get(model_combo, &iter_combo, 0, &spe_temp, -1);
                                            
                                            if (spe_temp && strcmp(spe_temp, specialite) == 0) {
                                                gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo9), &iter_combo);
                                                found = TRUE;
                                                printf("Spécialité trouvée dans combo9: %s\n", specialite);
                                            }
                                            
                                            if (spe_temp) g_free(spe_temp);
                                            
                                        } while (!found && gtk_tree_model_iter_next(model_combo, &iter_combo));
                                    }
                                    
                                    // Si non trouvé, utiliser l'entrée
                                    if (!found && GTK_IS_COMBO_BOX_ENTRY(combo9)) {
                                        GtkWidget *entry = gtk_bin_get_child(GTK_BIN(combo9));
                                        gtk_entry_set_text(GTK_ENTRY(entry), specialite);
                                        printf("Spécialité entrée manuellement: %s\n", specialite);
                                    }
                                }
                            }
                            
                            // Configurer les radio boutons
                            if (radio_oui && radio_non) {
                                if (strcmp(coaching_prive, "oui") == 0) {
                                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_oui), TRUE);
                                } else {
                                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio_non), TRUE);
                                }
                            }
                            
                            printf("✅ Données chargées pour ID %d\n", id);
                        }
                    }
                }
                fclose(f);
                
                if (!trouve) {
                    printf("⚠️ Coach non trouvé\n");
                }
            }
        }
    }
}

void on_buttonmodifier_clicked(GtkWidget *button, gpointer user_data)
{
    printf("🔄 Bouton Modifier cliqué\n");
    
    GtkWidget *window = gtk_widget_get_toplevel(button);
    
    // Récupérer les widgets
    GtkWidget *combo_id = lookup_widget(window, "modifier1");
    GtkWidget *entry_nom = lookup_widget(window, "entrynom2");
    GtkWidget *entry_prenom = lookup_widget(window, "entryprenom2");
    GtkWidget *combo9 = lookup_widget(window, "combo9");  // Nouvelle combobox pour spécialité
    GtkWidget *spin_cours = lookup_widget(window, "coursmodifier");
    GtkWidget *radio_oui = lookup_widget(window, "oui22");
    GtkWidget *label_status = lookup_widget(window, "modifier");
    GtkWidget *treeview = lookup_widget(window, "treeview3");
    
    if (!combo_id || !entry_nom || !entry_prenom || !combo9 ||  
        !spin_cours || !radio_oui || !label_status) {
        printf("❌ Widgets non trouvés\n");
        printf("combo9: %s\n", combo9 ? "TROUVÉ" : "NON TROUVÉ");
        return;
    }
    
    // Récupérer l'ID sélectionné
    GtkTreeIter iter;
    GtkTreeModel *model;
    int id_selectionne = -1;
    
    if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo_id), &iter)) {
        gtk_label_set_text(GTK_LABEL(label_status), "Veuillez sélectionner un coach!");
        return;
    }
    
    model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo_id));
    gtk_tree_model_get(model, &iter, 0, &id_selectionne, -1);
    
    // Récupérer les nouvelles données
    const gchar *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const gchar *prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
    int nbr_cours = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_cours));
    gboolean coaching_prive = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_oui));
    
    // Récupérer la spécialité depuis combo9
    const gchar *specialite = "";
    
    if (GTK_IS_COMBO(combo9)) {
        // Pour GtkCombo
        GtkWidget *entry = GTK_COMBO(combo9)->entry;
        specialite = gtk_entry_get_text(GTK_ENTRY(entry));
        printf("Spécialité (combo9 - GtkCombo): %s\n", specialite);
    } else if (GTK_IS_COMBO_BOX(combo9)) {
        // Pour GtkComboBox
        GtkTreeIter iter_spe;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo9), &iter_spe)) {
            GtkTreeModel *model_spe = gtk_combo_box_get_model(GTK_COMBO_BOX(combo9));
            gchar *specialite_temp = NULL;
            gtk_tree_model_get(model_spe, &iter_spe, 0, &specialite_temp, -1);
            if (specialite_temp) {
                specialite = specialite_temp;
                printf("Spécialité (combo9 - GtkComboBox): %s\n", specialite);
                g_free(specialite_temp);
            }
        }
    }
    
    // Validation
    if (strlen(nom) == 0 || strlen(prenom) == 0 || strlen(specialite) == 0) {
        gtk_label_set_text(GTK_LABEL(label_status), "Tous les champs sont obligatoires!");
        return;
    }
    
    // Créer l'objet coach modifié
    coach nouv;
    nouv.id = id_selectionne;
    strcpy(nouv.nom, nom);
    strcpy(nouv.prenom, prenom);
    strcpy(nouv.specialite, specialite);  // Ajouter la spécialité
    nouv.nbr_cours = nbr_cours;
    
    if (coaching_prive) {
        strcpy(nouv.coaching_prive, "oui");
    } else {
        strcpy(nouv.coaching_prive, "non");
    }
    
    // Modifier dans le fichier
    if (modifier_coach(id_selectionne, nouv)) {
        gtk_label_set_text(GTK_LABEL(label_status), "✅ Coach modifié avec succès!");
        
        // Recharger le TreeView si disponible
        if (treeview) {
            afficher_coachs(treeview);
        }
        
        // Recharger les IDs dans le ComboBox
        charger_ids_dans_combo(combo_id);
        
        printf("✅ Modification réussie pour ID %d\n", id_selectionne);
    } else {
        gtk_label_set_text(GTK_LABEL(label_status), "❌ Erreur lors de la modification!");
        printf("❌ Échec de la modification\n");
    }
}
