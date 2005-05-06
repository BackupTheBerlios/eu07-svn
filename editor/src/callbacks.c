#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include "core/GL_realizer.h"
#include "publisher.h"
#include "property_controls.h"
#include <stdlib.h>
#include <gdk/gdkkeysyms.h>


#ifndef EDITOR_FULL

#include <GL/gl.h>
#include <GL/glu.h>
#include "core/GL_realizer.h"
#include "atrapy.h"
#include <windows.h>
#else
#include "core/TopView.h"
#include "core/PerspectiveView.h"
#include "core/Editor.h"
#endif


GtkWidget *main_window, *window2, *quit_dialog;


gboolean
to_handler (gpointer data)
{
  static int i = 2;
  printf ("timeout occured\n");
  /*
     if (i=!i)
     ((Kit*)data)[1].setOwnerWithRead(NULL);
     else
     ((Kit*)data)[1].setOwnerWithWrite(NULL);
   */
  if (i--)
    ((Kit *) data)[1 - i].activate ();
  else
    {
      Kit::disable ();
      i += 3;
    }

  return TRUE;
}

gboolean gajewski_h(gpointer data)
{
         return TRUE;
}

void
init_interface (int *argcp, char ***argvp)
{
printf("init_interface started\n");
  GL_realizer::gl_init (argcp, argvp);

  main_window = create_main_window ();
  Publisher::init (main_window);
  Kit::external_widget =
    (GtkBox *) lookup_widget (main_window, "properties_vbox");
  g_signal_connect ((gpointer) main_window, "destroy",
		    G_CALLBACK (gtk_main_quit), NULL);

  window2 = create_window2 ();
  g_signal_connect ((gpointer) window2, "destroy", G_CALLBACK (gtk_main_quit),
		    NULL);
  gtk_window_set_transient_for (GTK_WINDOW (window2),
				GTK_WINDOW (main_window));
		    
  quit_dialog = create_quit_dialog();
  gtk_window_set_transient_for (GTK_WINDOW (quit_dialog),
				GTK_WINDOW (main_window));

printf("Wins created, not shown\n"); 


#ifdef EDITOR_FULL
  osg::ref_ptr < TopView > top =
    new TopView (lookup_widget (main_window, "main_drawingarea"));
printf("(1)\n");
  osg::ref_ptr < PerspectiveView > perspective =
    new PerspectiveView (lookup_widget (window2, "drawingarea2"));
printf("(2)\n");
  static Editor editor (top.get ());
printf("(3)\n");
  editor.addPerspectiveView (perspective.get ());
printf("(4)\n");
  perspective->
    unsetNodeMask (Editor::Masks::nm_Nodes | Editor::Masks::nm_Maps);
printf("(5)\n");
#else
  Publisher::set_status_msg (3, "Publisher::set_status_msg(...)");
  static foo realizer2 (lookup_widget (window2, "drawingarea2"));
  static foo realizer1 (lookup_widget (main_window, "main_drawingarea"));
  static Editor editor;
  // printf("foos created\n"); _sleep(4444);
#endif
printf("Win show attempt\n"); 
  gtk_widget_show (main_window);
  gtk_widget_show (window2);
printf("Wins shown\n"); 

#ifndef EDITOR_FULL

  Kit *kit = new Kit[3];
  
  kit[1].addIntEntry ("Malinowski", 0, 100, 1, 1);
  kit[1].addIntEntry ("Kowalski", 0, 1000, 1, 555);
  kit[1].addFileSel ("Majewski", "C:\\WINDOWS\\Desktop\\Editor\\src","(*.h$)|(*.o$)","Konarski");
  kit[0].addTextEntry ("Wisniewski", "");
  kit[0].addTextEntry ("Bednarski", "");
  kit[0].addTextEntry ("Jankowski", "");
  kit[0].addTextEntry ("Kozlowski", "");
  kit[0].addTextEntry ("Jaworski", "");
  kit[0].addButton ("Borowski", 1, 0);
  kit[0].addFileSel ("Majewski", "C:\\WINDOWS\\Desktop\\Editor","*.*","Zielinski");
  static Kit::Item gajewski_item = kit[0].addButton ("Gajewski", 1, 1);
  // gajewski_item.switching();

  // kit[1].activate();
  gtk_timeout_add (7000, to_handler, kit);
  //gtk_timeout_add (1000, gajewski_h, &kit[0]);
  
#endif

printf("ret/init_interface\n");

};




void
on_new_toolbutton_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
  Editor::instance ()->freeNodes ();
}


void
on_open_toolbutton_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
  GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET (toolbutton));
  Editor::instance ()->loadFromFile ("test.bscn");
}


void
on_save_toolbutton_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
  GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET (toolbutton));
  Editor::instance ()->saveToFile ("test.bscn");
}


void
on_new_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  Editor::instance ()->freeNodes ();
}


void
on_open_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  Editor::instance ()->loadFromFile ("test.bscn");
}


void
on_save_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  Editor::instance ()->saveToFile ("test.bscn");
}


void
on_save_as_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  Editor::instance ()->saveToFile ("test.bscn");
}


void
on_cut_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
//	Editor::instance()->toggleCurve();
}


void
on_copy_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
}


void
on_paste_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
//	Editor::instance()->applyFeatures();
}


void
on_delete_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	Editor::instance()->freeSelectedNode();
}


void
on_about_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
}


void
on_select_mode_radiotoolbutton_toggled (GtkToggleToolButton *
					toggletoolbutton, gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_Select);
}


void
on_move_mode_radiotoolbutton_toggled (GtkToggleToolButton * toggletoolbutton,
				      gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_Move);
}


void
on_rotate_mode_radiotoolbutton_toggled (GtkToggleToolButton *
					toggletoolbutton, gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_CreateCatenary);
}


void
  on_createPoints_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_CreatePoints);
}


void
  on_createPointFeatures_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_CreatePointFeatures);
}


void
  on_createLines_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_CreateLines);
}


void
  on_connectPoints_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_ConnectPoints);
}


void
  on_createRailLines_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_CreateRailLines);
}


void
  on_createTracks_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_CreateTracks);
}


void
  on_createFence_mode_radiotoolbutton_toggled
  (GtkToggleToolButton * toggletoolbutton, gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_CreateFence);
}


void
on_fill_mode_radiotoolbutton_toggled (GtkToggleToolButton * toggletoolbutton,
				      gpointer user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_Fill);
}


void
on_export_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  Editor::instance ()->saveToFile ("tmp.bscn");
  Editor::instance ()->exportToDirectory ("..\\eu07\\scenery\\test\\");
  gtk_main_quit();
//  Editor::instance ()->freeNodes ();
}


gboolean
on_main_window_key_press_event (GtkWidget * widget,
				GdkEventKey * event, gpointer user_data)
{
  int i;
  switch (event->keyval)
    {
    case GDK_a:
		
      printf ("a pr/rel\n");
      break;
    case GDK_A:
      printf ("A pr/rel\n");
      break;
    case GDK_Delete:
//    case GDK_d:
	  Editor::instance ()->freeSelectedNode();
      break;
    }

  i = event->keyval;
  // printf ("code: %d\n",i);

  for (int i = 0; i < 32; i++)
    printf ("%d%s", !!(event->state & (1 << i)), i == 31 ? "\n" : "");

  return FALSE;
}


void
on_select_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  printf ("Select@menu\n");
}


void
on_move_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  printf ("Move@menu\n");
}


void
on_rotate_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  printf ("Rotate@menu\n");
}

void
on_signals_mode_radiotoolbutton_toggled
                                        (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
    Editor::instance ()->changeMode (Editor::em_CreateSignals);
}

/*
gboolean
on_main_window_key_press_event         (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{

  return FALSE;
}
*/

void
on_catenary_mode_radiotoolbutton_toggled
                                        (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
	  Editor::instance ()->changeMode (Editor::em_CreateCatenary);
}

void
on_import_mode_radiotoolbutton_toggled (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data)
{
  if (gtk_toggle_tool_button_get_active (toggletoolbutton))
	  Editor::instance ()->changeMode (Editor::em_Import);

}

gboolean quit_request()
{
         gtk_widget_show(quit_dialog);
         return TRUE;
}

void
on_quit_toolbutton_clicked             (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
   quit_request();
}


void
on_quit_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
   quit_request();
}

void
on_quit_dialog_response                (GtkDialog       *dialog,
                                        gint             response_id,
                                        gpointer         user_data)
{
   switch(response_id)
   {
       case GTK_RESPONSE_YES:
            // trza zapisac:
			Editor::instance ()->saveToFile ("test.bscn");
                    
       case GTK_RESPONSE_NO:
            gtk_main_quit();
            
       case GTK_RESPONSE_CANCEL:
            gtk_widget_hide(GTK_WIDGET(dialog));
            
       case GTK_RESPONSE_DELETE_EVENT:
            return;
       
       default:
            Publisher::warn("Code bug:\nUnknown response from the 'quit dialog'");
   }
}

gboolean
on_main_window_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  return quit_request();
}


gboolean
on_main_window_destroy_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return quit_request();
}


gboolean
on_window2_delete_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return quit_request();
}


gboolean
on_window2_destroy_event               (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return quit_request();
}

