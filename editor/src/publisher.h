#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP 1
#include <gtk/gtk.h>
#include <string>

#define E_DEBUG 0

class Publisher
{
    public:
        static void init(GtkWidget* main_window);

	    static void err(const char*);
	    static void err(std::string);
	    static void warn(const char*);
	    static void warn(std::string);
	    static int set_status_msg(int,const char *);

    private:
	    static GtkWidget *err_window;
	    static GtkWidget *warn_window;
};

#endif

