#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "membre.h"

/* ----------------- Global variables for dates ----------------- */
guint g_jour_naiss = 1, g_mois_naiss = 1, g_annee_naiss = 2000;
guint g_jour_inscrip = 1, g_mois_inscription = 1, g_annee_inscrip = 2025;

/* ----------------- Calendar Callbacks ----------------- */
void on_calendar_date_de_naiss_membre_day_selected(GtkWidget *widget, gpointer user_data) {
    guint y, m, d;
    gtk_calendar_get_date(GTK_CALENDAR(widget), &y, &m, &d);
    g_jour_naiss = d;
    g_mois_naiss = m + 1;
    g_annee_naiss = y;
    g_print("Date de naissance sélectionnée : %02d/%02d/%04d\n", d, m+1, y);
}

void on_calendar_date_inscrip_day_selected(GtkWidget *widget, gpointer user_data) {
    guint y, m, d;
    gtk_calendar_get_date(GTK_CALENDAR(widget), &y, &m, &d);
    g_jour_inscrip = d;
    g_mois_inscription = m + 1;
    g_annee_inscrip = y;
    g_print("Date d'inscription sélectionnée : %02d/%02d/%04d\n", d, m+1, y);
}

/* ----------------- Add Member ----------------- */
void on_button_save_ajouts_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *entry_id    = lookup_widget(button, "entry_id_membre");
    GtkWidget *entry_nom   = lookup_widget(button, "entry_nom_membre");
    GtkWidget *entry_pr    = lookup_widget(button, "entry_pr__nom_membre");
    GtkWidget *entry_tel   = lookup_widget(button, "entry_t__lf_membre");
    GtkWidget *entry_email = lookup_widget(button, "entry_email_membre");
    GtkWidget *entry_long  = lookup_widget(button, "entry_longueur_membre");
    GtkWidget *entry_poids = lookup_widget(button, "entry_poids_membre");
    GtkWidget *entry_frais = lookup_widget(button, "entry_frais_pay__");
    GtkWidget *combo_type  = lookup_widget(button, "comboboxtype");
    GtkWidget *spin_duree  = lookup_widget(button, "spinbutton_dure");

    if (!entry_id || !entry_nom || !entry_pr || !entry_tel || !entry_email ||
        !entry_long || !entry_poids || !entry_frais || !combo_type || !spin_duree) {
        g_print("ERROR: One or more widgets not found!\n");
        return;
    }

    membre m;
    memset(&m, 0, sizeof(m));

    strncpy(m.id,     gtk_entry_get_text(GTK_ENTRY(entry_id)),     sizeof(m.id)-1);
    strncpy(m.nom,    gtk_entry_get_text(GTK_ENTRY(entry_nom)),    sizeof(m.nom)-1);
    strncpy(m.prenom, gtk_entry_get_text(GTK_ENTRY(entry_pr)),     sizeof(m.prenom)-1);
    strncpy(m.tel,    gtk_entry_get_text(GTK_ENTRY(entry_tel)),    sizeof(m.tel)-1);
    strncpy(m.email,  gtk_entry_get_text(GTK_ENTRY(entry_email)),  sizeof(m.email)-1);

    m.longueur   = atof(gtk_entry_get_text(GTK_ENTRY(entry_long)));
    m.poids      = atof(gtk_entry_get_text(GTK_ENTRY(entry_poids)));
    m.frais_paye = atof(gtk_entry_get_text(GTK_ENTRY(entry_frais)));

    // ComboBox type
    {
        GtkTreeIter iter;
        GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo_type));
        gchar *value = NULL;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo_type), &iter)) {
            gtk_tree_model_get(model, &iter, 0, &value, -1);
            if (value) {
                strncpy(m.type_abonnement, value, sizeof(m.type_abonnement)-1);
                g_free(value);
            }
        }
    }

    int duree = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_duree));
    snprintf(m.duree_abonnement, sizeof(m.duree_abonnement), "%d mois", duree);

    m.jour_naissance    = g_jour_naiss;
    m.mois_naissance    = g_mois_naiss;
    m.annee_naissance   = g_annee_naiss;
    m.jour_inscription   = g_jour_inscrip;
    m.mois_inscription   = g_mois_inscription;
    m.annee_inscription  = g_annee_inscrip;

    if (ajouter_membre(&m, "membre_test.txt")) {
        g_print("Member added: ID=%s\n", m.id);
    } else {
        g_print("ERROR: Could not add member\n");
    }
}

/* ----------------- Reset Add Form ----------------- */
void on_button_delete_ajouts_clicked(GtkWidget *widget, gpointer user_data) {
    const char *ids[] = {
        "entry_id_membre", "entry_nom_membre", "entry_pr__nom_membre",
        "entry_t__lf_membre", "entry_email_membre", "entry_longueur_membre",
        "entry_poids_membre", "entry_frais_pay__"
    };
    for (int i = 0; i < 8; i++) {
        GtkWidget *e = lookup_widget(widget, ids[i]);
        if (e) gtk_entry_set_text(GTK_ENTRY(e), "");
    }
    GtkWidget *combo = lookup_widget(widget, "comboboxtype");
    if (combo) gtk_combo_box_set_active(GTK_COMBO_BOX(combo), -1);
    GtkWidget *spin = lookup_widget(widget, "spinbutton_dure");
    if (spin) gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), 1);
    g_print("Formulaire vidé (Reset OK)\n");
}

/* ----------------- Refresh TreeView ----------------- */
void on_button_actualiser_liste_membre_clicked(GtkWidget *widget, gpointer user_data) {
    GtkWidget *tv = lookup_widget(widget, "treeview1");
    if (!tv) {
        g_print("ERROR: treeview1 not found!\n");
        return;
    }

    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tv)));
    if (!store) {
        store = gtk_list_store_new(12,
            G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
            G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
            G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING
        );
        gtk_tree_view_set_model(GTK_TREE_VIEW(tv), GTK_TREE_MODEL(store));
        g_object_unref(store);

        const char *titles[12] = {
            "ID","Nom","Prenom","Phone","Email",
            "Date Naissance","Date Inscrip","Longueur",
            "Poids","Durée","Type","Frais"
        };
        for (int i = 0; i < 12; i++) {
            GtkCellRenderer *r = gtk_cell_renderer_text_new();
            GtkTreeViewColumn *c = gtk_tree_view_column_new_with_attributes(
                titles[i], r, "text", i, NULL);
            gtk_tree_view_append_column(GTK_TREE_VIEW(tv), c);
        }
    }

    if (!charger_membres(store, "membre_test.txt")) {
        g_print("ERROR: Could not load members from file\n");
    } else {
        g_print("Members loaded into treeview\n");
    }
}

/* ----------------- Search for Modification ----------------- */
void on_button_chercher_pour_modif_clicked(GtkWidget *button, gpointer user_data) {
    const char *id = gtk_entry_get_text(GTK_ENTRY(lookup_widget(button, "entry_chercher_membre")));
    if (!id || strlen(id) == 0) return;

    GtkWidget *entry_id     = lookup_widget(button, "entry_id_membre_modif");
    GtkWidget *entry_nom    = lookup_widget(button, "entry_nom_membre_modif");
    GtkWidget *entry_prenom = lookup_widget(button, "entry_prenom_membre_modif");
    GtkWidget *entry_tel    = lookup_widget(button, "entry_tlf_membre_modif");
    GtkWidget *entry_email  = lookup_widget(button, "entry_tlf_modif");
    GtkWidget *entry_long   = lookup_widget(button, "entry_long_modif");
    GtkWidget *entry_poids  = lookup_widget(button, "entry_poids_modif");
    GtkWidget *entry_frais  = lookup_widget(button, "entry_frais_modif");
    GtkWidget *combo_type   = lookup_widget(button, "comboboxtype_modif");
    GtkWidget *spin_duree   = lookup_widget(button, "spinbutton_dure_modif");
    GtkWidget *cal_naiss   = lookup_widget(button, "calendar_date_naissance_modif");
    GtkWidget *cal_inscr   = lookup_widget(button, "calendar_inscrip_modif");

    if (!entry_id || !entry_nom || !entry_prenom || !entry_tel || !entry_email ||
        !entry_long || !entry_poids || !entry_frais || !combo_type ||
        !spin_duree || !cal_naiss || !cal_inscr)
    {
        g_print("ERROR: Some widgets not found (modif search)\n");
        return;
    }

    /* Load all members into a temporary list, then find the one with matching ID */
    GtkListStore *store = gtk_list_store_new(12,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING
    );
    if (!charger_membres(store, "membre_test.txt")) {
        g_print("ERROR: Could not load members file for search\n");
        g_object_unref(store);
        return;
    }

    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while (valid) {
        gchar *fid;
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &fid, -1);
        if (strcmp(fid, id) == 0) {
            gchar *fields[12];
            for (int c = 0; c < 12; c++) {
                gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, c, &fields[c], -1);
            }
            // Fill entries
            gtk_entry_set_text(GTK_ENTRY(entry_id),     fields[0]);
            gtk_entry_set_text(GTK_ENTRY(entry_nom),    fields[1]);
            gtk_entry_set_text(GTK_ENTRY(entry_prenom), fields[2]);
            gtk_entry_set_text(GTK_ENTRY(entry_tel),    fields[3]);
            gtk_entry_set_text(GTK_ENTRY(entry_email),  fields[4]);

            // Dates
            {
                int d, mo, y;
                sscanf(fields[5], "%d/%d/%d", &d, &mo, &y);
                gtk_calendar_select_month(GTK_CALENDAR(cal_naiss), mo-1, y);
                gtk_calendar_select_day(GTK_CALENDAR(cal_naiss), d);
                sscanf(fields[6], "%d/%d/%d", &d, &mo, &y);
                gtk_calendar_select_month(GTK_CALENDAR(cal_inscr), mo-1, y);
                gtk_calendar_select_day(GTK_CALENDAR(cal_inscr), d);
            }

            gtk_entry_set_text(GTK_ENTRY(entry_long),  fields[7]);
            gtk_entry_set_text(GTK_ENTRY(entry_poids), fields[8]);
            gtk_entry_set_text(GTK_ENTRY(entry_frais), fields[11]); // frais
            gtk_entry_set_text(GTK_ENTRY(entry_frais), fields[9]);  // durée? or frais — depends on your format

            // Combo + spin
            int type_idx = 0;
            if (strcmp(fields[10], "Journalier") == 0)       type_idx = 0;
            else if (strcmp(fields[10], "Hebdomadaire") == 0) type_idx = 1;
            else if (strcmp(fields[10], "Mensuel") == 0)      type_idx = 2;
            else if (strcmp(fields[10], "Trimestriel") == 0)  type_idx = 3;
            else if (strcmp(fields[10], "Semestriel") == 0)   type_idx = 4;
            else if (strcmp(fields[10], "Annuel") == 0)       type_idx = 5;
            gtk_combo_box_set_active(GTK_COMBO_BOX(combo_type), type_idx);

            int duree = atoi(fields[9]);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_duree), duree);

            // free fields strings
            for (int c = 0; c < 12; c++) g_free(fields[c]);
            break;
        }
        g_free(fid);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }

    g_object_unref(store);
}

/* ----------------- Modify Member ----------------- */
void on_button_modif_clicked(GtkWidget *widget, gpointer user_data) {
    GtkWidget *entry_search = lookup_widget(widget, "entry_chercher_membre");
    const char *id = gtk_entry_get_text(GTK_ENTRY(entry_search));
    if (!id || strlen(id) == 0) { g_print("ERROR: No ID provided\n"); return; }

    GtkWidget *entry_id     = lookup_widget(widget, "entry_id_membre_modif");
    GtkWidget *entry_nom    = lookup_widget(widget, "entry_nom_membre_modif");
    GtkWidget *entry_prenom = lookup_widget(widget, "entry_prenom_membre_modif");
    GtkWidget *entry_tel    = lookup_widget(widget, "entry_tlf_membre_modif");
    GtkWidget *entry_email  = lookup_widget(widget, "entry_tlf_modif");
    GtkWidget *entry_long   = lookup_widget(widget, "entry_long_modif");
    GtkWidget *entry_poids  = lookup_widget(widget, "entry_poids_modif");
    GtkWidget *entry_frais  = lookup_widget(widget, "entry_frais_modif");
    GtkWidget *combo_type   = lookup_widget(widget, "comboboxtype_modif");
    GtkWidget *spin_duree   = lookup_widget(widget, "spinbutton_dure_modif");
    GtkWidget *cal_naiss    = lookup_widget(widget, "calendar_date_naissance_modif");
    GtkWidget *cal_inscr    = lookup_widget(widget, "calendar_inscrip_modif");

    if (!entry_id || !entry_nom || !entry_prenom || !entry_tel || !entry_email ||
        !entry_long || !entry_poids || !entry_frais || !combo_type ||
        !spin_duree || !cal_naiss || !cal_inscr)
    {
        g_print("ERROR: Some widgets missing for modify\n");
        return;
    }

    membre m;
    memset(&m, 0, sizeof(m));
    strncpy(m.id,     gtk_entry_get_text(GTK_ENTRY(entry_id)),     sizeof(m.id)-1);
    strncpy(m.nom,    gtk_entry_get_text(GTK_ENTRY(entry_nom)),    sizeof(m.nom)-1);
    strncpy(m.prenom, gtk_entry_get_text(GTK_ENTRY(entry_prenom)), sizeof(m.prenom)-1);
    strncpy(m.tel,    gtk_entry_get_text(GTK_ENTRY(entry_tel)),    sizeof(m.tel)-1);
    strncpy(m.email,  gtk_entry_get_text(GTK_ENTRY(entry_email)),  sizeof(m.email)-1);

    m.longueur   = atof(gtk_entry_get_text(GTK_ENTRY(entry_long)));
    m.poids      = atof(gtk_entry_get_text(GTK_ENTRY(entry_poids)));
    m.frais_paye = atof(gtk_entry_get_text(GTK_ENTRY(entry_frais)));

    // Combo type
    {
        GtkTreeIter iter;
        GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo_type));
        gchar *value = NULL;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo_type), &iter)) {
            gtk_tree_model_get(model, &iter, 0, &value, -1);
            if (value) {
                strncpy(m.type_abonnement, value, sizeof(m.type_abonnement)-1);
                g_free(value);
            }
        }
    }

    int duree = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_duree));
    snprintf(m.duree_abonnement, sizeof(m.duree_abonnement), "%d mois", duree);

    {
        guint y, mo, d;
        gtk_calendar_get_date(GTK_CALENDAR(cal_naiss), &y, &mo, &d);
        m.jour_naissance = d;
        m.mois_naissance = mo + 1;
        m.annee_naissance = y;
        gtk_calendar_get_date(GTK_CALENDAR(cal_inscr), &y, &mo, &d);
        m.jour_inscription = d;
        m.mois_inscription = mo + 1;
        m.annee_inscription = y;
    }

    if (modifier_membre(id, &m, "membre_test.txt")) {
        g_print("Member %s modified\n", id);
    } else {
        g_print("ERROR: Could not modify member %s\n", id);
    }
}

/* ----------------- Delete Member ----------------- */
void on_button_supprimer_clicked(GtkWidget *widget, gpointer user_data) {
    GtkWidget *entry_id = lookup_widget(widget, "entry_id_membre_modif");
    const char *id = gtk_entry_get_text(GTK_ENTRY(entry_id));
    if (!id || strlen(id)==0) {
        g_print("ERROR: No ID to delete\n");
        return;
    }

    if (supprimer_membre(id, "membre_test.txt")) {
        g_print("Member %s deleted\n", id);
    } else {
        g_print("ERROR: Could not delete member %s\n", id);
    }

    // Optional: clear UI fields
}

/* ----------------- Calculate Stats ----------------- */
void on_button_calculer_statis_clicked(GtkWidget *widget, gpointer user_data) {
    GtkWidget *e_total     = lookup_widget(widget, "entry_total");
    GtkWidget *e_actifs    = lookup_widget(widget, "entry_actifs");
    GtkWidget *e_inactifs  = lookup_widget(widget, "entry_inactif");
    GtkWidget *e_pct       = lookup_widget(widget, "entry_pourcentage_actif");

    int total, actifs, inactifs;
    float pct;
    if (!calculer_statistiques("membre_test.txt", &total, &actifs, &inactifs, &pct)) {
        g_print("ERROR: Could not compute statistics\n");
        return;
    }

    if (e_total)    gtk_entry_set_text(GTK_ENTRY(e_total),    g_strdup_printf("%d", total));
    if (e_actifs)   gtk_entry_set_text(GTK_ENTRY(e_actifs),   g_strdup_printf("%d", actifs));
    if (e_inactifs) gtk_entry_set_text(GTK_ENTRY(e_inactifs), g_strdup_printf("%d", inactifs));
    if (e_pct)      gtk_entry_set_text(GTK_ENTRY(e_pct),      g_strdup_printf("%.2f%%", pct));
    g_print("Stats updated: total=%d, actifs=%d, inactifs=%d, pct=%.2f%%\n", total, actifs, inactifs, pct);
}

/* ----------------- Calculate IMC ----------------- */
void on_button_imc_clicked(GtkWidget *widget, gpointer user_data) {
    GtkWidget *entry_id = lookup_widget(widget, "entry_id_membre_imc");
    GtkWidget *entry_res= lookup_widget(widget, "entry_res_imc");
    if (!entry_id || !entry_res) { g_print("ERROR: IMC widgets not found\n"); return; }
    const char *id = gtk_entry_get_text(GTK_ENTRY(entry_id));
    float imc = calculer_imc(id, "membre_test.txt");
    if (imc > 0) gtk_entry_set_text(GTK_ENTRY(entry_res), g_strdup_printf("%.2f", imc));
    else        gtk_entry_set_text(GTK_ENTRY(entry_res), "N/A");
}

/* ----------------- Account / Authentication buttons ----------------- */
void on_button_entrer_clicked(GtkWidget *objet_graphique, gpointer user_data) {
    GtkWidget *entry_user = lookup_widget(objet_graphique, "entry_username");
    GtkWidget *entry_pass = lookup_widget(objet_graphique, "entry_passeword");
    GtkWidget *notebook   = lookup_widget(objet_graphique, "notebook1");
    const char *u = gtk_entry_get_text(GTK_ENTRY(entry_user));
    const char *p = gtk_entry_get_text(GTK_ENTRY(entry_pass));
    int found = 0;

    FILE *f = fopen("compte.txt","r");
    if (f) {
        char u2[50], p2[50];
        while (fscanf(f, "%49[^;];%49s\n", u2, p2)==2) {
            if (strcmp(u, u2)==0 && strcmp(p, p2)==0) { found = 1; break; }
        }
        fclose(f);
    }

    if (found && notebook) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
    } else {
        g_print("Wrong username/password\n");
    }
}

void on_button_cr__er_compte_clicked(GtkWidget *objet_graphique, gpointer user_data) {
    GtkWidget *notebook = lookup_widget(objet_graphique, "notebook1");
    if (notebook) gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 8);
}

void on_button_add_account_clicked(GtkWidget *objet_graphique, gpointer user_data) {
    GtkWidget *e_user = lookup_widget(objet_graphique, "entry_user_name");
    GtkWidget *e_pass = lookup_widget(objet_graphique, "entry_password_new");
    if (!e_user || !e_pass) { g_print("ERROR: account widgets not found\n"); return; }

    const char *u = gtk_entry_get_text(GTK_ENTRY(e_user));
    const char *p = gtk_entry_get_text(GTK_ENTRY(e_pass));

    FILE *f = fopen("compte.txt","a");
    if (!f) { g_print("ERROR: Cannot open compte.txt\n"); return; }
    fprintf(f, "%s;%s\n", u, p);
    fclose(f);
    gtk_entry_set_text(GTK_ENTRY(e_user),"");
    gtk_entry_set_text(GTK_ENTRY(e_pass),"");
    g_print("New account added\n");
}



void on_button_chercher_membre_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry = lookup_widget(objet_graphique, "entry_membre_a_chercher");
    GtkWidget *treeview = lookup_widget(objet_graphique, "treeview1");

    const char *id_to_find = gtk_entry_get_text(GTK_ENTRY(entry));

    if (id_to_find == NULL || strlen(id_to_find) == 0) {
        g_print("ID vide\n");
        return;
    }

    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    GtkTreeIter iter;

    gboolean valid = gtk_tree_model_get_iter_first(model, &iter);

    while (valid) {
        char *current_id = NULL;
        gtk_tree_model_get(model, &iter, 0, &current_id, -1); // column 0 = ID

        if (current_id && strcmp(current_id, id_to_find) == 0) {
            GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
            gtk_tree_selection_select_iter(selection, &iter);

            GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
            gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(treeview), path, NULL, TRUE, 0.5, 0.0);
            gtk_tree_path_free(path);

            g_free(current_id);
            return;
        }

        g_free(current_id);
        valid = gtk_tree_model_iter_next(model, &iter);
    }

    // If we reach here: NOT FOUND
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                    GTK_DIALOG_MODAL,
                    GTK_MESSAGE_WARNING,
                    GTK_BUTTONS_OK,
                    "Membre introuvable !");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


void on_button_reset_clicked(GtkWidget *widget, gpointer user_data) {
    GtkWidget *tv = lookup_widget(widget, "treeview2");
    if (!tv) {
        g_print("ERROR: treeview2 not found!\n");
        return;
    }

    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tv)));

    /* If no model yet, create it and create the columns */
    if (!store) {
        store = gtk_list_store_new(6,
            G_TYPE_STRING, // Type
            G_TYPE_STRING, // Accès
            G_TYPE_STRING, // Sauna/Hammam
            G_TYPE_STRING, // Coaching personnalisé
            G_TYPE_STRING, // Durée
            G_TYPE_STRING  // Prix
        );
        gtk_tree_view_set_model(GTK_TREE_VIEW(tv), GTK_TREE_MODEL(store));
        /* unref here is fine because the treeview holds a reference now */
        g_object_unref(store);

        const char *titles[6] = {
            "Type", "Accès", "Sauna/Hammam", "Coaching", "Durée", "Prix"
        };

        for (int i = 0; i < 6; i++) {
            GtkCellRenderer *r = gtk_cell_renderer_text_new();
            GtkTreeViewColumn *c = gtk_tree_view_column_new_with_attributes(
                titles[i], r, "text", i, NULL);
            gtk_tree_view_append_column(GTK_TREE_VIEW(tv), c);
        }

        /* retrieve the store pointer back from the treeview's model */
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tv)));
    }

    /* Now load data into the store (charger_abonnements is in abonnement.c) */
    if (!charger_abonnements(store, "abon.txt")) {
        g_print("ERROR: Could not load abonnements from abonnements.txt\n");
    } else {
        g_print("Abonnements loaded into treeview2\n");
    }
}

void on_treeview2_row_activated(GtkTreeView *treeview,
                                GtkTreePath *path,
                                GtkTreeViewColumn *column,
                                gpointer user_data)
{
    GtkTreeIter iter;
    gchar *type;
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    if (!model) return;

    // Vérifie que l'itérateur existe pour la ligne double-cliquée
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter,
                           0, &type,  // On récupère seulement le type pour identifier
                           -1);

        g_print("Suppression de l'abonnement: %s\n", type);

        // Supprime la ligne du TreeView
        gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

        // Supprime du fichier
        if (!supprimer_abonnement(type, "abon.txt")) {
            g_print("Erreur: impossible de supprimer %s du fichier\n", type);
        }

        g_free(type);
    }
}


void
on_button_chercher_abon_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry = lookup_widget(objet_graphique, "entry16");
    GtkWidget *treeview = lookup_widget(objet_graphique, "treeview2");

    if (!entry || !treeview) {
        g_print("ERROR: entry16 or treeview2 missing !\n");
        return;
    }

    const char *type_to_find = gtk_entry_get_text(GTK_ENTRY(entry));

    if (!type_to_find || strlen(type_to_find) == 0) {
        g_print("Type vide !\n");
        return;
    }

    int ok = rechercher_abonnement(treeview, type_to_find);

    if (!ok) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Abonnement introuvable !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_calendar_date_sport_day_selected(GtkWidget *objet_graphique, gpointer user_data)
{
    int j, m, a;
    lire_date_selectionnee(objet_graphique, &j, &m, &a);
    g_print("Date choisie : %d/%d/%d\n", j, m, a);
}


void on_checkbutton1_toggled(GtkWidget *objet_graphique, gpointer user_data)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(objet_graphique)))
        g_print("Séance en salle sélectionnée\n");
}

void on_checkbutton2_toggled(GtkWidget *objet_graphique, gpointer user_data)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(objet_graphique)))
        g_print("Séanceen en ligne sélectionnée\n");
}

void on_radiobutton2_toggled(GtkWidget *objet_graphique, gpointer user_data)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(objet_graphique)))
        g_print("Sans matériel\n");
}

void on_radiobutton3_toggled(GtkWidget *objet_graphique, gpointer user_data)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(objet_graphique)))
        g_print("Avec matériel\n");
}

void on_button_envoi_demande_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    demande_coach d;

    GtkWidget *entry_id = lookup_widget(objet_graphique, "entry_mem_demande");
    GtkWidget *entry_h = lookup_widget(objet_graphique, "entry_heure");
    GtkWidget *entry_comment = lookup_widget(objet_graphique, "entry_comment");

    GtkWidget *combo_type = lookup_widget(objet_graphique, "combobox_type_demande");
    GtkWidget *combo_coach = lookup_widget(objet_graphique, "combobox_coach");
    GtkWidget *calendar = lookup_widget(objet_graphique, "calendar_date_sport");

    GtkWidget *cb_online = lookup_widget(objet_graphique, "checkbutton1");
    GtkWidget *cb_salle = lookup_widget(objet_graphique, "checkbutton2");

    GtkWidget *rb_avec = lookup_widget(objet_graphique, "radiobutton2");
    GtkWidget *rb_sans = lookup_widget(objet_graphique, "radiobutton3");

    strcpy(d.nom_membre, gtk_entry_get_text(GTK_ENTRY(entry_id)));
    strcpy(d.heure, gtk_entry_get_text(GTK_ENTRY(entry_h)));

    strcpy(d.commentaire, gtk_entry_get_text(GTK_ENTRY(entry_comment)));

    strcpy(d.type_entrainement,
           gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_type)));

    strcpy(d.coach_choisi,
           gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_coach)));

    lire_date_selectionnee(calendar,
                           &d.jour_souhaite,
                           &d.mois_souhaite,
                           &d.annee_souhaitee);

    get_type_seance(cb_online, cb_salle, d.type_seance);
    get_materiel(rb_avec, rb_sans, d.materiel);

    if (ajouter_demande_coach(d, "demande_coach.txt"))
        g_print("Demande envoyée avec succès !\n");
    else
        g_print("ERREUR: impossible d'enregistrer la demande\n");
}

void on_button_annuler_demande_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry = lookup_widget(objet_graphique, "entry_mem_demande");
    GtkWidget *entry_heure = lookup_widget(objet_graphique, "entry_heure");
    GtkWidget *entry_comment = lookup_widget(objet_graphique, "entry_comment");

    gtk_entry_set_text(GTK_ENTRY(entry), "");
    gtk_entry_set_text(GTK_ENTRY(entry_heure), "");
    gtk_entry_set_text(GTK_ENTRY(entry_comment), "");

    g_print("Formulaire réinitialisé\n");
}






/* ----------------- Simple debug buttons ----------------- */


void on_button_exporter_liste_membre_clicked(GtkWidget *objet_graphique, gpointer user_data) { g_print("Exporter liste clicked\n"); }

/* ----------------- Calendar modify callbacks ----------------- */
void on_calendar_date_naissance_modif_day_selected(GtkWidget *objet_graphique, gpointer user_data) {}
void on_calendar_inscrip_modif_day_selected(GtkWidget *objet_graphique, gpointer user_data) {}

/* ----------------- Search member button ----------------- */






void on_buttonfiltrer_clicked (GtkWidget *objet_graphique, gpointer user_data) {}










