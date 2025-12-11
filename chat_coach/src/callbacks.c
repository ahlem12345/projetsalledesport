#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"

/* ========== CALLBACK PRINCIPAL ========== */

void on_button1_clicked(GtkWidget *widget, gpointer user_data) {
    (void)user_data;
    
    GtkWidget *window = gtk_widget_get_toplevel(widget);
    if (!GTK_IS_WINDOW(window)) {
        g_warning("Parent widget is not a window");
        return;
    }
    
    GtkWidget *entry = lookup_widget(window, "entry1");
    GtkWidget *label = lookup_widget(window, "label2");
    
    if (!entry || !label) {
        g_warning("Could not find entry1 or label2");
        return;
    }
    
    const gchar *question = gtk_entry_get_text(GTK_ENTRY(entry));
    
    /* Message de recherche - version simple */
    gchar *loading_msg = g_strdup_printf(
        "🔍 **SIMULATION DE RECHERCHE WEB**\n\n"
        "**Question** : '%s'\n\n"
        "**État** : Connexion au réseau sportif...\n"
        "Analyse des bases de données...\n"
        "Extraction des informations pertinentes...", 
        question);
    
    gtk_label_set_text(GTK_LABEL(label), loading_msg);
    g_free(loading_msg);
    
    /* Forcer l'affichage */
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
    
    /* Simuler un délai de recherche */
    for (int i = 0; i < 3; i++) {
        g_usleep(300000);  /* 0.3 seconde */
        
        gchar *dots = g_strdup_printf(
            "🔍 **SIMULATION DE RECHERCHE WEB**\n\n"
            "**Question** : '%s'\n\n"
            "**État** : Connexion au réseau sportif...\n"
            "Analyse des bases de données...\n"
            "Extraction des informations pertinentes%s", 
            question, 
            i == 0 ? "." : i == 1 ? ".." : "...");
        
        gtk_label_set_text(GTK_LABEL(label), dots);
        g_free(dots);
        
        while (gtk_events_pending()) {
            gtk_main_iteration();
        }
    }
    
    /* Effectuer la recherche simulée - version simplifiée sans CURL */
    gchar *response = generate_coach_response(question);
    
    /* Afficher les résultats */
    gtk_label_set_text(GTK_LABEL(label), response);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    
    /* Libérer la mémoire */
    g_free(response);
    
    /* Forcer rafraîchissement */
    while (gtk_events_pending()) {
        gtk_main_iteration();
    }
}

/* ========== GÉNÉRATION DE RÉPONSE SIMPLIFIÉE ========== */

/* Fonction utilitaire pour mettre en minuscules */
static gchar* str_to_lower(const gchar *str) {
    if (!str) return NULL;
    gchar *result = g_strdup(str);
    gchar *p = result;
    while (*p) {
        *p = tolower(*p);
        p++;
    }
    return result;
}

/* Base de connaissances simplifiée */
typedef struct {
    const gchar *keywords;
    const gchar *response;
} SportKnowledge;

static const SportKnowledge sport_knowledge[] = {
    /* MUSCULATION */
    {"muscu|musculation|biceps|triceps|pec|dos|jambes",
     "💪 **MUSCULATION - Guide Complet**\n\n"
     "**Programme Débutant (Full Body)** :\n"
     "• Squat : 3x8-12\n• Développé Couché : 3x8-12\n"
     "• Rowing : 3x8-12\n• Pompe : 3x10-15\n\n"
     "**Nutrition** : 2g protéines/kg, surplus 300-500kcal\n\n"
     "**Fréquence** : 3-4 séances/semaine"},
    
    {"cardio|course|running|vélo|natation|endurance",
     "🏃 **CARDIOVASCULAIRE**\n\n"
     "**Recommandations** :\n"
     "• Santé : 150min/semaine modéré\n"
     "• Perte poids : 200-300min/semaine\n"
     "• Performance : HIIT + endurance\n\n"
     "**Zones FC** : (220 - âge) × 60-80%"},
    
    {"nutrition|aliment|protéine|glucide|lipide|manger",
     "🍗 **NUTRITION SPORTIVE**\n\n"
     "**Macronutriments** :\n"
     "• Protéines : 1.6-2.5g/kg\n"
     "• Glucides : 3-8g/kg\n"
     "• Lipides : 0.8-1.5g/kg\n\n"
     "**Hydratation** : 35-45ml/kg/jour"},
    
    {"perdre|maigrir|poids|graisse|ventre",
     "⚖️ **PERTE DE POIDS**\n\n"
     "**Stratégie** :\n"
     "• Déficit calorique : 300-500kcal/jour\n"
     "• Rythme : 0.5-1kg/semaine\n"
     "• Combinaison : Nutrition 80% + Sport 20%\n\n"
     "**Conseil** : Évitez les régimes express"},
    
    {"programme|plan|routine|séance|entraînement",
     "📅 **PROGRAMMES**\n\n"
     "**Débutant** : Full Body 3x/semaine\n"
     "**Intermédiaire** : Upper/Lower 4x/semaine\n"
     "**Avancé** : Push/Pull/Legs 6x/semaine\n\n"
     "**Périodisation** : Changez toutes les 6-8 semaines"},
    
    {"blessure|douleur|mal|articulation|tendinite",
     "⚠️ **BLESSURES**\n\n"
     "**Protocole RICE** :\n"
     "• Repos\n• Glace\n• Compression\n• Élévation\n\n"
     "**Consulter si** :\n"
     "• Douleur >48h\n• Incapacité bouger\n• Gonflement"},
    
    {"motivation|démotivé|fatigué|abandonner",
     "💪 **MOTIVATION**\n\n"
     "**Stratégies** :\n"
     "1. Objectifs SMART\n"
     "2. Routine > Motivation\n"
     "3. Suivi visuel\n"
     "4. Partenaire d'entraînement\n\n"
     "**Conseil** : Discipline surpasse motivation"},
    
    {"salle|gym|machine|appareil|haltère",
     "🏋️ **SALLE DE SPORT**\n\n"
     "**Débutants** : Machines guidées\n"
     "**Expérimentés** : Poids libres\n\n"
     "**Étiquette** :\n"
     "• Rangez vos poids\n• Essuyez les machines\n• Respectez les autres"},
    
    {"repos|récupération|dormir|sommeil",
     "😴 **RÉCUPÉRATION**\n\n"
     "**Sommeil** : 7-9h/nuit qualité\n"
     "**Récupération active** : Marche, étirements\n"
     "**Nutrition** : Protéines post-entraînement\n\n"
     "**Signes surentraînement** : Fatigue persistante"},
    
    {"complément|protéine poudre|créatine|whey",
     "💊 **COMPLÉMENTS**\n\n"
     "**Utiles** :\n"
     "• Whey protéine\n• Créatine monohydrate\n• Caféine\n• Vitamine D\n\n"
     "**Précautions** :\n"
     "• D'abord alimentation équilibrée\n• Respectez dosages"},
    
    {"bonjour|salut|hello|coucou",
     "👋 **COACH ANIS - Assistant Sportif**\n\n"
     "Je simule un moteur de recherche web pour vos questions sportives !\n\n"
     "**Exemples** :\n"
     "• 'programme débutant musculation'\n"
     "• 'nutrition prise muscle'\n"
     "• 'cardio perte graisse'\n\n"
     "Posez votre question !"},
    
    {"merci|remercie",
     "🙏 **Avec plaisir !**\n\n"
     "N'hésitez pas pour d'autres questions sportives.\n"
     "Mon système analyse vos questions et trouve les réponses les plus pertinentes."}
};

#define KNOWLEDGE_SIZE (sizeof(sport_knowledge) / sizeof(SportKnowledge))

/* Recherche dans la base */
gchar* generate_coach_response(const gchar *question) {
    if (!question || strlen(question) == 0) {
        return g_strdup("🔍 **MOTEUR DE RECHERCHE SPORTIF**\n\n"
                       "Posez votre question pour obtenir une réponse détaillée !");
    }
    
    gchar *question_lower = str_to_lower(question);
    gdouble best_score = 0.0;
    const gchar *best_response = NULL;
    
    /* Recherche du meilleur match */
    for (guint i = 0; i < KNOWLEDGE_SIZE; i++) {
        gdouble score = 0.0;
        gchar *keywords_copy = g_strdup(sport_knowledge[i].keywords);
        
        /* Compter les mots-clés correspondants */
        gchar *token = strtok(keywords_copy, "|");
        while (token != NULL) {
            if (strstr(question_lower, token) != NULL) {
                score += 1.0;
            }
            token = strtok(NULL, "|");
        }
        
        g_free(keywords_copy);
        
        if (score > best_score) {
            best_score = score;
            best_response = sport_knowledge[i].response;
        }
    }
    
    g_free(question_lower);
    
    /* Retourner la réponse */
    if (best_score > 0) {
        /* Ajouter un en-tête de recherche */
        gchar *search_header = g_strdup_printf(
            "🔍 **RECHERCHE : %d correspondance(s)**\n\n"
            "**Question** : \"%s\"\n\n"
            "%s\n\n"
            "💡 *Résultat simulé d'une recherche web sportive*", 
            (int)best_score, question, best_response);
        
        return search_header;
    }
    
    /* Si aucune correspondance */
    return g_strdup_printf(
        "🔍 **RECHERCHE EN LIGNE SIMULÉE**\n\n"
        "**Question** : \"%s\"\n\n"
        "**Résultats** : Aucune correspondance exacte trouvée\n\n"
        "**Suggestions** :\n"
        "1. Reformulez avec des mots-clés sportifs\n"
        "2. Essayez : 'musculation', 'cardio', 'nutrition'\n"
        "3. Précisez votre niveau et objectif\n\n"
        "💡 *Ce système simule une recherche web avancée "
        "avec une base de connaissances sportive étendue.*", 
        question);
}
