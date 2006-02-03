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
#include "undo.h"

#include <string>

#ifndef EDITOR_FULL

#include <GL/gl.h>
#include <GL/glu.h>
#include "core/GL_realizer.h"
#include "atrapy.h"
#else
#include "core/TopView.h"
#include "core/PerspectiveView.h"
#include "core/Editor.h"
#include "core/options.h"
#endif

const int AUTOSAVE_DELAY_SECONDS = 120;
const char *AUTOSAVE_FILENAME = "autosave.bscn";

GtkWidget *main_window, *window2, *quit_dialog, *new_dialog;

/* GdkPixbuf RGBA C-Source image dump */

#ifdef __SUNPRO_C
#pragma align 4 (icon_move)
#endif
#ifdef __GNUC__
static const guint8 icon_move[] __attribute__ ((__aligned__ (4))) = 
#else
static const guint8 icon_move[] = 
#endif
{ ""
  /* Pixbuf magic (0x47646b50) */
  "GdkP"
  /* length: header (24) + pixel_data (2304) */
  "\0\0\11\30"
  /* pixdata_type (0x1010002) */
  "\1\1\0\2"
  /* rowstride (96) */
  "\0\0\0`"
  /* width (24) */
  "\0\0\0\30"
  /* height (24) */
  "\0\0\0\30"
  /* pixel_data: */
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\205\205\205\377BBB\377\0\0\0&\0\0\0\14\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\245\245\245\377\346\363\362\377\233\316\315\377\0\0\0\377\0\0"
  "\0\77\0\0\0\14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\241\241\241\377\377\377\377\377\340\357\357\377"
  "\216\307\307\377/\212\212\377\0\0\0\377\0\0\0\77\0\0\0\14\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0www\377\377\377\377\377"
  "\377\377\377\377\324\352\352\377z\267\266\377(vv\377\0QQ\377\0\0\0\377"
  "\0\0\0\77\0\0\0\14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377"
  "\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0""2"
  "22\377---\377'''\377\314\346\346\377j\237\240\377\0\0\0\377\0\0\0\377"
  "\0\0\0\377\0\0\0\177\0\0\0&\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0&\0\0\0r###\377\315\347\346\377b\223\224\377\0\0\0\377"
  "\0\0\0\277\0\0\0\246\0\0\0r\0\0\0&\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\14\0\0\0&OOO\377\323\352\351\377d\227\227\377"
  "\0\0\0\377\0\0\0\246\0\0\0Y\0\0\0&\0\0\0\14\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0www\3772"
  "22\377\0\0\0&\0\0\0\14\0\0\0\0\0\0\0\0zzz\377\332\354\354\377n\244\245"
  "\377\0\0\0\377\0\0\0\231\0\0\0""2\0\0\0\0\0\0\0\0\211\211\211\377OOO"
  "\377\0\0\0&\0\0\0\14\0\0\0\0\0\0\0\0\377\377\377\0\0\0\0\0\0\0\0\0\241"
  "\241\241\377\377\377\377\377---\377\0\0\0r\0\0\0&\0\0\0\0\0\0\0\0\213"
  "\213\213\377\332\355\355\377y\266\266\377\0\0\0\377\0\0\0\231\0\0\0""2"
  "\0\0\0\0\0\0\0\0\213\213\213\377\377\377\377\377\0\0\0\377\0\0\0\77\0"
  "\0\0\14\0\0\0\0\377\377\377\0\0\0\0\0\245\245\245\377\377\377\377\377"
  "\377\377\377\377'''\377###\377OOO\377zzz\377\213\213\213\377ttt\377\327"
  "\353\353\377\201\300\300\377\0\0\0\377\17\17\17\377HHH\377vvv\377\211"
  "\211\211\377uuu\377\377\377\377\377\361\361\361\377\0\0\0\377\0\0\0\77"
  "\0\0\0\14\377\377\377\0\205\205\205\377\346\362\362\377\340\360\357\377"
  "\324\352\352\377\315\347\346\377\315\347\347\377\323\351\351\377\331"
  "\354\355\377\332\355\355\377\327\354\353\377\322\350\350\377\177\277"
  "\300\377\305\337\337\377\314\346\347\377\324\351\352\377\331\354\354"
  "\377\332\355\355\377\327\354\353\377\316\346\346\377\254\303\302\377"
  "\212\234\234\377\0\0\0\377\0\0\0\77\377\377\377\0BBB\377\233\316\316"
  "\377\216\307\307\377z\266\266\377j\240\237\377b\223\224\377d\227\226"
  "\377m\245\244\377y\266\266\377\201\301\300\377\177\277\300\377u\261\261"
  "\377i\235\236\377b\223\224\377d\226\226\377n\244\245\377x\265\265\377"
  "{\271\271\377o\247\247\377T~~\377@``\377\0\0\0\377\0\0\0\177\377\377"
  "\377\0\0\0\0&\0\0\0\377.\211\211\377'vv\377\0\0\0\377\0\0\0\377\0\0\0"
  "\377\0\0\0\377\0\0\0\377\0\0\0\377\305\340\340\377i\236\235\377\0\0\0"
  "\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377#ff\377\32MM\377"
  "\0\0\0\377\0\0\0\277\0\0\0\177\377\377\377\0\0\0\0\14\0\0\0\77\0\0\0"
  "\377\0QQ\377\0\0\0\377\0\0\0\277\0\0\0\246\0\0\0\231\0\0\0\231\17\17"
  "\17\377\314\346\347\377b\224\224\377\0\0\0\377\0\0\0\277\0\0\0\246\0"
  "\0\0\231\0\0\0\231\0\0\0\377\0CC\377\0\0\0\377\0\0\0\277\0\0\0\214\0"
  "\0\0\77\377\377\377\0\0\0\0\0\0\0\0\14\0\0\0\77\0\0\0\377\0\0\0\377\0"
  "\0\0\246\0\0\0Y\0\0\0""2\0\0\0""2HHH\377\323\351\351\377d\226\226\377"
  "\0\0\0\377\0\0\0\246\0\0\0Y\0\0\0""2\0\0\0""2\0\0\0\377\0\0\0\377\0\0"
  "\0\277\0\0\0\214\0\0\0\77\0\0\0\14\377\377\377\0\0\0\0\0\0\0\0\0\0\0"
  "\0\14\0\0\0\77\0\0\0\177\0\0\0r\0\0\0&\0\0\0\0\0\0\0\0vvv\377\331\354"
  "\354\377m\244\245\377\0\0\0\377\0\0\0\231\0\0\0""2\0\0\0\0\0\0\0\0\0"
  "\0\0&\0\0\0r\0\0\0\177\0\0\0\77\0\0\0\14\0\0\0\0\377\377\377\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\14\0\0\0&\0\0\0&\0\0\0\14\0\0\0\0\0\0\0\0\211"
  "\211\211\377\332\355\355\377x\265\265\377\0\0\0\377\0\0\0\231\0\0\0""2"
  "\0\0\0\0\0\0\0\0\0\0\0\14\0\0\0&\0\0\0&\0\0\0\14\0\0\0\0\0\0\0\0\377"
  "\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\211"
  "\211\211\377\213\213\213\377uuu\377\327\353\353\377{\270\271\377\0\0"
  "\0\377\0\0\0\377\0\0\0\377\0\0\0&\0\0\0\14\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0OOO\377\377\377\377\377\377\377\377\377\316\346"
  "\346\377o\247\247\377\"gf\377\0CC\377\0\0\0\377\0\0\0r\0\0\0&\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0&\0\0\0\377\361\361\361"
  "\377\254\302\302\377T~~\377\32MM\377\0\0\0\377\0\0\0\277\0\0\0\177\0"
  "\0\0&\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\14\0\0\0"
  "\77\0\0\0\377\212\233\233\377@``\377\0\0\0\377\0\0\0\277\0\0\0\214\0"
  "\0\0\77\0\0\0\14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377"
  "\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\14\0\0\0\77\0\0\0\377\0\0\0\377\0\0\0\277\0\0\0\214\0\0"
  "\0\77\0\0\0\14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0\0\0\0\0\0\0\0\0\0\0\0\0\14\0\0\0\77\0\0\0\177\0\0\0\177\0\0\0\77\0"
  "\0\0\14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
  "\0"};

void attach_icon(const char *toolbutton_name, const char *filename)
{
     GtkWidget *img,*button = lookup_widget ( main_window, toolbutton_name );
     GdkPixbuf *pixbuf;
     static int i=0;
     GError *error = NULL;

     if (!button)
        Publisher::warn ( std::string("Code bug:\nattach_icon() called with an invalid widget name\n")+toolbutton_name );
     else
     {
          img = gtk_image_new_from_file(filename);

         // img = create_pixmap (main_window,filename);

         // img = gtk_image_new();
         // gtk_image_set_from_file(GTK_IMAGE(img),filename);
/*
         if (i++)
            pixbuf = gdk_pixbuf_new_from_inline(-1,icon_move,false,NULL);
         else
             pixbuf = gdk_pixbuf_new_from_file("c:\\select.png",&error);

         if (!pixbuf)
         {
                     printf("Shan't! (%s)\n",error->message);
                     return;
         }
         
         img = gtk_image_new_from_pixbuf(pixbuf);
*/
         if (!img)
         {
                     printf("Shan't! (2)\n");
                     return;
         }

         gtk_widget_show(img);
         gtk_tool_button_set_icon_widget (GTK_TOOL_BUTTON (button), img);
     }
}

void fill_toolbar_with_icons()
{
     attach_icon("move_mode_radiotoolbutton","editor_icons\\move.png");
     attach_icon("select_mode_radiotoolbutton","editor_icons\\select.png");
}

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

struct scenery_name
{
       static void set(const char *n)
            { name=n; a=1; }
       static const char *get()
             { return name.c_str(); }
       static int assigned()
             { return a; }
       static void clear()
             { a=0; }

       private:
               static std::string name;
               static int a;
};

std::string scenery_name::name;
int scenery_name::a=0;

void open_request()
{
     struct handler
     {
            static void file_activated ( GtkFileChooser *filechooser, gpointer data )
            {
                   printf("activated\n");
                   gchar *full_name = gtk_file_chooser_get_filename(filechooser);
                   printf("activated %s\n",full_name);
                   if ( ! ( Editor::instance ()->loadFromFile ( full_name ) ) )
                      Publisher::warn("Load failed,\ncheck the log for details");
                   else
                       scenery_name::set(full_name);
                   gtk_widget_destroy(GTK_WIDGET(filechooser));
            }
     };

  GtkWidget *fcd = gtk_file_chooser_dialog_new (_("Scenery"), NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL);
  gtk_window_set_modal(GTK_WINDOW(fcd),TRUE);
  g_signal_connect ( (gpointer) fcd, "file_activated",
                   G_CALLBACK (handler::file_activated), NULL );

  GtkFileFilter *filter = gtk_file_filter_new();
  g_object_ref(G_OBJECT(filter));
  gtk_file_filter_add_pattern(filter, "*.bscn");
  gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(fcd),filter);

  gtk_widget_show(fcd);
}

void save_as_request(void(*after_call)(void)=NULL)
{
     struct handler
     {
            static void file_activated ( GtkFileChooser *filechooser, void(*after_call)(void) )
            {
                   gchar *full_name = gtk_file_chooser_get_filename(filechooser);
                   if ( ! ( Editor::instance ()->saveToFile ( full_name ) ) )
                      Publisher::warn("Save failed,\ncheck the log for details");
                   else
                   {
                       scenery_name::set(full_name);
                       if (after_call)
                          after_call();
                   }
                   gtk_widget_destroy(GTK_WIDGET(filechooser));
            }

            static void response  (GtkDialog       *dialog,
                                   gint             response_id,
                                   void             (*after_call)(void))
            {
                   std::string full_name;
                   switch (response_id)
                   {
                          case GTK_RESPONSE_OK:
                               full_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
                               printf("wpisano %s\n",full_name.c_str());
                               if (full_name.substr(full_name.length()-std::string(".bscn").length())!=".bscn")
                                  full_name+=".bscn";
                               printf("zapis do %s\n",full_name.c_str());
                               if ( ! ( Editor::instance ()->saveToFile ( full_name.c_str() ) ) )
                                  Publisher::warn("Save failed,\ncheck the log for details");
                               else
                               {
                                   if (after_call)
                                       after_call();
                                   else
                                       scenery_name::set(full_name.c_str());
                               }
                          case GTK_RESPONSE_CANCEL:
                               gtk_widget_destroy(GTK_WIDGET(dialog));

                          /* przy zakomentowanym wyskakuje dialog z niedopasowanym rozmiarem: */
                          case GTK_RESPONSE_DELETE_EVENT:
                               break;

                          default:
                                  Publisher::warn("Code bug:\nUnknown response from the 'save-as dialog'");

                   }
            }
     };

  GtkWidget *fcd = gtk_file_chooser_dialog_new (_("Scenery"), NULL, GTK_FILE_CHOOSER_ACTION_SAVE,
                                               GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				                               GTK_STOCK_SAVE, GTK_RESPONSE_OK,
                                               NULL);

  gtk_window_set_modal(GTK_WINDOW(fcd),TRUE);
  g_signal_connect ( (gpointer) fcd, "file_activated",
                   G_CALLBACK (handler::file_activated),
                   (gpointer)after_call );
  
  g_signal_connect ((gpointer) fcd, "response",
                    G_CALLBACK (handler::response),
                    (gpointer)after_call);

  GtkFileFilter *filter = gtk_file_filter_new();
  g_object_ref(G_OBJECT(filter));
  gtk_file_filter_add_pattern(filter, "*.bscn");
  gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(fcd),filter);

  gtk_widget_show(fcd);
}

void save_request(void(*after_call)(void)=NULL)
{
     if (!scenery_name::assigned())
     {
        save_as_request(after_call);
        return;
     }

     if ( ! ( Editor::instance ()->saveToFile ( scenery_name::get() ) ) )
        Publisher::warn("Save failed\ncheck the log for details");
     else
         if (after_call)
            after_call();
}

void clearLock_and_quit()
{
     Undo::clear_lock();
     gtk_main_quit();
}

gboolean autosave_callback(gpointer data)
{
     if ( ! ( Editor::instance ()->saveToFile ( AUTOSAVE_FILENAME ) ) )
          Publisher::warn("Autosave failed,\ncheck the log for details");
     return TRUE;
}

void
init_interface (int *argcp, char ***argvp)
{
printf("init_interface started\n");
  GL_realizer::gl_init (argcp, argvp);

  main_window = create_main_window ();
  Publisher::init (main_window);
  fill_toolbar_with_icons();
  
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

  new_dialog = create_new_dialog();
  gtk_window_set_transient_for (GTK_WINDOW (new_dialog),
				GTK_WINDOW (main_window));

  GtkWidget *fcd = gtk_file_chooser_dialog_new (_("Choose a file"), NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL);

printf("Wins created, not shown\n"); 


#ifdef EDITOR_FULL
  osg::ref_ptr < TopView > top =
    new TopView (lookup_widget (main_window, "main_drawingarea"));
  osg::ref_ptr < PerspectiveView > perspective =
    new PerspectiveView (lookup_widget (window2, "drawingarea2"));
  static Editor editor (top.get ());
  editor.addPerspectiveView (perspective.get ());
  perspective->
    unsetNodeMask (Editor::Masks::nm_Nodes | Editor::Masks::nm_Maps);
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
  kit[0].addTextEntry ("Janowski", "");
  kit[0].addTextEntry ("Zalewski", "");
  kit[0].addButton ("Borowski", 1, 0);
  // kit[0].addFileSel ("Majewski", "C:\\WINDOWS\\Desktop","*.*","Zielinski");
  std::vector<std::string> p, e;
  p.push_back(std::string("*.*"));
  /*
  e.push_back(std::string("emb #1"));
  e.push_back(std::string("emb #2"));
  e.push_back(std::string("emb #3"));
  */
  kit[0].addFileSel ("Majewski", "C:\\WINDOWS\\Desktop",p,"Zielinski",3,true,e);
  static Kit::Item gajewski_item = kit[0].addButton ("Gajewski", 1, 1);
  // gajewski_item.switching();

  // kit[1].activate();
  gtk_timeout_add (7000, to_handler, kit);
  //gtk_timeout_add (1000, gajewski_h, &kit[0]);

#endif

  Undo::init ( lookup_widget(main_window,"undo_toolbutton"),
             lookup_widget(main_window,"record_toolbutton") );
             
  g_timeout_add(AUTOSAVE_DELAY_SECONDS*1000,autosave_callback,NULL);
};


void new_request()
{
     gtk_widget_show(new_dialog);
}


void
on_new_toolbutton_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
  new_request();
}


void
on_open_toolbutton_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
  open_request();
}


void
on_save_toolbutton_clicked (GtkToolButton * toolbutton, gpointer user_data)
{
  save_request();
}


void
on_new_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  new_request();
}


void
on_open_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  open_request();
}


void
on_save_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  save_request();
}


void
on_save_as_mi_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  save_as_request();
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
  // save_request();

  Editor::instance ()->exportToDirectory (edOptions::instance()->exportDir.c_str());
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
    case GDK_r:
    case GDK_R:
      if (GTK_WIDGET_IS_SENSITIVE(lookup_widget(main_window,"record_toolbutton")))
         Undo::store();
      break;
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

/*
  i = event->keyval;
  // printf ("code: %d\n",i);

  for (int i = 0; i < 32; i++)
    printf ("%d%s", !!(event->state & (1 << i)), i == 31 ? "\n" : "");
*/

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
            gtk_widget_hide(GTK_WIDGET(dialog));
			save_request(clearLock_and_quit);
			return;

       case GTK_RESPONSE_NO:
            clearLock_and_quit();

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


void clear_scenery()
{
  Editor::instance ()->freeNodes ();
  scenery_name::clear();
}


void
on_new_dialog_response                 (GtkDialog       *dialog,
                                        gint             response_id,
                                        gpointer         user_data)
{
   switch(response_id)
   {
       case GTK_RESPONSE_YES:
            // trza zapisac:
            gtk_widget_hide(GTK_WIDGET(dialog));
			save_request(clear_scenery);
			return;

       case GTK_RESPONSE_NO:
            clear_scenery();

       case GTK_RESPONSE_CANCEL:
            gtk_widget_hide(GTK_WIDGET(dialog));

       case GTK_RESPONSE_DELETE_EVENT:
            return;

       default:
            Publisher::warn("Code bug:\nUnknown response from the 'new dialog'");
   }
}

void
on_createMiscLines_mode_radiotoolbutton_toggled
                                        (GtkToggleToolButton *toggletoolbutton,
                                        gpointer         user_data)
{

}


void
on_undo_toolbutton_clicked             (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
   Undo::perform();
}

void
on_record_toolbutton_clicked           (GtkToolButton   *toolbutton,
                                        gpointer         user_data)
{
   Undo::store();
}

void
on_merge_mi_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

