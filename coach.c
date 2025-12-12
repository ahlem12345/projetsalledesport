#include "coach.h"
#include <stdio.h>
#include <string.h>

int ajouter_dans_fichier_coach(coach c) {
    // COACH_FILE est défini dans coach.h
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

void afficher_coachs(GtkWidget *treeview) {
    printf("🟢 DEBUT afficher_coachs()\n");
    
    if (!treeview || !GTK_IS_TREE_VIEW(treeview)) {
        printf("❌ TreeView invalide\n");
        return;
    }
    
    // Nettoyer les colonnes existantes
    GtkTreeViewColumn *column;
    while ((column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0)) != NULL) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), column);
    }
    
    // Créer le store avec 6 colonnes
    GtkListStore *store = gtk_list_store_new(6, 
        G_TYPE_INT,     // ID
        G_TYPE_STRING,  // Nom
        G_TYPE_STRING,  // Prénom
        G_TYPE_STRING,  // Spécialité
        G_TYPE_INT,     // Nombre de cours
        G_TYPE_STRING   // Coaching privé
    );
    
    printf("✅ Store créé\n");
    
    // Créer les colonnes
    column = gtk_tree_view_column_new_with_attributes("ID", 
        gtk_cell_renderer_text_new(), "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    printf("✅ Colonne ID créée\n");
    
    column = gtk_tree_view_column_new_with_attributes("Nom", 
        gtk_cell_renderer_text_new(), "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    printf("✅ Colonne Nom créée\n");
    
    column = gtk_tree_view_column_new_with_attributes("Prénom", 
        gtk_cell_renderer_text_new(), "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    printf("✅ Colonne Prénom créée\n");
    
    column = gtk_tree_view_column_new_with_attributes("Spécialité", 
        gtk_cell_renderer_text_new(), "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    printf("✅ Colonne Spécialité créée\n");
    
    column = gtk_tree_view_column_new_with_attributes("Cours", 
        gtk_cell_renderer_text_new(), "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    printf("✅ Colonne Cours créée\n");
    
    column = gtk_tree_view_column_new_with_attributes("Privé", 
        gtk_cell_renderer_text_new(), "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    printf("✅ Colonne Privé créée\n");
    
    // Lire le fichier
    FILE *f = fopen(COACH_FILE, "r");
    if (!f) {
        printf("📭 Fichier %s non trouvé ou vide\n", COACH_FILE);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
        printf("🟢 FIN afficher_coachs()\n");
        return;
    }
    
    printf("📂 Lecture du fichier: %s\n", COACH_FILE);
    char ligne[256];
    int count = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        // Supprimer le saut de ligne
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) == 0) continue;
        
        printf("📖 Ligne lue: %s\n", ligne);
        
        // Analyser la ligne avec strtok
        char *token;
        char *tokens[6];
        int i = 0;
        
        // Faire une copie pour strtok
        char ligne_copie[256];
        strcpy(ligne_copie, ligne);
        
        token = strtok(ligne_copie, ";");
        while (token != NULL && i < 6) {
            tokens[i++] = token;
            token = strtok(NULL, ";");
        }
        
        // Vérifier qu'on a bien 6 champs
        if (i == 6) {
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            
            gtk_list_store_set(store, &iter,
                0, atoi(tokens[2]),          // ID
                1, tokens[0],                // Nom
                2, tokens[1],                // Prénom
                3, tokens[3],                // Spécialité
                4, atoi(tokens[4]),          // Nombre de cours
                5, tokens[5],                // Coaching privé
                -1);
            
            count++;
            printf("✅ Coach %d ajouté: %s %s (ID: %d)\n", 
                   count, tokens[0], tokens[1], atoi(tokens[2]));
        } else {
            printf("⚠️ Ligne mal formatée ignorée: %s (champs: %d/6)\n", ligne, i);
        }
    }
    
    fclose(f);
    
    printf("✅ %d coachs chargés depuis le fichier\n", count);
    
    // Appliquer le modèle au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
    
    printf("✅ Modèle appliqué au TreeView\n");
    printf("🟢 FIN afficher_coachs()\n");
}
// Fonction pour charger les IDs des coachs dans une combobox
// Fonction pour charger les IDs des coachs dans une combobox
void charger_ids_dans_combo(GtkWidget *combo) {  // Changé de int à void
    printf("🔄 Chargement des IDs dans la combobox\n");
    
    if (!combo || !GTK_IS_COMBO_BOX(combo)) {
        printf("❌ ComboBox invalide\n");
        return;
    }
    
    // Créer un nouveau store
    GtkListStore *store = gtk_list_store_new(1, G_TYPE_INT);
    GtkTreeIter iter;
    
    // Lire le fichier pour récupérer les IDs
    FILE *f = fopen(COACH_FILE, "r");
    if (!f) {
        printf("📭 Fichier %s non trouvé\n", COACH_FILE);
        gtk_combo_box_set_model(GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
        g_object_unref(store);
        return;
    }
    
    char ligne[256];
    int count = 0;
    
    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = 0;
        
        if (strlen(ligne) == 0) continue;
        
        // Analyser la ligne pour récupérer l'ID
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
        
        if (i >= 3) {  // Au moins nom, prénom, id
            int id = atoi(tokens[2]);
            if (id > 0) {
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, 0, id, -1);
                count++;
                printf("✅ ID ajouté: %d\n", id);
            }
        }
    }
    
    fclose(f);
    
    printf("✅ %d IDs chargés dans la combobox\n", count);
    
    // Appliquer le modèle
    gtk_combo_box_set_model(GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
    g_object_unref(store);
    
    // Ajouter un renderer pour afficher les IDs
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_clear(GTK_CELL_LAYOUT(combo));
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), renderer, "text", 0, NULL);
    
    // Sélectionner le premier élément s'il y en a
    if (count > 0) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    }
}
// Fonction pour modifier un coach
int modifier_coach(int id, coach nouv) {
    printf("✏️ Modification du coach ID %d\n", id);
    
    int trouve = 0;
    FILE *f = fopen(COACH_FILE, "r");
    FILE *f2 = fopen("coachs_temp.txt", "w");
    
    if (f != NULL && f2 != NULL) {
        char ligne[256];
        
        while (fgets(ligne, sizeof(ligne), f)) {
            ligne[strcspn(ligne, "\n")] = 0;
            
            if (strlen(ligne) == 0) {
                fputs("\n", f2);
                continue;
            }
            
            // Copier la ligne pour analyse
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
                    // Écrire les nouvelles données
                    fprintf(f2, "%s;%s;%d;%s;%d;%s\n",
                            nouv.nom, nouv.prenom, nouv.id, 
                            nouv.specialite, nouv.nbr_cours, 
                            nouv.coaching_prive);
                    trouve = 1;
                    printf("✅ Coach ID %d modifié\n", id);
                } else {
                    // Réécrire l'ancienne ligne
                    fprintf(f2, "%s\n", ligne);
                }
            }
        }
        
        fclose(f);
        fclose(f2);
        
        // Remplacer l'ancien fichier par le nouveau
        remove(COACH_FILE);
        rename("coachs_temp.txt", COACH_FILE);
        
    } else {
        printf("❌ Erreur d'ouverture des fichiers\n");
        if (f) fclose(f);
        if (f2) fclose(f2);
    }
    
    return trouve;
}
