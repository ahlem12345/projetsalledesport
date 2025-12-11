#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

void on_button1_clicked(GtkWidget *widget, gpointer user_data);
gchar* generate_coach_response(const gchar *question);

#endif /* CALLBACKS_H */
