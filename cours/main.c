#include <gtk/gtk.h>
#include "callbacks.h"
#include "cours.h"

//Ancien main

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "interface.glade", &error);

    if (error)
    {
        g_print("Erreur Glade: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    // Connecte tous les signaux, user_data = builder
    gtk_builder_connect_signals(builder, builder);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

// Callback pour fermer la fenêtre
void on_window_destroy(GtkWidget *widget, gpointer user_data)
{
    gtk_main_quit();
}

