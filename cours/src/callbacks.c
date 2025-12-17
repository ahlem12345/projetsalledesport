#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
/*-----------------*/


#include "cours.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ------------------------- */
/* Ajouter un cours          */
/* ------------------------- */
void on_button_ajouter_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry_id ;
    GtkWidget *entry_nom ;   
    GtkWidget *entry_coach ; 
    GtkWidget *entry_jour   ;
    GtkWidget *entry_heure  ;
    GtkWidget *spin_places ;
    GtkWidget *treeview_cours;
    Cours c;
    
    entry_id = lookup_widget(objet_graphique, "entry_id");
    entry_nom = lookup_widget(objet_graphique, "entry_nom");
    entry_coach = lookup_widget(objet_graphique, "entry_coach");
    entry_jour = lookup_widget(objet_graphique, "entry_jour");
    entry_heure = lookup_widget(objet_graphique, "entry_heure");
    spin_places = lookup_widget(objet_graphique, "spin_places");
    treeview_cours = lookup_widget(objet_graphique, "treeview_cours");

    // Vérifier que tous les champs sont remplis
    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
    const gchar *nom_text = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const gchar *coach_text = gtk_entry_get_text(GTK_ENTRY(entry_coach));
    const gchar *jour_text = gtk_entry_get_text(GTK_ENTRY(entry_jour));
    const gchar *heure_text = gtk_entry_get_text(GTK_ENTRY(entry_heure));
    
    if (id_text == NULL || strlen(id_text) == 0 ||
        nom_text == NULL || strlen(nom_text) == 0 ||
        coach_text == NULL || strlen(coach_text) == 0 ||
        jour_text == NULL || strlen(jour_text) == 0 ||
        heure_text == NULL || strlen(heure_text) == 0) {
        g_print("Veuillez remplir tous les champs\n");
        return;
    }

    strcpy(c.id, id_text);
    strcpy(c.nom, nom_text);
    strcpy(c.coach, coach_text);
    strcpy(c.jour, jour_text);
    strcpy(c.heure, heure_text);
    c.places = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_places));
    
    ajouter_cours(c);
    g_print("Cours %s ajouté avec succès!\n", c.nom);
    
    // Rafraîchir l'affichage
    if (treeview_cours != NULL) {
        refresh_treeview(treeview_cours);
    }
    
    // Vider les champs après ajout
    gtk_entry_set_text(GTK_ENTRY(entry_id), "");
    gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
    gtk_entry_set_text(GTK_ENTRY(entry_coach), "");
    gtk_entry_set_text(GTK_ENTRY(entry_jour), "");
    gtk_entry_set_text(GTK_ENTRY(entry_heure), "");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_places), 1);

//code ajouter 

GtkWidget *combobox = lookup_widget(objet_graphique, "comboboxentry1");
if (combobox)
    remplir_combobox_cours(combobox);

}

/* Modifier un cours */
void on_button_modifier_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry_id ;
    GtkWidget *entry_nom ;   
    GtkWidget *entry_coach ; 
    GtkWidget *entry_jour   ;
    GtkWidget *entry_heure  ;
    GtkWidget *spin_places ;
    GtkWidget *treeview_cours;
    Cours c;
    
    /*entry_id = lookup_widget(objet_graphique, "entry_id_modifier");
    entry_nom = lookup_widget(objet_graphique, "entry_nom_modifier");
    entry_coach = lookup_widget(objet_graphique, "entry_coach_modifier");
    entry_jour = lookup_widget(objet_graphique, "entry_jour_modifier");
    entry_heure = lookup_widget(objet_graphique, "entry_heure_modifier");
    spin_places = lookup_widget(objet_graphique, "spin_places_modifier");
    treeview_cours = lookup_widget(objet_graphique, "treeview_cours");
*/
    entry_id = lookup_widget(objet_graphique, "entry_id");
    entry_nom = lookup_widget(objet_graphique, "entry_nom");
    entry_coach = lookup_widget(objet_graphique, "entry_coach");
    entry_jour = lookup_widget(objet_graphique, "entry_jour");
    entry_heure = lookup_widget(objet_graphique, "entry_heure");
    spin_places = lookup_widget(objet_graphique, "spin_places");
    treeview_cours = lookup_widget(objet_graphique, "treeview_cours");


    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
    if (id_text == NULL || strlen(id_text) == 0) {
        g_print("Veuillez entrer un ID de cours à modifier\n");
        return;
    }

    strcpy(c.id, id_text);
    strcpy(c.nom, gtk_entry_get_text(GTK_ENTRY(entry_nom)));
    strcpy(c.coach, gtk_entry_get_text(GTK_ENTRY(entry_coach)));
    strcpy(c.jour, gtk_entry_get_text(GTK_ENTRY(entry_jour)));
    strcpy(c.heure, gtk_entry_get_text(GTK_ENTRY(entry_heure)));
    c.places = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_places));
       
    modifier_cours(c);
    g_print("Cours %s modifié avec succès!\n", c.id);
    
    // Rafraîchir l'affichage
    if (treeview_cours != NULL) {
        refresh_treeview(treeview_cours);
    }
// Vider les champs après modifier
    gtk_entry_set_text(GTK_ENTRY(entry_id), "");
    gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
    gtk_entry_set_text(GTK_ENTRY(entry_coach), "");
    gtk_entry_set_text(GTK_ENTRY(entry_jour), "");
    gtk_entry_set_text(GTK_ENTRY(entry_heure), "");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_places), 1);

//code ajouter

GtkWidget *combobox = lookup_widget(objet_graphique, "comboboxentry1");
if (combobox)
    remplir_combobox_cours(combobox);

}

/* Supprimer un cours */
void on_button_supprimer_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry_id;
    GtkWidget *treeview_cours;
    char id[20];

    entry_id = lookup_widget(objet_graphique, "entry_id_supprimer");
    treeview_cours = lookup_widget(objet_graphique, "treeview_cours");

    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
    if (id_text == NULL || strlen(id_text) == 0) {
        g_print("Veuillez entrer un ID de cours à supprimer\n");
        return;
    }

    strncpy(id, id_text, sizeof(id)-1);
    id[sizeof(id)-1] = '\0';
    
    supprimer_cours(id);
    g_print("Cours %s supprimé avec succès!\n", id);
    
    // Rafraîchir l'affichage
    if (treeview_cours != NULL) {
        refresh_treeview(treeview_cours);
    }
    
    // Vider le champ après suppression
    gtk_entry_set_text(GTK_ENTRY(entry_id), "");

//code ajouter

GtkWidget *combobox = lookup_widget(objet_graphique, "comboboxentry1");
if (combobox)
    remplir_combobox_cours(combobox);

}

/* Rechercher un cours */
void on_button_rechercher_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry_id_rechercher;
    GtkWidget *label59;
    char id[20];
    char resultat[500];
    
    // Récupérer les widgets
    entry_id_rechercher = lookup_widget(objet_graphique, "entry_id_rechercher");
    label59 = lookup_widget(objet_graphique, "label59");
    
    // Récupérer l'ID à rechercher
    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id_rechercher));
    
    if (id_text == NULL || strlen(id_text) == 0) {
        gtk_label_set_text(GTK_LABEL(label59), "Veuillez entrer un ID à rechercher");
        return;
    }
    
    strncpy(id, id_text, sizeof(id)-1);
    id[sizeof(id)-1] = '\0';

    // Rechercher le cours
    Cours c = rechercher_cours(id);
    
    if (strcmp(c.id, "NOT_FOUND") == 0) {
        snprintf(resultat, sizeof(resultat), 
                "Cours introuvable !\nID recherché : %s", id);
        gtk_label_set_text(GTK_LABEL(label59), resultat);
    } else {
        snprintf(resultat, sizeof(resultat),
                "Cours trouvé :\n"
                "ID: %s\t"
                "Nom: %s\t"
                "Coach: %s\t"
                "Jour: %s\t"
                "Heure: %s\t"
                "Places: %d\t"
                "Inscrits: %d\t"
                "Places restantes: %d",
                c.id, c.nom, c.coach, c.jour, c.heure, 
                c.places, c.nb_inscrits, c.places - c.nb_inscrits);
        gtk_label_set_text(GTK_LABEL(label59), resultat);
    }
}


/* Inscrire un membre */


/*void on_button_inscrire_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry_id_inscrire = lookup_widget(objet_graphique, "entry_id_inscrire");
    GtkWidget *treeview_cours = lookup_widget(objet_graphique, "treeview_cours");
    char id[50];

    if (!entry_id_inscrire) return;

    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id_inscrire));
    if (!id_text || strlen(id_text) == 0) return;

    strncpy(id, id_text, sizeof(id)-1);
    id[sizeof(id)-1] = '\0';

    // Appeler la fonction d'inscription
    int resultat = inscrire_membre(id);
    if (resultat)
        g_print("Inscription réussie!\n");
    else
        g_print("Cours complet ou introuvable!\n");

    // Rafraîchir l'affichage
    if (treeview_cours)
        refresh_treeview(treeview_cours);

    gtk_entry_set_text(GTK_ENTRY(entry_id_inscrire), "");
}
*/

/* void on_button_inscrire_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry_id = lookup_widget(objet_graphique, "entry_id_inscrire");
    GtkWidget *combobox = lookup_widget(objet_graphique, "comboboxentry1");
    GtkWidget *treeview = lookup_widget(objet_graphique, "treeview_cours");

    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
    //const gchar *nom_combo = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox));
	gchar *nom_combo = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox));

    if (!id_text || strlen(id_text) == 0 || !nom_combo) {
        g_print("Veuillez saisir l'ID et choisir un cours\n");
        return;
    }

    // Rechercher le cours par ID
    Cours c = rechercher_cours((char *)id_text);

    if (strcmp(c.id, "NOT_FOUND") == 0) {
        g_print("Cours introuvable\n");
        return;
    }

    // Vérifier que le NOM correspond au ComboBox
    if (strcmp(c.nom, nom_combo) != 0) {
        g_print("L'ID ne correspond pas au cours sélectionné\n");
        return;
    }

    // Inscription
    if (inscrire_membre(c.id))
        g_print("Inscription réussie au cours %s\n", c.nom);
    else
        g_print("Cours complet\n");

    if (treeview)
        refresh_treeview(treeview);

    gtk_entry_set_text(GTK_ENTRY(entry_id), "");

 /* 🔹 LIBÉRATION MÉMOIRE ICI 🔹 */
   /* g_free(nom_combo);
}*/

/*void on_button_inscrire_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry_id   = lookup_widget(objet_graphique, "entry_id_inscrire");
    GtkWidget *combobox   = lookup_widget(objet_graphique, "comboboxentry1");
    GtkWidget *spin       = lookup_widget(objet_graphique, "spin_inscription");
    GtkWidget *treeview   = lookup_widget(objet_graphique, "treeview_cours");

    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id)); */

    /* GTK2 : récupérer le texte du ComboBoxEntry */
  /*  GtkWidget *entry_combo = gtk_bin_get_child(GTK_BIN(combobox));
    const gchar *nom_combo = gtk_entry_get_text(GTK_ENTRY(entry_combo));

    int nb = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin));

    if (!id_text || strlen(id_text) == 0 ||
        !nom_combo || strlen(nom_combo) == 0 ||
        nb <= 0)
    {
        g_print("Veuillez saisir l'ID, choisir un cours et un nombre valide\n");
        return;
    } */

    /* Vérifier le cours */
   /* Cours c = rechercher_cours((char *)id_text);
    if (strcmp(c.id, "NOT_FOUND") == 0) {
        g_print("Cours introuvable\n");
        return;
    }
*/
    /* Vérifier correspondance ID / Nom */
  /*  if (strcmp(c.nom, nom_combo) != 0) {
        g_print("L'ID ne correspond pas au cours sélectionné\n");
        return;
    } */

    /* INSCRIPTIONS MULTIPLES */
  /*  int i, success = 0;
    for (i = 0; i < nb; i++) {
        if (inscrire_membre(c.id))
            success++;
        else
            break;   // cours plein
    }

    if (success > 0) {
        g_print("%d inscription(s) ajoutée(s)\n", success);

        if (treeview)
            refresh_treeview(treeview);

        if (combobox)
            remplir_combobox_cours(combobox);
    } else {
        g_print("Cours complet\n");
    }

    gtk_entry_set_text(GTK_ENTRY(entry_id), "");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), 1);
}
*/

void on_button_inscrire_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *radio_id     = lookup_widget(objet_graphique, "radio_par_id");
    GtkWidget *radio_cours  = lookup_widget(objet_graphique, "radio_par_cours");

    GtkWidget *entry_id     = lookup_widget(objet_graphique, "entry_id_inscrire");
    GtkWidget *combobox    = lookup_widget(objet_graphique, "comboboxentry1");
    GtkWidget *spin        = lookup_widget(objet_graphique, "spin_inscription");
    GtkWidget *treeview    = lookup_widget(objet_graphique, "treeview_cours");

    int nb = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin));
    if (nb <= 0) nb = 1;

    Cours c;
    int i, success = 0;

    /* MODE 1 : PAR ID           */

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_id)))
    {
        const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
        if (!id_text || strlen(id_text) == 0) {
            g_print("Veuillez saisir un ID\n");
            return;
        }

        c = rechercher_cours((char *)id_text);
        if (strcmp(c.id, "NOT_FOUND") == 0) {
            g_print("Cours introuvable\n");
            return;
        }
    }

    /* MODE 2 : PAR COURS        */

    else
    {
        GtkWidget *entry_combo = gtk_bin_get_child(GTK_BIN(combobox));
        const gchar *nom_combo = gtk_entry_get_text(GTK_ENTRY(entry_combo));

        if (!nom_combo || strlen(nom_combo) == 0) {
            g_print("Veuillez choisir un cours\n");
            return;
        }

        /* Rechercher par nom */
        FILE *f = fopen("cours.txt", "r");
        if (!f) return;

        strcpy(c.id, "NOT_FOUND");

        while (fscanf(f, "%s %s %s %s %s %d %d",
                       c.id, c.nom, c.coach, c.jour, c.heure,
                       &c.places, &c.nb_inscrits) != EOF)
        {
            if (strcmp(c.nom, nom_combo) == 0)
                break;
        }
        fclose(f);

        if (strcmp(c.id, "NOT_FOUND") == 0) {
            g_print("Cours introuvable\n");
            return;
        }
    }

    /* INSCRIPTION MULTIPLE      */

    for (i = 0; i < nb; i++) {
        if (inscrire_membre(c.id))
            success++;
        else
            break;
    }

    if (success > 0) {
        g_print("%d inscription(s) ajoutée(s)\n", success);
        refresh_treeview(treeview);
        remplir_combobox_cours(combobox);
    } else {
        g_print("Cours complet\n");
    }

    gtk_entry_set_text(GTK_ENTRY(entry_id), "");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), 1);
}


/* desinscrire un membre */
/*
void on_button_desinscrire_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry_id_inscrire = lookup_widget(objet_graphique, "entry_id_inscrire");
    GtkWidget *treeview_cours = lookup_widget(objet_graphique, "treeview_cours");
    char id[50];

    if (!entry_id_inscrire) return;

    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id_inscrire));
    if (!id_text || strlen(id_text) == 0) return;

    strncpy(id, id_text, sizeof(id)-1);
    id[sizeof(id)-1] = '\0';

    // Appeler la fonction d'inscription
    int resultat = desinscrire_membre(id);
    if (resultat)
        g_print("desinscription réussie!\n");
    else
        g_print("Déja 0!\n");

    // Rafraîchir l'affichage
    if (treeview_cours)
        refresh_treeview(treeview_cours);

    gtk_entry_set_text(GTK_ENTRY(entry_id_inscrire), "");
}
*/

void on_button_desinscrire_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *radio_id    = lookup_widget(objet_graphique, "radio_par_id");
    GtkWidget *radio_cours = lookup_widget(objet_graphique, "radio_par_cours");

    GtkWidget *entry_id  = lookup_widget(objet_graphique, "entry_id_inscrire");
    GtkWidget *combobox = lookup_widget(objet_graphique, "comboboxentry1");
    GtkWidget *spin     = lookup_widget(objet_graphique, "spin_inscription");
    GtkWidget *treeview = lookup_widget(objet_graphique, "treeview_cours");

    int nb = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin));
    if (nb <= 0) nb = 1;

    Cours c;
    int i, success = 0;

    /* MODE 1 : PAR ID           */

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_id)))
    {
        const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
        if (!id_text || strlen(id_text) == 0) {
            g_print("Veuillez saisir un ID\n");
            return;
        }

        c = rechercher_cours((char *)id_text);
        if (strcmp(c.id, "NOT_FOUND") == 0) {
            g_print("Cours introuvable\n");
            return;
        }
    }

    /* MODE 2 : PAR COURS        */
  
    else
    {
        GtkWidget *entry_combo = gtk_bin_get_child(GTK_BIN(combobox));
        const gchar *nom_combo = gtk_entry_get_text(GTK_ENTRY(entry_combo));

        if (!nom_combo || strlen(nom_combo) == 0) {
            g_print("Veuillez choisir un cours\n");
            return;
        }

        FILE *f = fopen("cours.txt", "r");
        if (!f) return;

        strcpy(c.id, "NOT_FOUND");

        while (fscanf(f, "%s %s %s %s %s %d %d",
                       c.id, c.nom, c.coach, c.jour, c.heure,
                       &c.places, &c.nb_inscrits) != EOF)
        {
            if (strcmp(c.nom, nom_combo) == 0)
                break;
        }
        fclose(f);

        if (strcmp(c.id, "NOT_FOUND") == 0) {
            g_print("Cours introuvable\n");
            return;
        }
    }

    /* DÉSINSCRIPTION MULTIPLE   */

    for (i = 0; i < nb; i++) {
        if (desinscrire_membre(c.id))
            success++;
        else
            break;   // déjà à 0
    }

    if (success > 0) {
        g_print("%d désinscription(s) effectuée(s)\n", success);
        refresh_treeview(treeview);
        remplir_combobox_cours(combobox);
    } else {
        g_print("Aucun inscrit à retirer\n");
    }

    gtk_entry_set_text(GTK_ENTRY(entry_id), "");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), 1);
}


/* Statistiques */
void on_button_calculer_clicked(GtkWidget *objet_graphique, gpointer user_data)
{
    GtkWidget *entry_nombre_total;
    GtkWidget *entry_cours_actif;
    
    entry_nombre_total = lookup_widget(objet_graphique, "entry_nombre_total");
    entry_cours_actif = lookup_widget(objet_graphique, "entry_cours_actif");

    // Calculer le nombre total de cours
    int total = nombre_total_cours();
    char total_str[20];
    snprintf(total_str, sizeof(total_str), "%d", total);
    gtk_entry_set_text(GTK_ENTRY(entry_nombre_total), total_str);

    // Trouver le cours le plus actif
    Cours plus_actif = cours_le_plus_actif();
    if (strcmp(plus_actif.id, "NOT_FOUND") == 0) {
        gtk_entry_set_text(GTK_ENTRY(entry_cours_actif), "Aucun cours");
    } else {
        char actif_str[100];
        snprintf(actif_str, sizeof(actif_str), "%s (%d inscrits)", 
                plus_actif.nom, plus_actif.nb_inscrits);
        gtk_entry_set_text(GTK_ENTRY(entry_cours_actif), actif_str);
    }
    
    g_print("Statistiques calculées : %d cours au total\n", total);
}

/* Fonction pour initialiser l'affichage des cours */
/*
void on_window_show(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *treeview_cours;
    
    treeview_cours = lookup_widget(widget, "treeview_cours");
    if (treeview_cours != NULL) {
        load_cours_into_treeview(treeview_cours);
    }
}
*/

void on_window_show(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *treeview_cours;
    GtkWidget *combobox;

    treeview_cours = lookup_widget(widget, "treeview_cours");
    combobox = lookup_widget(widget, "comboboxentry1");

    if (treeview_cours)
        load_cours_into_treeview(treeview_cours);

    if (combobox)
        remplir_combobox_cours(combobox);
}

/* Fonction pour rafraîchir l'affichage */
void refresh_treeview(GtkWidget *treeview)
{
    GtkTreeModel *model;
    
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    if (model != NULL) {
        gtk_list_store_clear(GTK_LIST_STORE(model));
    }
    
    load_cours_into_treeview(treeview);
}

/* Charger les cours dans le TreeView */
void load_cours_into_treeview(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeIter iter;

    char line[256];
    char id[100], nom[100], coach[100], jours[100], heure[100];
    int nombre_places, nb_inscrits;

    // Vérifier si le treeview existe
    if (treeview == NULL) {
        g_print("Erreur : TreeView non trouvé\n");
        return;
    }

    // Créer le store avec les colonnes (ajout de nb_inscrits)
    store = gtk_list_store_new(7,
                               G_TYPE_STRING,  // id
                               G_TYPE_STRING,  // nom
                               G_TYPE_STRING,  // coach
                               G_TYPE_STRING,  // jours
                               G_TYPE_STRING,  // heure
                               G_TYPE_INT,     // nombre_places
                               G_TYPE_INT);    // nb_inscrits

    // Définir le modèle
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    
    // Nettoyer les colonnes existantes
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    GList *col;
    for (col = columns; col != NULL; col = col->next) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_COLUMN(col->data));
    }
    g_list_free(columns);

    // Créer les colonnes
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Colonne ID
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Colonne Nom
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Colonne Coach
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Coach", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Colonne Jour
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Jour", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Colonne Heure
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Heure", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Colonne Places
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Places", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Colonne Inscrits
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Inscrits", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Lecture du fichier
    FILE *f = fopen("cours.txt", "r");
    if (!f) {
        g_print("Aucun cours trouvé. Le fichier sera créé lors du premier ajout.\n");
        g_object_unref(store);
        return;
    }

    while (fgets(line, sizeof(line), f)) {
        // Nettoyer le saut de ligne
        line[strcspn(line, "\n")] = 0;
        
        // Lire avec le nouveau format incluant nb_inscrits
        if (sscanf(line, "%s %s %s %s %s %d %d",
                   id, nom, coach, jours, heure,
                   &nombre_places, &nb_inscrits) == 7) {

            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               0, id,
                               1, nom,
                               2, coach,
                               3, jours,
                               4, heure,
                               5, nombre_places,
                               6, nb_inscrits,
                               -1);
        }
    }

    fclose(f);
    g_object_unref(store);
}

/*void remplir_combobox_cours(GtkWidget *combobox)
{
    FILE *f = fopen("cours.txt", "r");
    if (!f) return;

    char id[50], nom[50], coach[50], jour[50], heure[50];
    int places, inscrits;

    /* Créer le modèle pour le ComboBox (GTK2) */
 /*   GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
    GtkTreeIter iter;

    while (fscanf(f, "%s %s %s %s %s %d %d",
                  id, nom, coach, jour, heure, &places, &inscrits) != EOF)
    {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, nom,
                           -1);
    }

    fclose(f);

    /* Associer le modèle au ComboBox */
  /*  gtk_combo_box_set_model(GTK_COMBO_BOX(combobox), GTK_TREE_MODEL(store));

    /* Définir le renderer texte */
 /*   GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_clear(GTK_CELL_LAYOUT(combobox));
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox),
                                   renderer, "text", 0, NULL);

    g_object_unref(store);
}*/

void remplir_combobox_cours(GtkWidget *combobox)
{
    FILE *f = fopen("cours.txt", "r");
    if (!f) return;

    char id[50], nom[50], coach[50], jour[50], heure[50];
    int places, inscrits;

    GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
    GtkTreeIter iter;

    while (fscanf(f, "%s %s %s %s %s %d %d",
                  id, nom, coach, jour, heure, &places, &inscrits) != EOF)
    {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, nom, -1);
    }

    fclose(f);

    gtk_combo_box_set_model(GTK_COMBO_BOX(combobox), GTK_TREE_MODEL(store));

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_clear(GTK_CELL_LAYOUT(combobox));
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox),
                                   renderer, "text", 0, NULL);

    g_object_unref(store);
}
