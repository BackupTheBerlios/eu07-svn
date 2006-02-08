#ifndef GL_REALIZER_HPP
#define GL_REALIZER_HPP 1

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../publisher.h"

class MouseCoords
{
public:
	MouseCoords() : x(0), y(0) {};
	MouseCoords(double _x, double _y) : x(_x), y(_y) {};
	void set(double _x, double _y) { x=_x; y=_y; };
	double x,y;
};

class GL_realizer
{
    public:
        GL_realizer ( GtkWidget* dr_area );
		virtual ~GL_realizer ( ) {};
        static void gl_init ( int *argc, char ***argv );

    protected:
        bool begin_gl();
        void commit_gl();
        void end_gl();
        GtkAllocation *get_area_alloc() 
                { return &dr_area->allocation; }; /* int x,y,width,height */

		virtual gboolean button ( GdkEventButton * ) = 0;
        virtual gboolean motion ( GdkEventMotion * ) = 0;
        virtual void resized ( ) = 0;
        virtual gboolean exposed ( ) = 0;
        virtual gboolean enter_notify ( GdkEventCrossing * ) = 0;
        virtual gboolean leave_notify ( GdkEventCrossing * ) = 0;
        virtual gboolean scroll ( GdkEvent * ) = 0;

		inline double getMouseDX() { return mc0.x-mc1.x; };
		inline double getMouseDY() { return mc1.y-mc0.y; };

    private:
        GL_realizer ( ) {};
        GtkWidget *dr_area; 
        static GdkGLConfig *gl_config;    // tworzone 1 raz, na poczatku
        static void create_GL_config ( ); // wywolywane w gl_init(...);
        static int gl_inited;             // czy nastapilo gl_init(...);
		static GdkGLContext *shareContext;
		static void create_GL_context ( );

        static void sizealloc_callback ( GtkWidget *w, GtkAllocation *a, gpointer p )
                { /*printf("resize callback\n"); _sleep(3333);*/
                    ((GL_realizer*) p) -> resized(); };

        static gboolean expose_callback ( GtkWidget *w, GtkAllocation *a, gpointer p )
                { return ((GL_realizer*) p) -> exposed(); };

        static gboolean button_callback ( GtkWidget *w, GdkEventButton *e, gpointer p )
				{ ((GL_realizer*) p) -> flushMouseCoords(e->x,e->y); 
				return ((GL_realizer*) p) -> button(e); };
                
        static gboolean motion_callback ( GtkWidget *w, GdkEventMotion *e, gpointer p )
                { ((GL_realizer*) p) -> updateMouseCoords(e->x,e->y); 
				return ((GL_realizer *) p) -> motion(e); };
				
        static gboolean enter_notify_callback ( GtkWidget *w, GdkEventCrossing *e, gpointer p )
                { ((GL_realizer*) p) -> flushMouseCoords(e->x,e->y);
				return ((GL_realizer *) p) -> enter_notify(e); };

        static gboolean leave_notify_callback ( GtkWidget *w, GdkEventCrossing *e, gpointer p )
                { return ((GL_realizer *) p) -> leave_notify(e); };
                
        static gboolean scroll_callback ( GtkWidget *w, GdkEvent *e, gpointer p )
                { return ((GL_realizer *) p) -> scroll(e); };

		void flushMouseCoords(double x, double y) { mc0.set(x,y); mc1.set(x,y); };
		void updateMouseCoords(double x, double y) { mc1=mc0; mc0.set(x,y); };
	protected:
		MouseCoords mc0,mc1;
};

#endif

