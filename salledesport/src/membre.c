#include "membre.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* ----------------- Ajouter membre ----------------- */
int ajouter_membre(membre *m, const char *filename)
{
    FILE *f = fopen(filename, "a");
    if (!f) return 0;

    fprintf(f, "%s;%s;%s;%s;%s;%02d/%02d/%04d;%02d/%02d/%04d;%.2f;%.2f;%.2f;%s;%s\n",
            m->id, m->nom, m->prenom, m->tel, m->email,
            m->jour_naissance, m->mois_naissance, m->annee_naissance,
            m->jour_inscription, m->mois_inscription, m->annee_inscription,
            m->longueur, m->poids, m->frais_paye,
            m->type_abonnement, m->duree_abonnement);

    fclose(f);
    return 1;
}

/* ----------------- Modifier membre ----------------- */
int modifier_membre(const char *id_recherche, membre *m, const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    FILE *tmp = fopen("tmp.txt", "w");
    if (!tmp) { fclose(f); return 0; }

    char ligne[1024];
    int modified = 0;

    while(fgets(ligne, sizeof(ligne), f)) {
        char copy[1024]; strncpy(copy, ligne, sizeof(copy)-1); copy[sizeof(copy)-1]='\0';
        copy[strcspn(copy, "\r\n")] = 0;
        char *fields[20]; int i=0;
        char *token = strtok(copy, ";");
        while(token && i<20) { fields[i++] = token; token=strtok(NULL,";"); }

        if(i<1) { fputs(ligne,tmp); continue; }

        if(strcmp(fields[0], id_recherche)==0) {
            fprintf(tmp, "%s;%s;%s;%s;%s;%02d/%02d/%04d;%02d/%02d/%04d;%.2f;%.2f;%.2f;%s;%s\n",
                    m->id, m->nom, m->prenom, m->tel, m->email,
                    m->jour_naissance, m->mois_naissance, m->annee_naissance,
                    m->jour_inscription, m->mois_inscription, m->annee_inscription,
                    m->longueur, m->poids, m->frais_paye,
                    m->type_abonnement, m->duree_abonnement);
            modified = 1;
        } else {
            fputs(ligne, tmp);
        }
    }

    fclose(f); fclose(tmp);

    if(modified) {
        remove(filename);
        rename("tmp.txt", filename);
    } else {
        remove("tmp.txt");
    }
    return modified;
}

/* ----------------- Supprimer membre ----------------- */
int supprimer_membre(const char *id_recherche, const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    FILE *tmp = fopen("tmp.txt", "w");
    if(!tmp){ fclose(f); return 0; }

    char ligne[1024];
    int deleted = 0;

    while(fgets(ligne, sizeof(ligne), f)) {
        char copy[1024]; strcpy(copy, ligne);
        copy[strcspn(copy,"\r\n")]=0;
        char *fields[20]; int i=0;
        char *token=strtok(copy,";");
        while(token && i<20){fields[i++]=token; token=strtok(NULL,";");}

        if(i<1){fputs(ligne,tmp); continue;}

        if(strcmp(fields[0],id_recherche)==0){ deleted=1; continue;}

        fputs(ligne,tmp);
    }

    fclose(f); fclose(tmp);

    if(deleted){ remove(filename); rename("tmp.txt",filename); }
    else remove("tmp.txt");

    return deleted;
}

/* ----------------- Charger membres dans TreeView ----------------- */
int charger_membres(GtkListStore *store, const char *filename)
{
    FILE *f = fopen(filename,"r");
    if(!f) return 0;

    gtk_list_store_clear(store);
    char ligne[512]; GtkTreeIter iter;

    while(fgets(ligne,sizeof(ligne),f)) {
        ligne[strcspn(ligne,"\n")]=0;
        char *data[12]; int i=0;
        char *token=strtok(ligne,";");
        while(token && i<12){data[i++]=token; token=strtok(NULL,";");}
        if(i==12){
            gtk_list_store_append(store,&iter);
            gtk_list_store_set(store,&iter,
                               0,data[0],1,data[1],2,data[2],3,data[3],4,data[4],
                               5,data[5],6,data[6],7,data[7],8,data[8],9,data[9],
                               10,data[10],11,data[11],-1);
        }
    }

    fclose(f);
    return 1;
}

/* ----------------- Statistiques ----------------- */
int calculer_statistiques(const char *filename, int *total, int *actifs, int *inactifs, float *pourcentage)
{
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    char line[512];
    *total = 0;
    *actifs = 0;

    time_t now = time(NULL);
    struct tm today = *localtime(&now);

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;

        char *fields[12];
        int i = 0;
        char *token = strtok(line, ";");
        while (token && i < 12) { fields[i++] = token; token = strtok(NULL, ";"); }
        if (i < 12) continue;

        (*total)++;

        int j_inscr, m_inscr, a_inscr, duree;
        if (sscanf(fields[6], "%d/%d/%d", &j_inscr, &m_inscr, &a_inscr) != 3) continue;
        duree = atoi(fields[9]); // durée en mois

        // Créer la date d'expiration
        struct tm expiry = {0};
        expiry.tm_mday = j_inscr;
        expiry.tm_mon = m_inscr - 1 + duree; // ajouter les mois
        expiry.tm_year = a_inscr - 1900;

        // mktime va normaliser la date automatiquement
        time_t expiry_time = mktime(&expiry);

        if (difftime(expiry_time, now) >= 0)
            (*actifs)++;
    }

    fclose(f);
    *inactifs = *total - *actifs;
    *pourcentage = (*total > 0) ? ((float)(*actifs) / (*total)) * 100.0 : 0.0;

    return 1;
}


/* ----------------- IMC ----------------- */
float calculer_imc(const char *id_recherche, const char *filename)
{
    FILE *f=fopen(filename,"r");
    if(!f) return -1;
    char ligne[512]; int found=0; float imc=-1;

    while(fgets(ligne,sizeof(ligne),f)){
        ligne[strcspn(ligne,"\r\n")]=0;
        char *data[12]; int i=0;
        char *token=strtok(ligne,";");
        while(token && i<12){data[i++]=token; token=strtok(NULL,";");}
        if(i<12) continue;

        if(strcmp(data[0],id_recherche)==0){
            float l=atof(data[7])/100.0;
            float p=atof(data[8]);
            if(l>0) imc=p/(l*l);
            found=1;
            break;
        }
    }
    fclose(f);
    return imc;
}


int charger_abonnements(GtkListStore *store, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    gtk_list_store_clear(store);  // vider le TreeView
    char ligne[256];
    GtkTreeIter iter;

    while(fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n")] = 0; // retirer le \n

        char *fields[6];
        int i = 0;
        char *token = strtok(ligne, ";");
        while(token && i < 6) {
            fields[i++] = token;
            token = strtok(NULL, ";");
        }

        if(i == 6) { // vérifier qu’on a bien toutes les colonnes
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               0, fields[0],  // type
                               1, fields[1],  // accès
                               2, fields[2],  // sonna/hamem
                               3, fields[3],  // coaching
                               4, fields[4],  // durée
                               5, fields[5],  // prix
                               -1);
        }
    }

    fclose(f);
    return 1;
}



int supprimer_abonnement(const char *type, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    FILE *tmp = fopen("tmp.txt", "w");
    if (!tmp) { fclose(f); return 0; }

    char ligne[256];
    int deleted = 0;

    while (fgets(ligne, sizeof(ligne), f)) {
        char copy[256]; strcpy(copy, ligne);
        copy[strcspn(copy, "\n")] = 0;
        char *token = strtok(copy, ";");
        if (token && strcmp(token, type) == 0) {
            deleted = 1;
            continue; // on saute cette ligne
        }
        fputs(ligne, tmp);
    }

    fclose(f);
    fclose(tmp);

    if (deleted) {
        remove(filename);
        rename("tmp.txt", filename);
    } else {
        remove("tmp.txt");
    }

    return deleted;
}



int rechercher_abonnement(GtkWidget *treeview, const char *type_recherche)
{
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    GtkTreeIter iter;

    if (!model)
        return 0;

    int valid = gtk_tree_model_get_iter_first(model, &iter);

    while (valid) {

        char *current_type = NULL;

        gtk_tree_model_get(model, &iter,
                           0, &current_type,   // column 0
                           -1);

        if (current_type && strcmp(current_type, type_recherche) == 0) {

            GtkTreeSelection *sel =
                gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
            gtk_tree_selection_select_iter(sel, &iter);

            GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
           gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(treeview),
                                         path, NULL, TRUE, 0.5, 0.0);

            gtk_tree_path_free(path);

            g_free(current_type);
            return 1;   // FOUND
        }

        g_free(current_type);
        valid = gtk_tree_model_iter_next(model, &iter);
    }

    return 0;  // NOT FOUND
}

int ajouter_demande_coach(demande_coach d, const char *filename)
{
    FILE *f = fopen(filename, "a");
    if (!f) return 0;

    fprintf(f, "%s;%s;%s;%s;%d;%d;%d;%s;%s;%s\n",
            d.nom_membre,
            d.type_entrainement,
            d.coach_choisi,
            d.heure,
            d.jour_souhaite,
            d.mois_souhaite,
            d.annee_souhaitee,
            d.type_seance,
            d.materiel,
            d.commentaire);

    fclose(f);
    return 1;
}


void lire_date_selectionnee(GtkWidget *calendar, int *j, int *m, int *a)
{
    gtk_calendar_get_date(GTK_CALENDAR(calendar),
                          (guint *)a,
                          (guint *)m,
                          (guint *)j);
    *m += 1; // GTK months start at 0
}



void get_type_seance(GtkWidget *c1, GtkWidget *c2, char *out)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(c1)))
        strcpy(out, "En salle");
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(c2)))
        strcpy(out, "En Ligne");
    else
        strcpy(out, "Non spécifié");
}


void get_materiel(GtkWidget *r2, GtkWidget *r3, char *out)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(r2)))
        strcpy(out, "Avec");
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(r3)))
        strcpy(out, "Sans");
    else
        strcpy(out, "N/S");
}









