#include <gtk/gtk.h>

class Undo
{
   public:
      static void init(GtkWidget *undo, GtkWidget *record);
      static void perform();
      static void store();
      static void clear_lock();
   private:
      Undo();
};
