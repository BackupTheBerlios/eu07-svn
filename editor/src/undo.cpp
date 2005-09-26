#include "undo.h"
#include <gtk/gtk.h>
#include <string>
#include "Publisher.h"
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

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


const char *LOCK_NAME="editor.lock";
const char *TMP_DIR_NAME="editor_tmp";
const int MAX_FILES=15;

int najstarszy=0;
int najmlodszy=-1;

GtkWidget *button_undo=NULL, *button_record=NULL;

void Undo::init(GtkWidget *undo, GtkWidget *record)
{
   // 0) sprawdzic button

      if (button_undo || button_record)
      {
          Publisher::warn("Code bug:\nmultiple calls of Undo::init");
          return;
      }

      if ( ! ( undo && record ) )
      {
          Publisher::warn("Code bug:\nNULL button given to Undo::init");
          return;
      }

   // 1) zalozyc blokade

      std::string name=TMP_DIR_NAME;
      name+=G_DIR_SEPARATOR_S;
      name+=LOCK_NAME;

      int fd;

      while (1)
      {
         fd = open ( name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR );
         if (fd==-1)
            if (errno==EINTR)
               continue;
            else
            {
                if (errno==EEXIST)
                   Publisher::warn("Lock file already exists\nUndo function will be disabled");
                else
                   Publisher::warn ( std::string("Can not create lock file,\n(") +
                                     std::string(sys_errlist[errno]) +
                                     std::string(")\nUndo function will be disabled") );
                return;
            }
         else
             break; /* OK */
      }

      while (1)
      {
          if (close(fd)==0)
              break;

          if (errno!=EINTR)
          {
              Publisher::warn( std::string("Close on the lock file failed,\n") +
                               sys_errlist[errno] );
              break;
          }
      }

   // 2) wyczyscic pliki tymczasowe

      GDir *tmp_dir = g_dir_open("editor_tmp",0,NULL);
      const char *p;

      while (p=g_dir_read_name(tmp_dir))
          if (strcmp(p,LOCK_NAME))
              remove ( (std::string(TMP_DIR_NAME)+G_DIR_SEPARATOR_S+p).c_str() );

      g_dir_close(tmp_dir);

   // 3) wlaczyc button record

       gtk_widget_set_sensitive(button_record=record,TRUE);
       gtk_widget_set_sensitive(button_undo=undo,FALSE);
}

std::string tmp_name(int n)
{
    char buf[sizeof(n)*5+9];
    sprintf(buf,"%d",n);
    std::string name = TMP_DIR_NAME;
    name=name+G_DIR_SEPARATOR_S+"tmp"+buf+".bscn";
    return name;
}

void Undo::perform()
{
     if ( ! ( button_undo && button_record ) )
     {
          Publisher::warn("Code bud:\nUndo button enabled before init call");
          return;
     }

    if ( najmlodszy - najstarszy < 0 )
    {
         Publisher::warn("Code bug:\nUndo button clicked when no tmp files were available");
         return;
    }

    printf ( "Odczyt z %s\n", tmp_name(najmlodszy).c_str() );
    if ( ! ( Editor::instance()->loadFromFile(tmp_name(najmlodszy).c_str()) ) )
    {
         Publisher::err ( std::string("") + 
                          "Could not load temporary file, current scenery is damaged\n" +
                          "check the log for details,\n" +
                          "look for earlier temporary files in " +
                          TMP_DIR_NAME + "directory,\n" +
                          "copy them if may be invaluable,\n"
                          "click OK to exit"
                          );
    }

    najmlodszy--;

    if ( najmlodszy - najstarszy < 0 )
       gtk_widget_set_sensitive(button_undo,FALSE);
}

void Undo::store()
{
     if ( ! ( button_undo && button_record ) )
     {
          Publisher::warn("Code bug:\nStore button enabled before init call");
          return;
     }

     if ( najmlodszy - najstarszy + 1 >= MAX_FILES )
     { // skasowac najstarszy
       printf("Kasujemy %s\n",tmp_name(najstarszy).c_str());
        if ( remove ( tmp_name(najstarszy).c_str() ) )
           Publisher::warn ( std::string("Temporary file (undo) removing problem:\n")
                             + sys_errlist[errno] );
        najstarszy++;
     }

     if ( Editor::instance()->saveToFile(tmp_name(najmlodszy+1).c_str()) )
     {
        printf ( "Zapis do %s\n", tmp_name(najmlodszy+1).c_str() );
        najmlodszy++;
        gtk_widget_set_sensitive(button_undo,TRUE);
     }
     else
        Publisher::warn("Store failed,\ncould not save temporary file (undo),\ncheck the log for details");
}

void Undo::clear_lock()
{
    if ( ! ( button_undo && button_record ) )
       return;

    button_undo = button_record = NULL;

    remove ( (std::string(TMP_DIR_NAME)+G_DIR_SEPARATOR_S+LOCK_NAME).c_str() );
}
