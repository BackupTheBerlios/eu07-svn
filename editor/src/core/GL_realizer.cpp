#include "GL_realizer.h"
#include "../interface.h"
#include "../support.h"
#include "../publisher.h"

#include <stdio.h>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#define DEBUG 1

int GL_realizer::gl_inited = 0;
GdkGLConfig *GL_realizer::gl_config= NULL;
GdkGLContext *GL_realizer::shareContext= NULL;

void GL_realizer::gl_init(int *argc, char ***argv)
{
    if (DEBUG) printf("entered GL_realizer::gl_init\n");
    gdk_gl_init(argc,argv);
    create_GL_config();
    create_GL_context();
    if (DEBUG) printf("leaving GL_realizer::gl_init\n");
}

GL_realizer::GL_realizer ( GtkWidget *_dr_area ) : dr_area(_dr_area)
{
    if (DEBUG) printf("entered GL_realizer::GL_realizer\n");

    static const char* gl_key = "gl-adapted";

    if (!gl_inited)
    {
        Publisher::err("Code bug:\ngl_init call required");
        return;
    }
    
    if (!dr_area)
    {
        Publisher::err("Code bug:\nNULL widget passed to GL_realizer constructor");
    }    

    GtkWidget *toplevel = gtk_widget_get_toplevel ( dr_area );

    GdkColormap *cm = gdk_gl_config_get_colormap ( gl_config );

    /*
        W oknie toplevel ustawiamy mape kolorow (GdkColormap).
        Jezeli okno jest REALIZED to czynnosc jest niewykonalna,
        zatem w takim przypadku sprawdzamy, czy mapa zostala ustawiona
        wczesniej. Jezeli okno nie jest REALIZED to ustawiamy mape
        i oznaczamy w slowniku poprzez klucz 'gl_key'.
    */
    if ( !gtk_object_get_data ( GTK_OBJECT(toplevel), gl_key ) )
        if ( GTK_WIDGET_REALIZED(toplevel) )
                Publisher::err("Code bug:\nCreating GL_realizer for a drawingarea attached\nto realized window is not allowed.");
        else
        {
                gtk_widget_set_colormap ( toplevel, cm );
                gtk_object_set_data ( GTK_OBJECT(toplevel), gl_key, NULL );
        }

    gtk_widget_set_colormap ( dr_area, cm );

    gtk_widget_set_gl_capability (dr_area,
                                  gl_config,
                                  shareContext,
                                  TRUE,
                                  GDK_GL_RGBA_TYPE /*currently unused?*/);

    gtk_widget_set_events ( dr_area, 
                GDK_EXPOSURE_MASK |
                GDK_POINTER_MOTION_MASK | 
                GDK_POINTER_MOTION_HINT_MASK | 
                GDK_BUTTON_MOTION_MASK | 
                GDK_BUTTON1_MOTION_MASK | 
                GDK_BUTTON2_MOTION_MASK | 
                GDK_BUTTON3_MOTION_MASK | 
                GDK_BUTTON_PRESS_MASK | 
                GDK_BUTTON_RELEASE_MASK | 
                GDK_KEY_PRESS_MASK | 
                GDK_KEY_RELEASE_MASK | 
                GDK_ENTER_NOTIFY_MASK | 
                GDK_LEAVE_NOTIFY_MASK | 
                GDK_FOCUS_CHANGE_MASK | 
                GDK_STRUCTURE_MASK | 
                GDK_PROPERTY_CHANGE_MASK | 
                GDK_VISIBILITY_NOTIFY_MASK | 
                GDK_PROXIMITY_IN_MASK | 
                GDK_PROXIMITY_OUT_MASK
                );

    gtk_widget_show ( dr_area );

    g_signal_connect ( G_OBJECT (dr_area), "configure_event",
//    g_signal_connect ( G_OBJECT (dr_area), "size_allocate",
                       G_CALLBACK (sizealloc_callback), this );
    g_signal_connect ( G_OBJECT (dr_area), "expose_event",
	                   G_CALLBACK (expose_callback), this );
    g_signal_connect ( G_OBJECT (dr_area), "button_press_event",
                       G_CALLBACK (button_callback), this );
    g_signal_connect ( G_OBJECT (dr_area), "button_release_event",
                       G_CALLBACK (button_callback), this);
    g_signal_connect ( G_OBJECT (dr_area), "motion_notify_event",
                       G_CALLBACK (motion_callback), this );
    g_signal_connect ( G_OBJECT (dr_area), "enter_notify_event",
                       G_CALLBACK (enter_notify_callback), this );
    g_signal_connect ( G_OBJECT (dr_area), "leave_notify_event",
                       G_CALLBACK (leave_notify_callback), this );
    g_signal_connect ( G_OBJECT (dr_area), "scroll_event",
                       G_CALLBACK (scroll_callback), this );

    gtk_widget_set_double_buffered ( dr_area, FALSE );

    if (DEBUG) printf("leaving GL_realizer::gl_init\n");
}

void GL_realizer::create_GL_config()
{
    if (DEBUG) printf("entered GL_realizer::create_GL_config\n");

   static const int gl_cfg[] = {
      GDK_GL_DOUBLEBUFFER,
      GDK_GL_RGBA,
      GDK_GL_RED_SIZE,        1,
      GDK_GL_GREEN_SIZE,      1,
      GDK_GL_BLUE_SIZE,       1,
      GDK_GL_DEPTH_SIZE,      12,
      GDK_GL_ATTRIB_LIST_NONE
      };

    gl_config = gdk_gl_config_new(&gl_cfg[0]);

	if (!gl_config)
    {
        gl_config = gdk_gl_config_new(&gl_cfg[1]);
        if (gl_config)
            Publisher::warn("Can not create double buffered GL-visual\n(using single)");
        else
            Publisher::err("Can not create GL-visual");
    }    
    gl_inited = 1;

    if (DEBUG) printf("leaving GL_realizer::create_GL_config\n");
}

void GL_realizer::create_GL_context()
{
    if (DEBUG) printf("entered GL_realizer::create_GL_context\n");
    GtkWidget
        *window = create_ghost_window(),
        *area = lookup_widget(window,"drawingarea");

    gtk_widget_set_colormap ( area, gdk_gl_config_get_colormap ( gl_config ) );

    gtk_widget_set_gl_capability ( area,
                                   gl_config,
                                   NULL,
                                   TRUE,
                                   GDK_GL_RGBA_TYPE );

    gtk_widget_realize(area);

	shareContext = gtk_widget_get_gl_context (area);

    if (DEBUG) printf("leaving GL_realizer::create_GL_context\n");
}

bool GL_realizer::begin_gl()
{
    if (DEBUG) printf("entered GL_realizer::begin_gl\n");

   // printf("Begin gl\n");
   GdkGLContext *glc = gtk_widget_get_gl_context ( dr_area );
   GdkGLDrawable *gld = gtk_widget_get_gl_drawable ( dr_area );
   // printf("Begin gl %d %d\n",glc,gld);
   if ( !gdk_gl_drawable_gl_begin(gld,glc) )
   {
      Publisher::err("Can not begin gl"); 
	  return false;
   }

   if (DEBUG) printf("leaving GL_realizer::begin_gl\n");
   return true;
}

void GL_realizer::end_gl()
{
   if (DEBUG) printf("entered GL_realizer::end_gl\n");
   GdkGLDrawable *gld = gtk_widget_get_gl_drawable ( dr_area );
   gdk_gl_drawable_gl_end(gld);
   if (DEBUG) printf("leaving GL_realizer::end_gl\n");
}

void GL_realizer::commit_gl()
{
    if (DEBUG) printf("entered GL_realizer::commit_gl\n");
    GdkGLDrawable *gld = gtk_widget_get_gl_drawable ( dr_area );
    if ( gdk_gl_drawable_is_double_buffered(gld) )
        gdk_gl_drawable_swap_buffers(gld);
    else
        glFlush();
    if (DEBUG) printf("leaving GL_realizer::commit_gl\n");
}
