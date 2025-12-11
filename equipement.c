#include <stdio.h>
#include <string.h>
#include "equipement.h"
#include <gtk/gtk.h>



int ajouterEquipement(char *filename, Equipement e)
{
    FILE *f = fopen(filename, "a");
    if (f != NULL)
    {
        fprintf(f, "%s,%s,%s,%d,%d,%d\n",
                e.reference, e.nom, e.type, e.quantite,
                e.etat, e.localisation);
        fclose(f);
        return 1;
    }
    return 0;
}


int rechercherEquipement(char *filename, char *reference)
{
    Equipement e;
    FILE *f = fopen(filename, "r");
    if (f != NULL)
    {
         while (fscanf(f, "%[^,],%[^,],%[^,],%d,%d,%d\n",
                      e.reference, e.nom, e.type, &e.quantite,
                      &e.etat, &e.localisation) != EOF)
        {
            if (strcmp(e.reference, reference) == 0)
            {
                fclose(f);
                return 1;
            }
        }
        fclose(f);
    }
    return 0;
}

int modifierEquipement(char *filename, char *reference, Equipement nouv)
{
    Equipement e;
    int trouve = 0;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("equipements_temp.txt", "w");

    if (f != NULL && f2 != NULL)
    {
        while (fscanf(f, "%[^,],%[^,],%[^,],%d,%d,%d\n",
                      e.reference, e.nom, e.type, &e.quantite,
                      &e.etat, &e.localisation) != EOF)
        {
            if (strcmp(e.reference, reference) == 0)
            {
                fprintf(f2, "%s,%s,%s,%d,%d,%d\n",
                        nouv.reference, nouv.nom, nouv.type, nouv.quantite,
                        nouv.etat, nouv.localisation);
                trouve = 1;
            }
            else
            {
                fprintf(f2, "%s,%s,%s,%d,%d,%d\n",
                        e.reference, e.nom, e.type, e.quantite,
                        e.etat, e.localisation);
            }
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("equipements_temp.txt", filename);
    }
    return trouve;
}

int supprimerEquipement(char *filename, char *reference)
{
    Equipement e;
    int trouve = 0;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("equipements_temp.txt", "w");

    if (f != NULL && f2 != NULL)
    {
        while (fscanf(f, "%[^,],%[^,],%[^,],%d,%d,%d\n",
                      e.reference, e.nom, e.type, &e.quantite,
                      &e.etat, &e.localisation) != EOF)
        {
            if (strcmp(e.reference, reference) != 0)
            {
                fprintf(f2, "%s,%s,%s,%d,%d,%d\n",
                        e.reference, e.nom, e.type, e.quantite,
                        e.etat, e.localisation);
            }
            else
                trouve = 1;
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("equipements_temp.txt", filename);
    }
    return trouve;
}

void afficherEquipementsCTree(GtkWidget *ctree)
{
    GtkCTree *tree = GTK_CTREE(ctree);
    GtkCTreeNode *node;

    // Vider le treeview avant d’ajouter
    gtk_clist_clear(GTK_CLIST(ctree));

    FILE *f = fopen("equipements.txt", "r");
    if (!f) return;

    Equipement e;

    while (fscanf(f, "%[^,],%[^,],%[^,],%d,%d,%d\n",
                  e.reference, e.nom, e.type,
                  &e.quantite, &e.etat, &e.localisation) != EOF)
    {
        char *data[6];

        data[0] = e.reference;
        data[1] = e.nom;
        data[2] = e.type;

        static char buffer_qte[10];
        sprintf(buffer_qte, "%d", e.quantite);
        data[3] = buffer_qte;

        data[4] = (char*) etat_to_string(e.etat);
        data[5] = (char*) localisation_to_string(e.localisation);

        node = gtk_ctree_insert_node(
                    tree, NULL, NULL,
                    data, 0, NULL, NULL, NULL, NULL,
                    FALSE, FALSE);
    }

    fclose(f);
}




void afficherEquipements(char *filename)
{
    Equipement e;
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Erreur: impossible d’ouvrir %s\n", filename);
        return;
    }

    printf("\n--- Liste des équipements ---\n");
    while (fscanf(f, "%[^,],%[^,],%[^,],%d,%d,%d\n",
                      e.reference, e.nom, e.type, &e.quantite,
                      &e.etat, &e.localisation) != EOF)
    {
        printf("Ref: %s | Nom: %s | Type: %s | Qté: %d | État: %d | Loc: %d \n",
               e.reference, e.nom, e.type, e.quantite,
               e.etat, e.localisation);
    }
    fclose(f);
}

int ajouterReservation(char *filename, Reservation r)
{
    if (!verifierDisponibilite(filename, r.equipement, r.dateReservation, r.quantite))
    {
        // Équipement non disponible
        return 0;
    }
    FILE *f = fopen(filename, "a");
    if (f != NULL)
    {
        fprintf(f, "%s %s %d %d %d %d %d\n",
                r.id_coach, r.equipement, r.quantite,
                r.dateReservation.jour, r.dateReservation.mois, r.dateReservation.annee,
                r.active);
        fclose(f);
        return 1;
    }
    return 0;
}

int annulerReservation(char *filename, char *id_coach, char *equipement)
{
    Reservation r;
    int trouve = 0;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("reservations_temp.txt", "w");

    if (f != NULL && f2 != NULL)
    {
        while (fscanf(f, "%s %s %d %d %d %d %d",
                      r.id_coach, r.equipement, &r.quantite,
                      &r.dateReservation.jour, &r.dateReservation.mois, &r.dateReservation.annee,
                      &r.active) != EOF)
        {
            if (strcmp(r.id_coach, id_coach) == 0 && strcmp(r.equipement, equipement) == 0)
            {
                r.active = 0;
                trouve = 1;
            }
            fprintf(f2, "%s %s %d %d %d %d %d\n",
                    r.id_coach, r.equipement, r.quantite,
                    r.dateReservation.jour, r.dateReservation.mois, r.dateReservation.annee,
                    r.active);
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("reservations_temp.txt", filename);
    }
    return trouve;
}

int verifierDisponibilite(char *filename, char *equipement, Date date, int quantite)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
        return 0;

    Reservation r;
    int totalReserve = 0;

    while (fscanf(f, "%s %s %d %d %d %d %d",
                  r.id_coach,
                  r.equipement,
                  &r.quantite,
                  &r.dateReservation.jour,
                  &r.dateReservation.mois,
                  &r.dateReservation.annee,
                  &r.active) != EOF)
    {
        if (strcmp(r.equipement, equipement) == 0 &&
            r.dateReservation.jour == date.jour &&
            r.dateReservation.mois == date.mois &&
            r.dateReservation.annee == date.annee &&
            r.active == 1)
        {
            totalReserve += r.quantite;
        }
    }

    fclose(f);
    return totalReserve < quantite; 
}

void afficherReservations(char *filename)
{
    Reservation r;
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Erreur: impossible d’ouvrir %s\n", filename);
        return;
    }

    printf("\n--- Liste des réservations ---\n");
    while (fscanf(f, "%s %s %d %d %d %d %d",
                  r.id_coach, r.equipement, &r.quantite,
                  &r.dateReservation.jour, &r.dateReservation.mois, &r.dateReservation.annee,
                  &r.active) != EOF)
    {
        printf("Coach: %s | Equipement: %s | Qté: %d | Date: %02d/%02d/%04d | Active: %d\n",
               r.id_coach, r.equipement, r.quantite,
               r.dateReservation.jour, r.dateReservation.mois, r.dateReservation.annee,
               r.active);
    }

    fclose(f);
}
void calculerStatistiques(char *fichierEquip, char *fichierReserv)
{
    FILE *fe = fopen(fichierEquip, "r");
    FILE *fr = fopen(fichierReserv, "r");
    FILE *fs = fopen("statistiques.txt", "w");

    if (!fe || !fr || !fs)
    {
        printf("Erreur : impossible d’ouvrir les fichiers !\n");
        return;
    }

    Equipement e;
    Reservation r;

    char equipements[MAX_EQUIP][50];
    int compteEquip[MAX_EQUIP] = {0};
    int nEquip = 0;

    char coachs[MAX_COACH][50];
    int compteCoach[MAX_COACH] = {0};
    int nCoach = 0;

    char equipMaintenance[MAX_EQUIP][50];
    int nbMaintenance[MAX_EQUIP] = {0};
    int nMaint = 0;

    int nbTotalEquip = 0;
    int nbEquipementsReserves = 0;
    while (fscanf(fe, "%s %s %s %d %d %d",
                  e.reference, e.nom, e.type, &e.quantite,
                  &e.etat, &e.localisation) != EOF)
    {
        nbTotalEquip++;

        int idx = -1;
        for (int i = 0; i < nMaint; i++)
            if (strcmp(equipMaintenance[i], e.nom) == 0)
                idx = i;

        if (idx == -1)
        {
            strcpy(equipMaintenance[nMaint], e.nom);
            nbMaintenance[nMaint] = (e.etat == MAINT) ? e.quantite : 0;
            nMaint++;
        }
        else
        {
            if (e.etat == MAINT)
                nbMaintenance[idx] += e.quantite;
        }
    }
    fclose(fe);
    while (fscanf(fr, "%s %s %d %d %d %d %d",
                  r.id_coach, r.equipement, &r.quantite,
                  &r.dateReservation.jour, &r.dateReservation.mois, &r.dateReservation.annee,
                  &r.active) != EOF)
    {
        if (r.active == 1)
        {
            nbEquipementsReserves += r.quantite;

            int trouve = 0;
            for (int i = 0; i < nEquip; i++)
            {
                if (strcmp(equipements[i], r.equipement) == 0)
                {
                    compteEquip[i] += r.quantite;
                    trouve = 1;
                    break;
                }
            }
            if (!trouve)
            {
                strcpy(equipements[nEquip], r.equipement);
                compteEquip[nEquip] = r.quantite;
                nEquip++;
            }

            trouve = 0;
            for (int i = 0; i < nCoach; i++)
            {
                if (strcmp(coachs[i], r.id_coach) == 0)
                {
                    compteCoach[i] += r.quantite;
                    trouve = 1;
                    break;
                }
            }
            if (!trouve)
            {
                strcpy(coachs[nCoach], r.id_coach);
                compteCoach[nCoach] = r.quantite;
                nCoach++;
            }
        }
    }
    fclose(fr);
    int maxRes = -1;
    char equipPlusRes[50] = "Aucun";
    for (int i = 0; i < nEquip; i++)
        if (compteEquip[i] > maxRes)
        {
            maxRes = compteEquip[i];
            strcpy(equipPlusRes, equipements[i]);
        }
    int maxMaint = -1;
    char equipPlusMaint[50] = "Aucun";
    for (int i = 0; i < nMaint; i++)
        if (nbMaintenance[i] > maxMaint)
        {
            maxMaint = nbMaintenance[i];
            strcpy(equipPlusMaint, equipMaintenance[i]);
        }

    int maxActif = -1;
    char coachPlusActif[50] = "Aucun";
    for (int i = 0; i < nCoach; i++)
        if (compteCoach[i] > maxActif)
        {
            maxActif = compteCoach[i];
            strcpy(coachPlusActif, coachs[i]);
        }
    
     
    fprintf(fs, "===== STATISTIQUES DES EQUIPEMENTS =====\n");
    fprintf(fs, "Equipement le plus réservé : %s\n", equipPlusRes);
    fprintf(fs, "Equipement en maintenance  : %s\n", equipPlusMaint);
    fprintf(fs, "Coach le plus actif         : %s\n", coachPlusActif);


    fclose(fs);

    printf("Statistiques sauvegardées dans statistiques.txt\n");
} 
