/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#include "txt.h"

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"

#ifdef G_OS_WIN32
char *package_prefix;
char *package_datadir;
#endif

int
main (int argc, char *argv[])
{
  GtkWidget *filechooserdialog1;

#ifdef G_OS_WIN32
  gchar *temp;

  package_prefix = g_win32_get_package_installation_directory (NULL, NULL);
  package_datadir = g_strdup_printf ("%s%s", package_prefix, "/share");
#endif

#ifdef ENABLE_NLS
#ifdef G_OS_WIN32
  temp = g_strdup_printf ("%s%s", package_prefix, "/lib/locale");
  bindtextdomain (GETTEXT_PACKAGE, temp);
  g_free (temp);
#else
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
#endif
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);

#ifdef G_OS_WIN32
  temp = g_strdup_printf ("%s/%s%s", package_datadir, PACKAGE, "/pixmaps");
  add_pixmap_directory (temp);
  g_free (temp);
#else
  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");
#endif


  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  filechooserdialog1 = create_filechooserdialog1 ();
  gtk_widget_show (filechooserdialog1);
  g_signal_connect ((gpointer) filechooserdialog1, "destroy", G_CALLBACK(gtk_main_quit),
                    NULL);
wysypujacy_text();
  gtk_main ();

#ifdef G_OS_WIN32
  g_free (package_prefix);
  g_free (package_datadir);
#endif
  return 0;
}
#ifdef _MSC_VER
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	return main (__argc, __argv);
}
#endif

