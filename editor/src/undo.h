#include <gtk/gtk.h>

class Undo
{
   public:
      // static void init(GtkWidget *undo, GtkWidget *record);
      static void init(GtkWidget *undo, GtkWidget *redo);
      // static void perform();
      // static void store();
      // static void clear_lock();
      typedef enum { UNDO_NONE, UNDO_BOTH, UNDO_ONLY_U, UNDO_ONLY_R } UndoBtStat;
      static void set_status ( UndoBtStat s );
   private:
      Undo();
};

