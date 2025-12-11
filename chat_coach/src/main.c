#include <gtk/gtk.h>
#include "interface.h"
#include "callbacks.h"
#include "support.h"

int main(int argc, char *argv[]) {
    GtkWidget *window;
    
    /* Initialisation GTK */
    gtk_init(&argc, &argv);
    
    /* Création fenêtre */
    window = create_window1();
    
    if (window) {
        /* Configuration titre */
        gtk_window_set_title(GTK_WINDOW(window), "Coach Anis - Expert Sportif");
        
        /* Personnalisation widgets */
        GtkWidget *label1 = lookup_widget(window, "label1");
        if (label1) {
            gtk_label_set_text(GTK_LABEL(label1), "💪 COACH ANIS");
            /* Optionnel : police plus grande */
            PangoFontDescription *font_desc = pango_font_description_from_string("Sans Bold 20");
            gtk_widget_modify_font(label1, font_desc);
            pango_font_description_free(font_desc);
        }
        
        GtkWidget *label2 = lookup_widget(window, "label2");
        if (label2) {
            gtk_label_set_text(GTK_LABEL(label2),
                "💪 **BIENVENUE CHEZ COACH ANIS !**\n\n"
                "**Votre expert sportif personnel**\n\n"
                "**Spécialités** :\n"
                "• Programmes musculation sur mesure\n"
                "• Conseils nutrition personnalisés\n"
                "• Plans cardio efficaces\n"
                "• Prévention des blessures\n"
                "• Motivation et suivi\n\n"
                "**Posez-moi vos questions** :\n"
                "• 'Quel programme pour débutant ?'\n"
                "• 'Comment prendre du muscle ?'\n"
                "• 'Nutrition pour perdre du poids ?'\n"
                "• 'Comment éviter les blessures ?'");
            
            gtk_label_set_line_wrap(GTK_LABEL(label2), TRUE);
            
            /* Optionnel : couleur de fond */
            GdkColor color;
            gdk_color_parse("#f0f8ff", &color);  /* AliceBlue */
            gtk_widget_modify_bg(label2, GTK_STATE_NORMAL, &color);
        }
        
        GtkWidget *entry1 = lookup_widget(window, "entry1");
        if (entry1) {
            gtk_entry_set_text(GTK_ENTRY(entry1), "Posez votre question sportive ici...");
        }
        
        GtkWidget *button1 = lookup_widget(window, "button1");
        if (button1) {
            gtk_button_set_label(GTK_BUTTON(button1), "💬 DEMANDER AU COACH");
        }
        
        /* Afficher tout */
        gtk_widget_show_all(window);
        
        /* Centrer la fenêtre */
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        
    } else {
        g_critical("Failed to create window");
        return 1;
    }
    
    /* Boucle principale */
    gtk_main();
    
    return 0;
}
