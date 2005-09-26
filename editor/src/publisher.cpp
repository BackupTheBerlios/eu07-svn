#include "publisher.h"
#include "support.h"
#include "interface.h"
#include <gtk/gtk.h>

#include <stdlib.h>

GtkWidget* Publisher::err_window;
GtkWidget* Publisher::warn_window;
GtkWidget* _main_window;

void Publisher::init(GtkWidget* main_window)
{
    static int allow = 1;
    if (!allow)
    {
        err("Code bug:\nRepeated Publisher::init");
        return;
    }
    
    allow = 0;

    err_window = create_error_window();
    warn_window = create_warning_window();
    _main_window = main_window;

    GtkWidget *hbox = lookup_widget(main_window,"status_hbox");
    if (!hbox)
       warn("Code bug:\nInaccessible status_hbox\nStatus msgs will not appear probably");
/*
    gtk_statusbar_push ( 
        GTK_STATUSBAR ( left_sb = lookup_widget(main_window,"left_statusbar") ),
        0, "" );
*/
}

void Publisher::err(const char *msg) 
{
    gtk_label_set_text ( GTK_LABEL ( lookup_widget(err_window,"label") ), msg );
    gtk_widget_show(err_window);
    gtk_main();
    gtk_main_quit();
};

void Publisher::err(std::string msg) 
{
    err(msg.c_str()); 
}

void Publisher::warn(const char *msg) 
{
    gtk_label_set_text ( GTK_LABEL ( lookup_widget(warn_window,"label") ), msg );
    gtk_widget_show(warn_window);
};

void Publisher::warn(std::string msg) 
{
     warn(msg.c_str());
}

int Publisher::set_status_msg(int sbar_num,const char *msg)
{
    GtkWidget *sb=NULL;
    char buf[2048];

    sprintf(buf,"n%d_statusbar", sbar_num);
    sb = lookup_widget(_main_window,buf);

    if (!sb)
    {
        warn("Code bug:\nInvalid sbar_num in Publisher::set_status_msg\nMessage abandoned");
        return -1;
    };    

    gtk_statusbar_push(GTK_STATUSBAR(sb),0,msg);
    /*
    gtk_widget_queue_draw(GTK_STATUSBAR(sb)->label);
    gtk_widget_queue_draw(GTK_STATUSBAR(sb));
    */
    gtk_widget_draw(sb,NULL);
    
    return 0;
}
