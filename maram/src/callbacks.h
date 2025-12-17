#include <gtk/gtk.h>


void
on_radiobutton_hg_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton_tr_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_conf_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button_addc_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobutton_trm_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton_hgm_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button_mod_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_checkbutton_confmod_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button_supp_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_button_rechc_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_affc_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_tradd_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_treeview_trainer_row_activated      (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_button_searchtr_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_afftr_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_treeview2_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_buttoncalcul_clicked                (GtkButton       *button,
                                        gpointer         user_data);
