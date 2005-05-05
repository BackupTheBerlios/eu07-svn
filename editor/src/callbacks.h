#include <gtk/gtk.h>

void init_interface (int *, char ***);

void
on_new_toolbutton_clicked (GtkToolButton * toolbutton, gpointer user_data);

void
on_open_toolbutton_clicked (GtkToolButton * toolbutton, gpointer user_data);

void
on_save_toolbutton_clicked (GtkToolButton * toolbutton, gpointer user_data);

void on_new_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_open_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_save_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_save_as_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_quit_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_cut_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_copy_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_paste_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_delete_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_about_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void
on_select_mode_radiotoolbutton_toggled (GtkToggleToolButton *
					toggletoolbutton, gpointer user_data);

void
on_move_mode_radiotoolbutton_toggled (GtkToggleToolButton * toggletoolbutton,
				      gpointer user_data);

void
on_rotate_mode_radiotoolbutton_toggled (GtkToggleToolButton *
					toggletoolbutton, gpointer user_data);

void
  on_createPoints_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data);

void
  on_createPointFeatures_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data);

void
  on_createLines_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data);

void
  on_connectPoints_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data);

void
  on_createRailLines_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data);

void
  on_createTracks_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data);

void
  on_createFence_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data);

void
on_divide_mode_radiotoolbutton_toggled (GtkToggleToolButton *
					toggletoolbutton, gpointer user_data);

void
on_fill_mode_radiotoolbutton_toggled (GtkToggleToolButton * toggletoolbutton,
				      gpointer user_data);

void on_export_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_select_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_move_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void on_rotate_mi_activate (GtkMenuItem * menuitem, gpointer user_data);

void
on_signals_mode_radiotoolbutton_toggled
                                        (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

gboolean
on_main_window_key_press_event         (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

void
on_catenary_mode_radiotoolbutton_toggled
                                        (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_import_mode_radiotoolbutton_toggled (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);

void
on_import_mode_radiotoolbutton_toggled (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data);
