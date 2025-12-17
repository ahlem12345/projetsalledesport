#include <gtk/gtk.h>


void rafraichir_treeview(GtkWidget *treeview);

void on_treeview1_row_activated(GtkTreeView *treeview,GtkTreePath *path,GtkTreeViewColumn *column,
gpointer user_data);



void
on_buttonRech_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonModif_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonAjout_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonSupp_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobutton3_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_calendar1_day_selected              (GtkCalendar     *calendar,
                                        gpointer         user_data);

void
on_buttonVd_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonRes_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonAnnul_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonCal_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_checkbutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_buttonAff_clicked                   (GtkButton       *button,
                                        gpointer         user_data);


