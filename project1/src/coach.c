#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include "coach.h"
#include "support.h"

GtkWidget* lookup_widget(GtkWidget *widget, const gchar *widget_name);
int ajouter_dans_fichier_coach(coach c) {

    FILE *f = fopen(COACH_FILE, "a");
    
    if (f == NULL) {
        printf("Erreur: Impossible d'ouvrir %s\n", COACH_FILE);
        return 0;
    }
    
    fprintf(f, "%s;%s;%d;%s;%d;%s\n",
            c.nom, c.prenom, c.id, c.specialite,
            c.nbr_cours, c.coaching_prive);
    
    fclose(f);
    return 1;
}
void nettoyer_treeview(GtkWidget *treeview) {
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) {
        return;
    }
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    GList *col;
    for (col = columns; col != NULL; col = col->next) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_COLUMN(col->data));
    }
    if (columns) {
        g_list_free(columns);
    }
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), NULL);
}

void afficher_coachs(GtkWidget *treeview) {
    printf(" DEBUT afficher_coachs()\n");
    
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) {
        printf(" TreeView invalide\n");
        return;
    }
    nettoyer_treeview(treeview);
    GtkListStore *store = gtk_list_store_new(6, 
        G_TYPE_INT,     
        G_TYPE_STRING,  
        G_TYPE_STRING,  
        G_TYPE_STRING,  
        G_TYPE_INT,     
        G_TYPE_STRING   
    );
    
    printf("✅ Store créé\n");
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Prénom", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Spécialité", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Cours", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Privé", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    FILE *f = fopen(COACH_FILE, "r");
    if (!f) {
        printf(" Fichier %s non trouvé ou vide\n", COACH_FILE);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
        printf(" FIN afficher_coachs()\n");
        return;
    }
    
    printf(" Lecture du fichier: %s\n", COACH_FILE);
    char ligne[256];
    int count = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) == 0) continue;
        
        printf(" Ligne lue: %s\n", ligne);
        char *token;
        char *tokens[6];
        int i = 0;
        char ligne_copie[256];
        strcpy(ligne_copie, ligne);
        
        token = strtok(ligne_copie, ";");
        while (token != NULL && i < 6) {
            tokens[i++] = token;
            token = strtok(NULL, ";");
        }
        if (i == 6) {
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            
            gtk_list_store_set(store, &iter,
                0, atoi(tokens[2]),          
                1, tokens[0],                
                2, tokens[1],                
                3, tokens[3],                
                4, atoi(tokens[4]),          
                5, tokens[5],                
                -1);
            
            count++;
            printf(" Coach %d ajouté: %s %s (ID: %d)\n", 
                   count, tokens[0], tokens[1], atoi(tokens[2]));
        } else {
            printf(" Ligne mal formatée ignorée: %s (champs: %d/6)\n", ligne, i);
        }
    }
    
    fclose(f);
    
    printf(" %d coachs chargés depuis le fichier\n", count);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
    
    printf(" Modèle appliqué au TreeView\n");
    printf(" FIN afficher_coachs()\n");
}
void charger_ids_dans_combo(GtkWidget *combo) {
    printf(" Chargement des IDs dans la combobox\n");
    
    if (!combo || !GTK_IS_COMBO_BOX(combo)) {
        printf(" ComboBox invalide\n");
        return;
    }
    GtkListStore *store = gtk_list_store_new(1, G_TYPE_INT);
    GtkTreeIter iter;
    FILE *f = fopen(COACH_FILE, "r");
    if (!f) {
        printf(" Fichier %s non trouvé\n", COACH_FILE);
        gtk_combo_box_set_model(GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
        g_object_unref(store);
        return;
    }
    
    char ligne[256];
    int count = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) == 0) continue;
        char *token;
        char *tokens[6];
        int i = 0;
        
        char ligne_copie[256];
        strcpy(ligne_copie, ligne);
        
        token = strtok(ligne_copie, ";");
        while (token != NULL && i < 6) {
            tokens[i++] = token;
            token = strtok(NULL, ";");
        }
        
        if (i >= 3) {
            int id = atoi(tokens[2]);
            if (id > 0) {
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, 0, id, -1);
                count++;
                printf(" ID ajouté: %d\n", id);
            }
        }
    }
    
    fclose(f);
    
    printf(" %d IDs chargés dans la combobox\n", count);
    gtk_combo_box_set_model(GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
    g_object_unref(store);
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_clear(GTK_CELL_LAYOUT(combo));
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), renderer, "text", 0, NULL);
    if (count > 0) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    }
}
int modifier_coach(int id, coach nouv) {
    printf(" DEBUT modifier_coach() - ID: %d\n", id);
    if (nouv.id != id) {
        printf(" ERREUR: ID mismatch - recherché: %d, fourni: %d\n", id, nouv.id);
        return 0;
    }
    
    printf("Nouvelles données:\n");
    printf("  Nom: %s\n", nouv.nom);
    printf("  Prénom: %s\n", nouv.prenom);
    printf("  ID: %d\n", nouv.id);
    printf("  Spécialité: %s\n", nouv.specialite);
    printf("  Cours: %d\n", nouv.nbr_cours);
    printf("  Privé: %s\n", nouv.coaching_prive);
    
    int trouve = 0;
    FILE *f = fopen(COACH_FILE, "r");
    if (!f) {
        printf(" Impossible d'ouvrir %s en lecture\n", COACH_FILE);
        return 0;
    }
    
    FILE *f2 = fopen("coach_temp.txt", "w");
    if (!f2) {
        printf(" Impossible de créer le fichier temporaire\n");
        fclose(f);
        return 0;
    }
    
    char ligne[256];
    int ligne_num = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne_num++;
        ligne[strcspn(ligne, "\n")] = 0;  
        
        if (strlen(ligne) == 0) {
            fprintf(f2, "\n");
            continue;
        }
        char ligne_copie[256];
        strcpy(ligne_copie, ligne);
        char *nom = strtok(ligne_copie, ";");
        char *prenom = strtok(NULL, ";");
        char *id_str = strtok(NULL, ";");
        char *specialite = strtok(NULL, ";");
        char *cours_str = strtok(NULL, ";");
        char *coaching_prive = strtok(NULL, ";");
        
        if (nom && prenom && id_str) {
            int id_ligne = atoi(id_str);
            
            if (id_ligne == id) {
                fprintf(f2, "%s;%s;%d;%s;%d;%s\n",
                        nouv.nom, nouv.prenom, nouv.id, 
                        nouv.specialite, nouv.nbr_cours, 
                        nouv.coaching_prive);
                trouve = 1;
                printf(" Ligne %d - Coach ID %d MODIFIÉ\n", ligne_num, id);
                printf("   Ancien: %s\n", ligne);
                printf("   Nouveau: %s;%s;%d;%s;%d;%s\n",
                       nouv.nom, nouv.prenom, nouv.id, 
                       nouv.specialite, nouv.nbr_cours, nouv.coaching_prive);
            } else {
                fprintf(f2, "%s\n", ligne);
                printf(" Ligne %d - Coach ID %d CONSERVÉ\n", ligne_num, id_ligne);
            }
        } else {
            fprintf(f2, "%s\n", ligne);
            printf(" Ligne %d mal formatée, conservée\n", ligne_num);
        }
    }
    
    fclose(f);
    fclose(f2);
    
    if (trouve) {
        if (remove(COACH_FILE) == 0) {
            printf(" Ancien fichier supprimé\n");
        } else {
            printf(" Impossible de supprimer l'ancien fichier\n");
        }
        
        if (rename("coach_temp.txt", COACH_FILE) == 0) {
            printf(" Fichier remplacé avec succès\n");
        } else {
            printf(" Impossible de renommer le fichier temporaire\n");
            return 0;
        }
        
        printf(" FIN modifier_coach() - SUCCÈS\n");
        return 1;
    } else {
        remove("coach_temp.txt");
        printf(" FIN modifier_coach() - Coach ID %d NON TROUVÉ\n", id);
        return 0;
    }
}

void afficher_contenu_fichier() {
    printf("\n CONTENU DU FICHIER %s:\n", COACH_FILE);
 
    
    FILE *f = fopen(COACH_FILE, "r");
    if (!f) {
        printf(" Impossible d'ouvrir le fichier\n");
        return;
    }
    
    char ligne[256];
    int ligne_num = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne_num++;
        ligne[strcspn(ligne, "\n")] = 0;
        printf("Ligne %d: %s\n", ligne_num, ligne);
    }
    
    fclose(f);

}
int supprimer_coach(char *filename, int id) {
    printf(" DEBUT supprimer_coach() - ID: %d, Fichier: %s\n", id, filename);
    
    int trouve = 0;
    char ligne[256];
    
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf(" Impossible d'ouvrir %s en lecture\n", filename);
        return 0;
    }
    
    FILE *f2 = fopen("coach_temp_supp.txt", "w");
    if (!f2) {
        printf(" Impossible de créer le fichier temporaire\n");
        fclose(f);
        return 0;
    }
    
    int ligne_num = 0;
    int lignes_conservees = 0;
    int lignes_supprimees = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne_num++;
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) == 0) {
            fprintf(f2, "\n");
            continue;
        }
        char ligne_copie[256];
        strcpy(ligne_copie, ligne);
        
        char *nom = strtok(ligne_copie, ";");
        char *prenom = strtok(NULL, ";");
        char *id_str = strtok(NULL, ";");
        char *specialite = strtok(NULL, ";");
        char *cours_str = strtok(NULL, ";");
        char *coaching_prive = strtok(NULL, ";");
        
        if (nom && prenom && id_str) {
            int id_ligne = atoi(id_str);
            
            if (id_ligne == id) {
                trouve = 1;
                lignes_supprimees++;
                printf(" Ligne %d SUPPRIMÉE - Coach ID %d\n", ligne_num, id);
                printf("   Données supprimées: %s\n", ligne);
            } else {
                fprintf(f2, "%s\n", ligne);
                lignes_conservees++;
            }
        } else {
            fprintf(f2, "%s\n", ligne);
            lignes_conservees++;
        }
    }
    
    fclose(f);
    fclose(f2);
    
    if (trouve) {
        if (remove(filename) == 0) {
            printf(" Ancien fichier supprimé\n");
        }
        
        if (rename("coach_temp_supp.txt", filename) == 0) {
            printf(" Fichier mis à jour\n");
            printf(" Statistiques: %d lignes conservées, %d ligne(s) supprimée(s)\n", 
                   lignes_conservees, lignes_supprimees);
        } else {
            printf(" Impossible de renommer le fichier temporaire\n");
            remove("coach_temp_supp.txt");
            return 0;
        }
        
        printf(" FIN supprimer_coach() - SUCCÈS\n");
        return 1;
    } else {
        remove("coach_temp_supp.txt");
        printf(" FIN supprimer_coach() - Coach ID %d NON TROUVÉ\n", id);
        return 0;
    }
}
void enregistrer_suppression_log(int id, const char *nom, const char *prenom, 
                                 const char *raison, const char *commentaire) {
    FILE *log = fopen("suppressions_log.txt", "a");
    if (!log) {
        printf(" Impossible de créer le fichier log\n");
        return;
    }
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date_heure[100];
    strftime(date_heure, sizeof(date_heure), "%Y-%m-%d %H:%M:%S", tm_info);
    
    fprintf(log, "=== SUPPRESSION COACH ===\n");
    fprintf(log, "Date/Heure: %s\n", date_heure);
    fprintf(log, "ID: %d\n", id);
    fprintf(log, "Nom: %s\n", nom);
    fprintf(log, "Prénom: %s\n", prenom);
    fprintf(log, "Raison: %s\n", raison);
    fprintf(log, "Commentaire: %s\n", commentaire);
    fprintf(log, "=========================\n\n");
    
    fclose(log);
    printf(" Suppression enregistrée dans suppressions_log.txt\n");
}
void rechercher_coachs(const char *specialite, const char *coaching_prive, GtkWidget *treeview) {
    printf(" DEBUT rechercher_coachs()\n");
    printf("  Spécialité recherchée: '%s'\n", specialite ? specialite : "(toutes)");
    printf("  Coaching privé: '%s'\n", coaching_prive ? coaching_prive : "(tous)");
    
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) {
        printf(" TreeView invalide\n");
        return;
    }
    nettoyer_treeview(treeview);
    GtkListStore *store = gtk_list_store_new(6, 
        G_TYPE_INT,  
        G_TYPE_STRING,
        G_TYPE_STRING, 
        G_TYPE_STRING, 
        G_TYPE_INT,   
        G_TYPE_STRING  
    );
    
    printf("✅ Store créé pour recherche\n");
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Prénom", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Spécialité", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Cours", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Privé", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    FILE *f = fopen(COACH_FILE, "r");
    if (!f) {
        printf("📭 Fichier %s non trouvé\n", COACH_FILE);
        printf(" Ajout de données de test\n");
        GtkTreeIter iter;
        
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, 99,
            1, "Test",
            2, "Coach Yoga",
            3, "Yoga",
            4, 10,
            5, "oui",
            -1);
            
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
        printf(" FIN rechercher_coachs() - Fichier vide\n");
        return;
    }
    
    printf(" Lecture du fichier pour recherche\n");
    char ligne[256];
    int count = 0;
    int total = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) == 0) continue;
        
        total++;

        char *token;
        char *tokens[6];
        int i = 0;
        
        char ligne_copie[256];
        strcpy(ligne_copie, ligne);
        
        token = strtok(ligne_copie, ";");
        while (token != NULL && i < 6) {
            tokens[i++] = token;
            token = strtok(NULL, ";");
        }
        
        if (i == 6) {
            printf("  Coach trouvé: %s %s - Spécialité: '%s' - Privé: '%s'\n",
                   tokens[0], tokens[1], tokens[3], tokens[5]);
            

            int correspond = 1;
            
            if (specialite && strlen(specialite) > 0 && strcasecmp(specialite, "Toutes") != 0) {
                if (strcasecmp(tokens[3], specialite) != 0) {
                    correspond = 0;
                    printf("     Ne correspond pas à la spécialité '%s'\n", specialite);
                }
            }
            
            // Critère 2: Coaching privé (si spécifié)
            if (correspond && coaching_prive && strlen(coaching_prive) > 0 && strcasecmp(coaching_prive, "Tous") != 0) {
                if (strcasecmp(tokens[5], coaching_prive) != 0) {
                    correspond = 0;
                    printf(" Ne correspond pas au coaching privé '%s'\n", coaching_prive);
                }
            }
            if (correspond) {
                GtkTreeIter iter;
                gtk_list_store_append(store, &iter);
                
                gtk_list_store_set(store, &iter,
                    0, atoi(tokens[2]),        
                    1, tokens[0],               
                    2, tokens[1],              
                    3, tokens[3],                
                    4, atoi(tokens[4]),          
                    5, tokens[5],               
                    -1);
                
                count++;
                printf("    ✅ AJOUTÉ: %s %s (ID: %d)\n", 
                       tokens[0], tokens[1], atoi(tokens[2]));
            }
        } else {
            printf(" Ligne mal formatée: %s\n", ligne);
        }
    }
    
    fclose(f);
    
    printf(" Résultats recherche: %d/%d coachs trouvés\n", count, total);
    if (count == 0) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, 0,
            1, "Aucun résultat",
            2, "",
            3, "",
            4, 0,
            5, "",
            -1);
    }
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
    
    printf("✅ Modèle de recherche appliqué au TreeView\n");
    printf("🔍 FIN rechercher_coachs()\n");
}
void charger_materiels_dans_combo(GtkWidget *combo) {
    printf(" Chargement matériels dans combo\n");
    
    if (!combo || !GTK_IS_COMBO_BOX(combo)) {
        printf(" ComboBox invalide\n");
        return;
    }
    GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
    GtkTreeIter iter;
    FILE *f = fopen(MATERIEL_FILE, "r");
    if (!f) {
        printf(" Fichier %s non trouvé\n", MATERIEL_FILE);
        const char *materiels_exemple[] = {
            "Ballon de football", "Tapis de yoga", "Haltères 5kg", 
            "Corde à sauter", "Ballon médecine", "Barre de traction",
            "Banc de musculation", "Vélo d'appartement", NULL
        };
        
        for (int i = 0; materiels_exemple[i] != NULL; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, materiels_exemple[i], -1);
        }
        
    } else {
        char ligne[256];
        
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) == 0) continue;
            char *nom = strtok(ligne, ";");
            if (nom) {
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, 0, nom, -1);
                printf("✅ Matériel ajouté: %s\n", nom);
            }
        }
        
        fclose(f);
    }
    gtk_combo_box_set_model(GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
    g_object_unref(store);
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_clear(GTK_CELL_LAYOUT(combo));
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), renderer, "text", 0, NULL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    
    printf("✅ Matériels chargés dans la combobox\n");
}
void charger_salles_dans_combo(GtkWidget *combo) {
    printf(" Chargement salles dans combo\n");
    
    if (!combo || !GTK_IS_COMBO_BOX(combo)) {
        printf(" ComboBox invalide\n");
        return;
    }
    GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
    GtkTreeIter iter;
    FILE *f = fopen(SALLE_FILE, "r");
    if (!f) {
        printf(" Fichier %s non trouvé\n", SALLE_FILE);
        const char *salles_exemple[] = {
            "Salle A", "Salle B", "Salle C", 
            "Salle D", "Salle E", "Salle F", NULL
        };
        
        for (int i = 0; salles_exemple[i] != NULL; i++) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, salles_exemple[i], -1);
        }
        
    } else {
        char ligne[256];
        
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) == 0) continue;
            char *nom = strtok(ligne, ";");
            if (nom) {
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, 0, nom, -1);
                printf("✅ Salle ajoutée: %s\n", nom);
            }
        }
        
        fclose(f);
    }
    gtk_combo_box_set_model(GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
    g_object_unref(store);
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_clear(GTK_CELL_LAYOUT(combo));
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), renderer, "text", 0, NULL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    
    printf("✅ Salles chargées dans la combobox\n");
}
int verifier_disponibilite(const char *nom, const char *type) {
    printf("🔍 Vérification disponibilité: %s (type: %s)\n", nom, type);
    
    if (!nom || !type) {
        printf("❌ Paramètres invalides\n");
        return 0;
    }
    
    FILE *f = fopen("disponibilite.txt", "r");
    if (!f) {
        printf("❌ Fichier disponibilite.txt non trouvé\n");
        return 0;
    }
    
    int disponible = 0;
    char ligne[256];
    int trouve = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = 0;
        ligne[strcspn(ligne, "\r")] = 0;
        
        if (strlen(ligne) == 0) continue;
        
        printf("Ligne lue: %s\n", ligne);
        char ligne_copie[256];
        strcpy(ligne_copie, ligne);
        char *nom_element = strtok(ligne_copie, ";");
        char *type_element = strtok(NULL, ";");
        char *statut = strtok(NULL, ";");
        char *salle_associee = strtok(NULL, ";");
        
        if (nom_element && type_element && statut) {
            printf("  - Analyse: nom='%s', type='%s', statut='%s'\n", 
                   nom_element, type_element, statut);
            if (strcasecmp(nom_element, nom) == 0 && 
                strcasecmp(type_element, type) == 0) {
                
                trouve = 1;
                printf(" Élément trouvé! Statut: %s\n", statut);
                if (strcasecmp(statut, "dispo") == 0) {
                    disponible = 1;
                } else if (strcasecmp(statut, "disponible") == 0) {
                    disponible = 1;
                } else if (strcasecmp(statut, "indispo") == 0) {
                    disponible = 0;
                } else if (strcasecmp(statut, "indisponible") == 0) {
                    disponible = 0;
                } else {
                    disponible = 0;
                }
                break;
            }
        } else {
            printf("  Ligne mal formatée\n");
        }
    }
    
    fclose(f);
    
    if (!trouve) {
        printf(" Élément '%s' (type: %s) non trouvé dans le fichier\n", nom, type);
    } else {
        printf(" Résultat: %s est %s\n", 
               nom, disponible ? "DISPONIBLE " : "INDISPONIBLE ");
    }
    
    return disponible;
}
void afficher_materiels_dans_treeview(GtkWidget *treeview) {
    printf(" Affichage matériels dans TreeView\n");
    
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) {
        printf(" TreeView invalide\n");
        return;
    }
    nettoyer_treeview(treeview);
    GtkListStore *store = gtk_list_store_new(3, 
        G_TYPE_STRING,  
        G_TYPE_STRING, 
        G_TYPE_INT    
    );
    FILE *f = fopen(MATERIEL_FILE, "r");
    if (f) {
        char ligne[256];
        
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) == 0) continue;
            char *nom = strtok(ligne, ";");
            char *type = strtok(NULL, ";");
            char *quantite_str = strtok(NULL, ";");
            
            if (nom && type && quantite_str) {
                int quantite = atoi(quantite_str);
                
                GtkTreeIter iter;
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter,
                    0, nom,
                    1, type,
                    2, quantite,
                    -1);
            }
        }
        
        fclose(f);
    }
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Quantité", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
    
    printf("Matériels affichés dans TreeView\n");
}
void afficher_salles_dans_treeview(GtkWidget *treeview) {
    printf(" Affichage salles dans TreeView\n");
    
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) {
        printf(" TreeView invalide\n");
        return;
    }
    nettoyer_treeview(treeview);
    GtkListStore *store = gtk_list_store_new(3, 
        G_TYPE_STRING,  
        G_TYPE_STRING, 
        G_TYPE_INT      
    );
    FILE *f = fopen(SALLE_FILE, "r");
    if (f) {
        char ligne[256];
        
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) == 0) continue;
            char *nom = strtok(ligne, ";");
            char *type = strtok(NULL, ";");
            char *capacite_str = strtok(NULL, ";");
            
            if (nom && type && capacite_str) {
                int capacite = atoi(capacite_str);
                
                GtkTreeIter iter;
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter,
                    0, nom,
                    1, type,
                    2, capacite,
                    -1);
            }
        }
        
        fclose(f);
    }
    
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Capacité", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
    
    printf(" Salles affichées dans TreeView\n");
}

void ajouter_element_cours_treeview(const char *nom, const char *type, GtkWidget *treeview) {
    printf(" Ajout au TreeView: %s (%s)\n", nom, type);
    
    if (!treeview) {
        printf(" TreeView NULL\n");
        return;
    }
    
    printf("  - TreeView: %p, Type: %s\n", treeview, G_OBJECT_TYPE_NAME(treeview));
    if (!GTK_IS_TREE_VIEW(treeview)) {
        printf(" Ce n'est pas un GtkTreeView!\n");
        return;
    }
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    GtkListStore *store = NULL;
    
    if (!model) {
        printf("  - Création d'un nouveau ListStore...\n");
        store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        printf("  - Nouveau store créé\n");
    } else {
        printf("  - Store existant trouvé\n");
        store = GTK_LIST_STORE(model);
    }
    
    if (!store) {
        printf("❌ Impossible de créer/obtenir le store\n");
        return;
    }
    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, nom,    
                       1, type,   
                       -1);
    
    printf("✅ Élément ajouté: %s (%s)\n", nom, type);
    marquer_indisponible(nom, type);
    
    if (!model) {
        printf("  - Création des colonnes...\n");
        GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
        if (columns) {
            g_list_free(columns);
        }

        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Élément", renderer, "text", 0, NULL);
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 1, NULL);
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        printf("  - Colonnes créées\n");
    }
}

void afficher_message_erreur(GtkWidget *window, const char *message) {
    printf(" Affichage message d'erreur: %s\n", message);
    
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "%s", message);
    
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    printf("✅ Message d'erreur affiché\n");
}
void creer_fichier_par_defaut(const char *fichier, const char *type) {
    printf("📄 Création fichier par défaut: %s\n", fichier);
    
    FILE *fp = fopen(fichier, "w");
    if (!fp) {
        printf(" Impossible de créer %s\n", fichier);
        return;
    }
    
    if (strcmp(type, "materiel") == 0) {
        fprintf(fp, "Ballon de football;10;disponible\n");
        fprintf(fp, "Tapis de yoga;5;disponible\n");
        fprintf(fp, "Haltères 5kg;20;disponible\n");
        fprintf(fp, "Corde à sauter;15;disponible\n");
        fprintf(fp, "Ballon médecine;8;disponible\n");
        fprintf(fp, "Barre de traction;3;disponible\n");
        fprintf(fp, "Banc de musculation;2;disponible\n");
        fprintf(fp, "Vélo d'appartement;4;disponible\n");
        printf("✅ Fichier materiel.txt créé avec 8 matériels\n");
    } else if (strcmp(type, "salle") == 0) {
        fprintf(fp, "Salle A;100;disponible\n");
        fprintf(fp, "Salle B;80;disponible\n");
        fprintf(fp, "Salle C;50;disponible\n");
        fprintf(fp, "Salle D;120;disponible\n");
        fprintf(fp, "Salle E;60;disponible\n");
        printf("✅ Fichier salles.txt créé avec 5 salles\n");
    }
    
    fclose(fp);
}

void ajouter_element_base(const char *nom, const char *type, const char *statut, const char *quantite) {
    char *fichier = NULL;
    if (strcmp(type, "materiel") == 0) {
        fichier = "materiel.txt";
    } else if (strcmp(type, "salle") == 0) {
        fichier = "salles.txt";
    } else {
        return;
    }
    
    FILE *fp = fopen(fichier, "a");
    if (!fp) {
        printf(" Impossible d'ouvrir %s en mode ajout\n", fichier);
        return;
    }
    
    fprintf(fp, "%s;%s;%s\n", nom, quantite, statut);
    fclose(fp);
    
    printf(" %s ajouté à %s (Quantité: %s, Statut: %s)\n", nom, fichier, quantite, statut);
}



void marquer_indisponible(const char *nom, const char *type) {
    printf(" Marquage %s comme indisponible\n", nom);
    
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
        printf(" Fichier source non trouvé: %s\n", fichier_source);
        return;
    }
    
    FILE *temp = fopen(fichier_temp, "w");
    if (!temp) {
        printf(" Impossible de créer fichier temporaire\n");
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
                fprintf(temp, "%s;%s;indisponible\n", nom_fichier, quantite);
                modifie = 1;
                printf("  - %s marqué comme indisponible\n", nom);
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
        remove(fichier_source);
        rename(fichier_temp, fichier_source);
        printf(" Fichier %s mis à jour\n", fichier_source);
    } else {
        remove(fichier_temp);
        printf(" %s non trouvé dans %s\n", nom, fichier_source);
    }
}

void initialiser_combo01(GtkWidget *window) {
    printf(" initialiser_combo01 - DEBUT\n");
    
    if (!window) {
        printf(" Fenêtre NULL\n");
        return;
    }
    
    printf("  - Recherche combo01...\n");
    GtkWidget *combo01 = lookup_widget(window, "combo01");
    
    if (!combo01) {
        printf(" combo01 non trouvé\n");
        return;
    }
    
    printf("  - Type trouvé: %s\n", G_OBJECT_TYPE_NAME(combo01));
    if (!GTK_IS_COMBO(combo01)) {
        printf(" Ce n'est pas un GtkCombo!\n");
        return;
    }
    
    printf("  - C'est bien un GtkCombo, initialisation...\n");
    printf("  - Vérification fichier materiel.txt...\n");
    FILE *fp = fopen("materiel.txt", "r");
    if (!fp) {
        printf(" Fichier non trouvé, création...\n");
        creer_fichier_par_defaut("materiel.txt", "materiel");
        fp = fopen("materiel.txt", "r");
        if (!fp) {
            printf(" Impossible de créer/ouvrir le fichier\n");
            return;
        }
    }
    printf("  - Lecture du fichier...\n");
    GList *items = NULL;
    char ligne[200];
    int count = 0;
    
    while (fgets(ligne, sizeof(ligne), fp)) {
        size_t len = strlen(ligne);
        if (len > 0 && ligne[len-1] == '\n') {
            ligne[len-1] = '\0';
        }
        
        if (strlen(ligne) == 0) continue;
        
        printf("    - Ligne: %s\n", ligne);
        char *token = strtok(ligne, ";");
        if (token) {
            char *nom = token;
            token = strtok(NULL, ";");
            if (token) {
                char *quantite = token;
                token = strtok(NULL, ";");
                if (token) {
                    char *statut = token;
                    
                    printf("    - Nom: %s, Statut: %s\n", nom, statut);
                    
                    if (strcasecmp(statut, "disponible") == 0) {
                        items = g_list_append(items, g_strdup(nom));
                        count++;
                        printf("    - Ajouté à la liste: %s\n", nom);
                    }
                }
            }
        }
    }
    
    fclose(fp);
    
    printf("  - %d éléments disponibles trouvés\n", count);
    
    if (count > 0) {
        printf("  - Configuration du GtkCombo...\n");
        gtk_combo_set_popdown_strings(GTK_COMBO(combo01), items);
        
        if (items) {
            GtkWidget *entry = GTK_COMBO(combo01)->entry;
            if (entry) {
                gchar *first_item = (gchar*)items->data;
                gtk_entry_set_text(GTK_ENTRY(entry), first_item);
                printf("  - Texte défini: %s\n", first_item);
            }
        }
        
        g_list_free_full(items, g_free);
        
        printf(" combo01 initialisé avec succès\n");
    } else {
        printf("Aucun matériel disponible\n");
        if (items) {
            g_list_free_full(items, g_free);
        }
    }
    
    printf(" initialiser_combo01 - FIN\n");
}
void initialiser_combo02(GtkWidget *window) {
    printf("initialiser_combo02 - DEBUT\n");
    
    if (!window) {
        printf(" Fenêtre NULL\n");
        return;
    }
    
    printf("  - Recherche combo02...\n");
    GtkWidget *combo02 = lookup_widget(window, "combo02");
    
    if (!combo02) {
        printf("combo02 non trouvé\n");
        return;
    }
    
    printf("  - Type trouvé: %s\n", G_OBJECT_TYPE_NAME(combo02));
    
    // Vérifier si c'est bien un GtkCombo
    if (!GTK_IS_COMBO(combo02)) {
        printf("Ce n'est pas un GtkCombo!\n");
        return;
    }
    
    printf(" C'est bien un GtkCombo, initialisation...\n");
    
    // Vérifier si le fichier existe
    printf(" Vérification fichier salles.txt...\n");
    FILE *fp = fopen("salles.txt", "r");
    if (!fp) {
        printf("Fichier non trouvé, création...\n");
        creer_fichier_par_defaut("salles.txt", "salle");
        fp = fopen("salles.txt", "r");
        if (!fp) {
            printf(" Impossible de créer/ouvrir le fichier\n");
            return;
        }
    }
    
    // Lire le fichier
    printf("  - Lecture du fichier...\n");
    GList *items = NULL;
    char ligne[200];
    int count = 0;
    
    while (fgets(ligne, sizeof(ligne), fp)) {
        // Supprimer le saut de ligne
        size_t len = strlen(ligne);
        if (len > 0 && ligne[len-1] == '\n') {
            ligne[len-1] = '\0';
        }
        
        if (strlen(ligne) == 0) continue;
        
        printf("    - Ligne: %s\n", ligne);
        

        char *token = strtok(ligne, ";");
        if (token) {
            char *nom = token;
            token = strtok(NULL, ";");
            if (token) {
                char *capacite = token;
                token = strtok(NULL, ";");
                if (token) {
                    char *statut = token;
                    
                    printf("    - Nom: %s, Statut: %s\n", nom, statut);
                    
                    if (strcasecmp(statut, "disponible") == 0) {
                        items = g_list_append(items, g_strdup(nom));
                        count++;
                        printf("    - Ajouté à la liste: %s\n", nom);
                    }
                }
            }
        }
    }
    
    fclose(fp);
    
    printf("  - %d salles disponibles trouvées\n", count);
    
    if (count > 0) {
       
        printf("  - Configuration du GtkCombo...\n");
        gtk_combo_set_popdown_strings(GTK_COMBO(combo02), items);

        if (items) {
            GtkWidget *entry = GTK_COMBO(combo02)->entry;
            if (entry) {
                gchar *first_item = (gchar*)items->data;
                gtk_entry_set_text(GTK_ENTRY(entry), first_item);
                printf("  - Texte défini: %s\n", first_item);
            }
        }

        g_list_free_full(items, g_free);
        
        printf("combo02 initialisé avec succès\n");
    } else {
        printf(" Aucune salle disponible\n");
        if (items) {
            g_list_free_full(items, g_free);
        }
    }
    
    printf("initialiser_combo02 - FIN\n");
}
int sauvegarder_cours_fichier(cours c) {
    printf("Sauvegarde du cours: %s\n", c.nom);
    
    FILE *fp = fopen("cours.txt", "a");
    if (!fp) {
        printf("Impossible d'ouvrir cours.txt\n");
        return 0;
    }
    
    // Formater la ligne
    fprintf(fp, "%s;%s;%d;%s;%s\n", 
            c.nom, 
            c.type, 
            c.participants,
            c.materiels,
            c.salle);
    
    fclose(fp);
    
    printf(" Cours sauvegardé:\n");
    printf(" Nom: %s\n", c.nom);
    printf(" Type: %s\n", c.type);
    printf(" Participants: %d\n", c.participants);
    printf(" Matériels: %s\n", c.materiels);
    printf(" Salle: %s\n", c.salle);
    
    return 1;
}

