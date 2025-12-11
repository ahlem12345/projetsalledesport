#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget* lookup_widget(GtkWidget *widget, const gchar *widget_name) {
    GtkWidget *parent, *found_widget;
    
    for (;;) {
        if (GTK_IS_MENU(widget))
            parent = gtk_menu_get_attach_widget(GTK_MENU(widget));
        else
            parent = widget->parent;
        if (!parent)
            parent = (GtkWidget*) g_object_get_data(G_OBJECT(widget), "GladeParentKey");
        if (!parent)
            break;
        widget = parent;
    }
    
    found_widget = (GtkWidget*) g_object_get_data(G_OBJECT(widget), widget_name);
    if (!found_widget)
        g_warning("Widget not found: %s", widget_name);
    
    return found_widget;
}

GtkWidget* create_pixmap(GtkWidget *widget, const gchar *filename) {
    (void)widget;
    
    GtkWidget *image = NULL;
    gchar *full_path = NULL;
    
    /* Essayer avec le chemin complet d'abord */
    if (filename && strlen(filename) > 0) {
        /* Essayer pixmaps/filename */
        full_path = g_strdup_printf("pixmaps/%s", filename);
        FILE *file = fopen(full_path, "r");
        
        if (file) {
            fclose(file);
            image = gtk_image_new_from_file(full_path);
            g_print("✅ Image chargée depuis : %s\n", full_path);
        } else {
            g_free(full_path);
            
            /* Essayer le chemin direct */
            file = fopen(filename, "r");
            if (file) {
                fclose(file);
                image = gtk_image_new_from_file(filename);
                g_print("✅ Image chargée depuis : %s\n", filename);
            } else {
                /* Créer une image de coach par défaut */
                g_print("⚠️ Image non trouvée. Création image coach par défaut.\n");
                image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO, GTK_ICON_SIZE_DIALOG);
            }
        }
        
        if (full_path) g_free(full_path);
    } else {
        /* Pas de fichier - image par défaut */
        image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO, GTK_ICON_SIZE_DIALOG);
    }
    
    return image;
}
