#include "property_controls.h"
#include "support.h"
#include "publisher.h"
#include "core/PropertySet.h"

#include <string>
#include <vector>

#define DEBUG 0

GtkBox *Kit::external_widget;
GtkWidget *Kit::active=NULL;

const char 
    *OWNER_KEY =            "owner",
    *PROP_LABEL_KEY =       "prop-label",
    *WRITE_FUNC_KEY =       "write-func",
    *READ_FUNC_KEY =        "read-func",
    *VISIBILITY_KEY =       "visibility";

typedef enum { ERROR_VISIBILITY=0, MANUAL_VISIBILITY, SWITCHING_VISIBILITY };

void my_gstring_free(gpointer gstring)
{
     g_string_free((GString*)gstring,TRUE);
}

Kit::Kit() : elems(0)
{
    if (DEBUG) printf("entering Kit::Kit\n");
    if (!external_widget)
        Publisher::err("Code bug:\nInaccessible container for property kits");
    kit_widget = GTK_BOX ( gtk_vbox_new (FALSE, 0) );
    gtk_box_pack_start (GTK_BOX(external_widget), GTK_WIDGET(kit_widget), FALSE, FALSE, 0);
    setOwner(NULL);
    if (DEBUG) printf("leaving Kit::Kit\n");
}

void Kit::activate()
{
    if (DEBUG) printf("entering Kit::activate\n");
    if (active)
        gtk_widget_hide(active);
    gtk_widget_show(active=GTK_WIDGET(kit_widget));
    if (DEBUG) printf("leaving Kit::activate\n");
}

void Kit::disable()
{ // static
    if (DEBUG) printf("entering Kit::disable\n");
    if (active)
        gtk_widget_hide(active);
    active = NULL;
    if (DEBUG) printf("leaving Kit::disable\n");
}

//changed_callback wywoluje sie przy setOwnerWithRead,
//szybki hack aby nie zapisywal property
static int disableOnChange= false; //MW - HACK :)

void Kit::setOwnerWithRead(PropertySet *set)
{
    if (DEBUG) printf("entering Kit::sOWR\n");
	disableOnChange= true; //MW - HACK :)
    setOwner(set);
    gtk_container_foreach(GTK_CONTAINER(kit_widget), readProperty, set);
	disableOnChange= false; //MW - HACK :)
    if (DEBUG) printf("leaving Kit::sOWR\n");
}

void Kit::setOwnerWithWrite(PropertySet *set)
{
    if (DEBUG) printf("entering Kit::sOWW\n");
    setOwner(set);
    gtk_container_foreach(GTK_CONTAINER(kit_widget), writeProperty, set);
    if (DEBUG) printf("leaving Kit::sOWW\n");
}

void Kit::readProperty(GtkWidget *item, gpointer owner)
{ // static
    if (DEBUG) printf("entering Kit::readProperty\n");

    void (*f)(GObject*) = 
        (void(*)(GObject*)) g_object_get_data(G_OBJECT(item),READ_FUNC_KEY);

    if (!f)
        Publisher::warn("Code bug:\nInaccessible 'read function' in Kit::readProperty\nProperty editor will not function correctly");
    else
        f(G_OBJECT(item));

    if (DEBUG) printf("leaving Kit::readProperty\n");
}

void Kit::writeProperty(GtkWidget *item, gpointer owner)
{ // static
    if (DEBUG) printf("entering Kit::writeProperty\n");

    void (*f)(GObject*) = 
        (void(*)(GObject*)) g_object_get_data(G_OBJECT(item),WRITE_FUNC_KEY);

    if (!f)
        Publisher::warn("Code bug:\nInaccessible 'write function' in Kit::writeProperty\nProperty editor will not function correctly");
    else
        f(G_OBJECT(item));

    if (DEBUG) printf("leaving Kit::readProperty\n");
}

PropertySet* Kit::getOwner(GtkBox *prop_item)
{ // static
    if (DEBUG) printf("entering Kit::getOwner\n");

    GtkWidget *kit_widget = gtk_widget_get_parent(GTK_WIDGET(prop_item));
    PropertySet *owner = (PropertySet*) g_object_get_data(G_OBJECT(kit_widget),OWNER_KEY);
    if (!owner)
        Publisher::warn("Code bug:\nInaccesible owner of properties\nProperty editor will not function correctly");

    if (DEBUG) printf("leaving Kit::getOwner soon\n");

    return (gpointer)owner==(gpointer)kit_widget ? NULL : owner;
}

const char* Kit::getPropName(GtkBox *prop_item)
{ // static
    if (DEBUG) printf("entering Kit::getPropName\n");

    gpointer label = g_object_get_data(G_OBJECT(prop_item),PROP_LABEL_KEY);
    
    if (!label)
    {
        Publisher::warn("Code bug:\nInaccessible property name\nProperty editor will not function correctly");
        return "?";
    }    

    if (DEBUG) printf("leaving Kit::getPropName soon\n");

    return gtk_label_get_text(GTK_LABEL(label));
}    

void Kit::update_visibility ( GtkWidget *prop_item, gpointer data )
{ // static
     if (DEBUG) printf("entering Kit::update_visibility\n");

     PropertySet *owner = getOwner(GTK_BOX(prop_item));
     switch ( (int)g_object_get_data ( G_OBJECT(prop_item), VISIBILITY_KEY ) )
     {
            case SWITCHING_VISIBILITY:
                 (owner?gtk_widget_show:gtk_widget_hide)(prop_item);
                 break;
            case MANUAL_VISIBILITY:
                 break;
            default:
                 Publisher::warn("Code bug:\nInaccessible visibility mode\nProperty editor may show/hide undesirable items");
     }
     
     if (DEBUG) printf("leaving Kit::update_visibility\n");
};


void Kit::setOwner(PropertySet* owner)
{
    if (DEBUG) printf("entering Kit::setOwner\n");
    gpointer ptr = owner ? (gpointer)owner : kit_widget;
    g_object_set_data ( G_OBJECT(kit_widget), OWNER_KEY, ptr );
    gtk_container_foreach(GTK_CONTAINER(kit_widget), update_visibility, NULL);
    if (DEBUG) printf("leaving Kit::setOwner\n");
}

void Kit::Item::shown()
{
     if (DEBUG) printf("entering Kit::Item::shown\n");
     gtk_widget_show(item);
     g_object_set_data ( G_OBJECT(item), VISIBILITY_KEY, (gpointer)MANUAL_VISIBILITY );
     if (DEBUG) printf("leaving Kit::Item::shown\n");
}

void Kit::Item::hidden()
{
     if (DEBUG) printf("entering Kit::Item::hidden\n");
     gtk_widget_hide(item);
     g_object_set_data ( G_OBJECT(item), VISIBILITY_KEY, (gpointer)MANUAL_VISIBILITY );
     if (DEBUG) printf("leaving Kit::Item::hidden\n");
}

void Kit::Item::switching()
{
     if (DEBUG) printf("entering Kit::Item::switching\n");
     g_object_set_data ( G_OBJECT(item), VISIBILITY_KEY, (gpointer)SWITCHING_VISIBILITY );
     update_visibility(item,NULL);
     if (DEBUG) printf("leaving Kit::Item::switching\n");
}

void Kit::registerPropertyItem ( GtkBox *prop_item,
                                 void(*write_prop_func)(GtkBox *),
                                 void(*read_prop_func)(GtkBox *),
                                 const char *prop_name
                                 )
{
    if (DEBUG) printf("entering Kit::rPI\n");
	/*
    if (elems++)
    {
        GtkWidget *hseparator = gtk_hseparator_new ();
        gtk_widget_show (hseparator);
        gtk_box_pack_start ( kit_widget, hseparator, FALSE, TRUE, 4);
    }*/
    GtkWidget *label = gtk_label_new ( prop_name );
    gtk_box_pack_start ( prop_item, label, FALSE, FALSE, 0);
    g_object_set_data ( G_OBJECT(prop_item), PROP_LABEL_KEY, (gpointer)label );
    g_object_set_data ( G_OBJECT(prop_item), WRITE_FUNC_KEY, (gpointer)write_prop_func );
    g_object_set_data ( G_OBJECT(prop_item), READ_FUNC_KEY, (gpointer)read_prop_func );
    g_object_set_data ( G_OBJECT(prop_item), VISIBILITY_KEY, (gpointer)MANUAL_VISIBILITY );
    gtk_box_pack_start ( kit_widget, GTK_WIDGET(prop_item), FALSE, TRUE, 0 );
    //                   container   new child              expand fill  padding
    if (DEBUG) printf("leaving Kit::rPI\n");
}

Kit::Item Kit::addTextEntry ( const char *prop_name, const char *default_val )
{
    if (DEBUG)
       printf("addTextEntry...\n");

    struct support
    {
        static void changed_callback ( GtkEditable *editable, gpointer item )
        {
			if (!disableOnChange)		//MW - HACK :)
				write_func(GTK_BOX(item));
            printf("changed callback\n");
        };

        static void write_func (GtkBox *item)
        {
            GtkEntry *entry = GTK_ENTRY(g_object_get_data(G_OBJECT(item),"entry"));
            PropertySet *owner = getOwner(item);
            const char* prop_name = getPropName(item);
            if (owner) owner->set ( prop_name, (const char*)gtk_entry_get_text(entry) );
            printf("write property %s\n",prop_name);
        };

        static void read_func (GtkBox *item)
        {
            GtkEntry *entry = GTK_ENTRY(g_object_get_data(G_OBJECT(item),"entry"));
            PropertySet *owner = getOwner(item);
            const char* prop_name = getPropName(item);
            if (owner) gtk_entry_set_text(entry,owner->getString(prop_name));
            printf("read property %s\n",prop_name);
        };
    };

  GtkWidget *item = gtk_vbox_new (FALSE, 4);
  gtk_widget_show (item);
  gtk_container_set_border_width (GTK_CONTAINER (item), 4);

  GtkWidget *label = gtk_label_new (_(prop_name));
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (item), label, FALSE, FALSE, 0);

  GtkWidget *entry = gtk_entry_new ();
  gtk_widget_show (entry);
  gtk_box_pack_start (GTK_BOX (item), entry, FALSE, FALSE, 0);
  g_object_set_data(G_OBJECT(item),"entry",entry);
  gtk_entry_set_text( GTK_ENTRY(entry), default_val );

  g_signal_connect ( (gpointer)entry, "changed", G_CALLBACK(support::changed_callback), item );

  registerPropertyItem ( GTK_BOX(item), support::write_func, support::read_func, prop_name );
  
  if (DEBUG)
       printf("addTextEntry done\n");
  return Kit::Item(item);
}

Kit::Item Kit::addIntEntry ( const char* prop_name, int min, int max, int step, int default_val )
{
    if (DEBUG)
       printf("addIntEntry ...\n");

    struct support
    {
        static void changed_callback ( GtkEditable *editable, gpointer item )
        {
            write_func(GTK_BOX(item));
            printf("changed callback\n");
        };

        static void write_func (GtkBox *item)
        {
            GtkSpinButton *spinbutton = GTK_SPIN_BUTTON(g_object_get_data(G_OBJECT(item),"spinbutton"));
            PropertySet *owner = getOwner(item);
            const char* prop_name = getPropName(item);
            if (owner) owner->set ( prop_name, gtk_spin_button_get_value_as_int(spinbutton) );
            printf("write property %s\n",prop_name);
        };

        static void read_func (GtkBox *item)
        {
            GtkSpinButton *spinbutton = GTK_SPIN_BUTTON(g_object_get_data(G_OBJECT(item),"spinbutton"));
            PropertySet *owner = getOwner(item);
            const char* prop_name = getPropName(item);
            if (owner) gtk_spin_button_set_value(spinbutton,owner->getInt(prop_name));
            printf("read property %s\n",prop_name);
        };
    };

  GtkWidget *item = gtk_vbox_new (FALSE, 4); // 2. - odstepy
  gtk_widget_show (item);
  gtk_container_set_border_width (GTK_CONTAINER (item), 4);

  GtkWidget *label = gtk_label_new (_(prop_name));
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (item), label, FALSE, FALSE, 0);

  GtkAdjustment *spinbutton_adj = GTK_ADJUSTMENT ( 
                        gtk_adjustment_new (default_val, min, max, step, step, step) );
  GtkWidget *spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adj), 1, 0);
  gtk_widget_show (spinbutton);
  gtk_box_pack_start (GTK_BOX (item), spinbutton, FALSE, FALSE, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton), TRUE);
  gtk_spin_button_set_snap_to_ticks (GTK_SPIN_BUTTON (spinbutton), TRUE);
  g_object_set_data(G_OBJECT(item),"spinbutton",spinbutton);

  g_signal_connect ( (gpointer)spinbutton, "changed", G_CALLBACK(support::changed_callback), item );

  registerPropertyItem ( GTK_BOX(item), support::write_func, support::read_func, prop_name );

  if (DEBUG)
       printf("addIntEntry done\n");
  
  return Kit::Item(item);
}    

Kit::Item Kit::addButton ( const char *prop_name, bool toggle, bool toggle_state )
{
     if (DEBUG)
        printf("addButton ...\n");
     struct support
     {
          static void button_pressed(GtkButton *button, gpointer item)
          {
                 write_int(GTK_BOX(item),1);
          }

          static void button_released(GtkButton *button, gpointer item)
          {
                 write_int(GTK_BOX(item),0);
          }
            
          static void button_toggled(GtkToggleButton *button, gpointer item)
          {
                 write_func_tb(GTK_BOX(item));
          }

          static void write_int(GtkBox *item, int val)
          {
                 PropertySet *owner = getOwner(item);
                 const char* prop_name = getPropName(item);
                 if (owner)
                    owner->set ( prop_name, val );
                 printf("write property %s\n",prop_name);
          }

          static void write_func_b(GtkBox *item)
          {
          }

          static void write_func_tb(GtkBox *item)
          {
                 GtkToggleButton *tb = 
                        GTK_TOGGLE_BUTTON(g_object_get_data(G_OBJECT(item),"button"));
                 write_int ( item, gtk_toggle_button_get_active ( tb ) );
          }

          static void read_func_b(GtkBox *item)
          {
          }

          static void read_func_tb(GtkBox *item)
          {
                 PropertySet *owner = getOwner(item);
                 const char *prop_name = getPropName(item);
                 GtkToggleButton *tb = GTK_TOGGLE_BUTTON(g_object_get_data(G_OBJECT(item),"button"));
                 if (owner)
                    gtk_toggle_button_set_active ( tb, !!owner->getInt(prop_name) );
          }
     };

     GtkWidget *item = gtk_vbox_new (FALSE, 4); // 2. - odstepy
     gtk_widget_show (item);
     gtk_container_set_border_width (GTK_CONTAINER (item), 4);
     
     GtkWidget *button;
     
     if (toggle)
     {
         button = gtk_check_button_new_with_mnemonic (_(prop_name));
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button),toggle_state);
         gtk_widget_show (button);
         gtk_box_pack_start (GTK_BOX (item), button, FALSE, FALSE, 0);
         
         g_object_set_data(G_OBJECT(item),"button",button);

         g_signal_connect ((gpointer) button, "toggled",
                           G_CALLBACK (support::button_toggled),
                           item );
         registerPropertyItem ( GTK_BOX(item), support::write_func_tb,
                                support::read_func_tb, prop_name );
     }
     else
     {
         button = gtk_button_new_with_mnemonic (_(prop_name));
         gtk_widget_show (button);
         gtk_box_pack_start (GTK_BOX (item), button, FALSE, FALSE, 0);

         g_signal_connect ((gpointer) button, "pressed",
                           G_CALLBACK(support::button_pressed),
                           item );
         g_signal_connect ((gpointer) button, "released",
                           G_CALLBACK (support::button_released),
                           item );
         registerPropertyItem ( GTK_BOX(item), support::write_func_b,
                                support::read_func_b, prop_name );
     }

     if (DEBUG)
        printf("addButton done\n");
     
     return Kit::Item(item);
}

Kit::Item Kit::addFileSel(const char *prop_name, const char *dir_path, const char *pattern, const char *default_val)
{
/*
  if (DEBUG)
     printf("addFileSel(dir:%s,val:%s) ...\n",dir_path,default_val);

  struct support
  {
         static GtkWidget* get_dialog()
         {
                static GtkWidget *filechooserdialog = NULL;
                if (!filechooserdialog)
                {
                   GtkWidget *fcd = gtk_file_chooser_dialog_new ("Choose a file", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL);
                   filechooserdialog = fcd;
                   gtk_window_set_position (GTK_WINDOW (filechooserdialog), GTK_WIN_POS_CENTER_ON_PARENT);
                   gtk_window_set_modal (GTK_WINDOW (filechooserdialog), TRUE);
                   gtk_window_set_destroy_with_parent (GTK_WINDOW (filechooserdialog), TRUE);
                   gtk_window_set_skip_taskbar_hint (GTK_WINDOW (filechooserdialog), FALSE);
                   gtk_window_set_type_hint (GTK_WINDOW (filechooserdialog), GDK_WINDOW_TYPE_HINT_DIALOG);
                   gtk_window_set_transient_for ( GTK_WINDOW(filechooserdialog), 
                                                  GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(external_widget))) );
                   g_signal_connect ( (gpointer) filechooserdialog, "file_activated",
                                    G_CALLBACK (support::file_activated), NULL );
                   g_signal_connect ( (gpointer) filechooserdialog, "delete_event",
                                    G_CALLBACK(gtk_widget_hide_on_delete), NULL );
                }
                return filechooserdialog;
         }

         static void combo_changed ( GtkComboBox *combobox, gpointer item )
         {
                // reorganizujemy listy wyboru
         }
         
         static void entry_changed ( GtkEditable *editable, gpointer item )
         {
                // wyslamy wartosc do wlasciciela
                write_func(GTK_BOX(item));
         }
         
         static const gchar* filename_slimming ( gchar *full_name, gpointer item )
         {
                gpointer dir_path = g_object_get_data(G_OBJECT(item),"dirpath");
                if (!dir_path)
                {
                   Publisher::warn("Code bug:\nInaccessible dirpath gstring for file-sel property item");
                   return "";
                }
                
                printf("Slimming, dir %s, file %s\n",((GString*)dir_path)->str,full_name);
                
                return ( g_str_has_prefix ( full_name, ((GString*)dir_path)->str) ) ?
                       full_name + ((GString*)dir_path)->len + 1: NULL;
         }

         static void file_activated ( GtkFileChooser *filechooser, gpointer data )
         {
                // ukrywamy dialog
                // wstawiamy nazwe pliku do entry
                // reorganizujemy listy wyboru

                gtk_widget_hide(GTK_WIDGET(filechooser));

                gpointer item = g_object_get_data(G_OBJECT(filechooser),"item");
                if (!item)
                {
                   Publisher::warn("Code bug:\nInaccessible item for file-sel dialog");
                   return;
                }

				//MW - coby pamietal sciezke na ktorej skonczylismy
//				gchar *dir_path = gtk_file_chooser_get_current_folder(filechooser);
//			    g_object_set_data_full ( G_OBJECT(item), "dirpath", g_string_new(dir_path), my_gstring_free );
				//MW


                gchar *full_name = gtk_file_chooser_get_filename(filechooser);
                
                const gchar *short_name = filename_slimming ( full_name, item );

                if (!short_name)
                {
                   Publisher::warn("Invalid selection");
                   return;
                }

                printf("File activated <%s> <%s>\n",short_name,full_name);

                GtkEntry *entry = GTK_ENTRY ( g_object_get_data(G_OBJECT(item),"entry") );
                if (!entry)
                   Publisher::warn("Code bug:\nInaccessible entry widget for file-sel property item");
                else
                   gtk_entry_set_text(entry,short_name);

                GtkComboBox *combo = GTK_COMBO_BOX ( g_object_get_data(G_OBJECT(item),"combo") );
                if (!entry)
                   Publisher::warn("Code bug:\nInaccessible combo widget for file-sel property item");
                else
                {
                   gtk_combo_box_prepend_text(combo,short_name);
                   gtk_combo_box_remove_text(combo,6);
                }
         }

         static void button_released ( GtkButton *button, gpointer item )
         {
                GtkWidget *dialog = get_dialog();
                printf("setting item for dialog (%X)\n",dialog);
                g_object_set_data(G_OBJECT(dialog),"item",item);
                printf("set\n");
                gpointer dir_path = g_object_get_data(G_OBJECT(item),"dirpath");
                if (!dir_path)
                {
                   Publisher::warn("Code bug:\nInaccessible dirpath gstring for file-sel property item");
                   return;
                }
                gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),((GString*)dir_path)->str);
                gpointer filter = g_object_get_data(G_OBJECT(item),"filter");
                if (!filter)
                {
                   Publisher::warn("Code bug:\nInaccessible filter for file-sel property item");
                   return;
                }
                gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog),GTK_FILE_FILTER(filter));
                gtk_widget_show(GTK_WIDGET(dialog));
         }

         static void read_func(GtkBox *item)
         {
            GtkEntry *entry = GTK_ENTRY(g_object_get_data(G_OBJECT(item),"entry"));
            PropertySet *owner = getOwner(item);
            const char* prop_name = getPropName(item);
            if (owner) gtk_entry_set_text(entry,owner->getString(prop_name));
            printf("read property %s\n",prop_name);
         }

         static void write_func(GtkBox *item)
         {
            GtkEntry *entry = GTK_ENTRY(g_object_get_data(G_OBJECT(item),"entry"));
            PropertySet *owner = getOwner(item);
            const char* prop_name = getPropName(item);
			if (!disableOnChange)		//MW - HACK :)
               if (owner) owner->set ( prop_name, (const char*)gtk_entry_get_text(entry) );
            printf("write property %s\n",prop_name);
         }
  };

  if (!dir_path)
  {
     Publisher::warn("Code bug:\nNULL dirpath for Kit::addFileSel");
     return Kit::Item(NULL);
  }

  if (E_DEBUG)
     printf("addFileSel/creating widgets\n");
  
  GtkWidget *item = gtk_vbox_new (FALSE, 4);
  gtk_widget_show (item);
  gtk_container_set_border_width (GTK_CONTAINER (item), 4);

  GtkWidget *label = gtk_label_new (_(prop_name));
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (item), label, FALSE, FALSE, 0);

  GtkWidget *hbox = gtk_hbox_new (FALSE, 4);
  gtk_widget_show (hbox);
  gtk_box_pack_start (GTK_BOX (item), hbox, TRUE, TRUE, 0);

  GtkWidget *comboboxentry = gtk_combo_box_entry_new_text ();
  gtk_widget_show (comboboxentry);
  gtk_box_pack_start (GTK_BOX (hbox), comboboxentry, TRUE, TRUE, 0);
  gtk_entry_set_text ( GTK_ENTRY(GTK_BIN(comboboxentry)->child), default_val );
  g_signal_connect ((gpointer) comboboxentry, "changed", G_CALLBACK (support::combo_changed),item);
  g_signal_connect ((gpointer) GTK_ENTRY(GTK_BIN(comboboxentry)->child), "changed", G_CALLBACK (support::entry_changed),item);

  if (DEBUG) printf("addFileSel/accessing dialog\n");
  GtkWidget *filechooserdialog = support::get_dialog();

  if (DEBUG) printf("addFileSel/creating filter(%s)\n",pattern);
  GtkFileFilter *filter = gtk_file_filter_new();
  if (DEBUG) printf("*\n");
  g_object_ref(G_OBJECT(filter));
  if (DEBUG) printf("**\n",pattern);
  gtk_file_filter_add_pattern(filter, pattern);

  if (DEBUG) printf("addFileSel/creating button\n");
  GtkWidget *button = gtk_button_new_with_mnemonic (_("..."));
  gtk_widget_show (button);
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);

  if (DEBUG)
     printf("addFileSel/connecting signals\n");

  g_signal_connect ((gpointer) button, "released", G_CALLBACK (support::button_released), item );
  
  g_object_set_data ( G_OBJECT(item), "entry", GTK_BIN(comboboxentry)->child );
  g_object_set_data ( G_OBJECT(item), "combo", comboboxentry );
  g_object_set_data ( G_OBJECT(item), "filter", filter );
  g_object_set_data_full ( G_OBJECT(item), "dirpath", g_string_new(dir_path), my_gstring_free );

  registerPropertyItem ( GTK_BOX(item), support::write_func,
                         support::read_func, prop_name );

  if (DEBUG)
     printf("addFileSel done\n");
                         
  return Kit::Item(item);
  */
  
  return addFileSel ( prop_name,
                      dir_path,
                      std::vector<std::string>(1,std::string(pattern)),
                      default_val,
                      6,
                      false,
                      std::vector<std::string>()
                      );
}

Kit::Item Kit::addFileSel(const char *prop_name, const char *dir_path,
          std::vector<std::string> patterns, const char *default_val,
          int store_size, bool editable, std::vector<std::string> embedded_vals )
{
  if (DEBUG)
     printf("addFileSel(dir:%s,val:%s) ...\n",dir_path,default_val);

  struct support
  {
         static GtkWidget* get_dialog()
         {
                static GtkWidget *filechooserdialog = NULL;
                if (!filechooserdialog)
                {
                   GtkWidget *fcd = gtk_file_chooser_dialog_new ("Choose a file", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL);
                   filechooserdialog = fcd;
                   gtk_window_set_position (GTK_WINDOW (filechooserdialog), GTK_WIN_POS_CENTER_ON_PARENT);
                   gtk_window_set_modal (GTK_WINDOW (filechooserdialog), TRUE);
                   gtk_window_set_destroy_with_parent (GTK_WINDOW (filechooserdialog), TRUE);
                   gtk_window_set_skip_taskbar_hint (GTK_WINDOW (filechooserdialog), FALSE);
                   gtk_window_set_type_hint (GTK_WINDOW (filechooserdialog), GDK_WINDOW_TYPE_HINT_DIALOG);
                   gtk_window_set_transient_for ( GTK_WINDOW(filechooserdialog), 
                                                  GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(external_widget))) );
                   g_signal_connect ( (gpointer) filechooserdialog, "file_activated",
                                    G_CALLBACK (support::file_activated), NULL );
                   g_signal_connect ( (gpointer) filechooserdialog, "delete_event",
                                    G_CALLBACK(gtk_widget_hide_on_delete), NULL );
                }
                return filechooserdialog;
         }

         static void combo_changed ( GtkComboBox *combobox, gpointer item )
         {
                // reorganizujemy listy wyboru
         }
         
         static void entry_changed ( GtkEditable *editable, gpointer item )
         {
                // wyslamy wartosc do wlasciciela
                write_func(GTK_BOX(item));
         }
         
         static const gchar* filename_slimming ( gchar *full_name, gpointer item )
         {
                gpointer dir_path = g_object_get_data(G_OBJECT(item),"dirpath");
                if (!dir_path)
                {
                   Publisher::warn("Code bug:\nInaccessible dirpath gstring for file-sel property item");
                   return "";
                }
                
                printf("Slimming, dir %s, file %s\n",((GString*)dir_path)->str,full_name);
                
                return ( g_str_has_prefix ( full_name, ((GString*)dir_path)->str) ) ?
                       full_name + ((GString*)dir_path)->len + 1: NULL;
         }

         static void file_activated ( GtkFileChooser *filechooser, gpointer data )
         {
                // ukrywamy dialog
                // wstawiamy nazwe pliku do entry
                // reorganizujemy listy wyboru

                gtk_widget_hide(GTK_WIDGET(filechooser));

                gpointer item = g_object_get_data(G_OBJECT(filechooser),"item");
                if (!item)
                {
                   Publisher::warn("Code bug:\nInaccessible item for file-sel dialog");
                   return;
                }

				//MW - coby pamietal sciezke na ktorej skonczylismy
//				gchar *dir_path = gtk_file_chooser_get_current_folder(filechooser);
//			    g_object_set_data_full ( G_OBJECT(item), "dirpath", g_string_new(dir_path), my_gstring_free );
				//MW


                gchar *full_name = gtk_file_chooser_get_filename(filechooser);
                
                const gchar *short_name = filename_slimming ( full_name, item );

                if (!short_name)
                {
                   Publisher::warn("Invalid selection");
                   return;
                }

                printf("File activated <%s> <%s>\n",short_name,full_name);

                GtkEntry *entry = GTK_ENTRY ( g_object_get_data(G_OBJECT(item),"entry") );
                if (!entry)
                   Publisher::warn("Code bug:\nInaccessible entry widget for file-sel property item");
                else
                   gtk_entry_set_text(entry,short_name);

                GtkComboBox *combo = GTK_COMBO_BOX ( g_object_get_data(G_OBJECT(item),"combo") );
                if (!entry)
                {
                   Publisher::warn("Code bug:\nInaccessible combo widget for file-sel property item");
                   return;
                }

                int s_size = (int) g_object_get_data ( G_OBJECT(item),"store_size" );
                if (!s_size)
                {
                   Publisher::warn("Code bug:\nInaccessible store size int for file-sel property item");
                   return;
                }

                int embd = (int) g_object_get_data ( G_OBJECT(item),"embd_amount" );
                if (!embd)
                {
                   Publisher::warn("Code bug:\nInaccessible embedded amount int for file-sel property item");
                   return;
                }

                gtk_combo_box_insert_text ( combo, embd-1, short_name );
                gtk_combo_box_remove_text ( combo, s_size-1 );
         }

         static void button_released ( GtkButton *button, gpointer item )
         {
                GtkWidget *dialog = get_dialog();
                printf("setting item for dialog (%X)\n",dialog);
                g_object_set_data(G_OBJECT(dialog),"item",item);
                printf("set\n");
                gpointer dir_path = g_object_get_data(G_OBJECT(item),"dirpath");
                if (!dir_path)
                {
                   Publisher::warn("Code bug:\nInaccessible dirpath gstring for file-sel property item");
                   return;
                }
                gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),((GString*)dir_path)->str);
                gpointer filter = g_object_get_data(G_OBJECT(item),"filter");
                if (!filter)
                {
                   Publisher::warn("Code bug:\nInaccessible filter for file-sel property item");
                   return;
                }
                gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog),GTK_FILE_FILTER(filter));
                gtk_widget_show(GTK_WIDGET(dialog));
         }

         static void read_func(GtkBox *item)
         {
            GtkEntry *entry = GTK_ENTRY(g_object_get_data(G_OBJECT(item),"entry"));
            PropertySet *owner = getOwner(item);
            const char* prop_name = getPropName(item);
            if (owner) gtk_entry_set_text(entry,owner->getString(prop_name));
            printf("read property %s\n",prop_name);
         }

         static void write_func(GtkBox *item)
         {
            GtkEntry *entry = GTK_ENTRY(g_object_get_data(G_OBJECT(item),"entry"));
            PropertySet *owner = getOwner(item);
            const char* prop_name = getPropName(item);
			if (!disableOnChange)		//MW - HACK :)
			{
               if (owner) owner->set ( prop_name, (const char*)gtk_entry_get_text(entry) );
               printf("write property %s %d\n",prop_name,owner);
            }
         }
  };

  if (E_DEBUG)
     printf("addFileSel/creating widgets\n");
  
  GtkWidget *item = gtk_vbox_new (FALSE, 4);
  gtk_widget_show (item);
  gtk_container_set_border_width (GTK_CONTAINER (item), 4);

  GtkWidget *label = gtk_label_new (_(prop_name));
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (item), label, FALSE, FALSE, 0);

  GtkWidget *hbox = gtk_hbox_new (FALSE, 4);
  gtk_widget_show (hbox);
  gtk_box_pack_start (GTK_BOX (item), hbox, TRUE, TRUE, 0);

  GtkWidget *comboboxentry = gtk_combo_box_entry_new_text ();
  gtk_widget_show (comboboxentry);
  gtk_box_pack_start (GTK_BOX (hbox), comboboxentry, TRUE, TRUE, 0);
  gtk_entry_set_text ( GTK_ENTRY(GTK_BIN(comboboxentry)->child), default_val );
  gtk_entry_set_editable ( GTK_ENTRY(GTK_BIN(comboboxentry)->child), editable );

  for ( int i=0; i<embedded_vals.size(); i++)
        gtk_combo_box_append_text ( GTK_COMBO_BOX(comboboxentry), embedded_vals[i].c_str() );

  g_signal_connect ((gpointer) comboboxentry, "changed", G_CALLBACK (support::combo_changed),item);
  g_signal_connect ((gpointer) GTK_ENTRY(GTK_BIN(comboboxentry)->child), "changed", G_CALLBACK (support::entry_changed),item);

  if (DEBUG) printf("addFileSel/accessing dialog\n");
  GtkWidget *filechooserdialog = support::get_dialog();

  GtkFileFilter *filter;
  if (dir_path)
  {
     if (DEBUG) printf("addFileSel/creating filter\n");
     filter = gtk_file_filter_new();
     if (DEBUG) printf("*\n");
     g_object_ref(G_OBJECT(filter));
     for (int j=0;j<patterns.size();j++)
     {
         gtk_file_filter_add_pattern(filter, patterns[j].c_str());
         if (DEBUG) printf("-> %s\n",patterns[j].c_str());
     }
  }

  if (DEBUG) printf("addFileSel/creating button\n");
  GtkWidget *button = gtk_button_new_with_mnemonic (_("..."));
  if (dir_path)
     gtk_widget_show (button);
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);

  if (DEBUG)
     printf("addFileSel/connecting signals\n");

  g_signal_connect ((gpointer) button, "released", G_CALLBACK (support::button_released), item );
  
  g_object_set_data ( G_OBJECT(item), "entry", GTK_BIN(comboboxentry)->child );
  g_object_set_data ( G_OBJECT(item), "combo", comboboxentry );
  g_object_set_data ( G_OBJECT(item), "filter", filter );
  g_object_set_data ( G_OBJECT(item), "store_size", (gpointer) (store_size+embedded_vals.size()+1) );
  g_object_set_data ( G_OBJECT(item), "embd_amount", (gpointer) (embedded_vals.size()+1) );
  g_object_set_data_full ( G_OBJECT(item), "dirpath", g_string_new(dir_path), my_gstring_free );

  registerPropertyItem ( GTK_BOX(item), support::write_func,
                         support::read_func, prop_name );

  if (DEBUG)
     printf("addFileSel done\n");
                         
  return Kit::Item(item);
}


/*
GtkWidget*
create_window1 (void)
{
  GtkWidget *window1;
  GtkWidget *hbox1;
  GtkWidget *label1;
  GtkWidget *vbox1;
  GtkWidget *expander1;
  GtkWidget *label5;
  GtkWidget *label2;
  GtkWidget *expander2;
  GtkWidget *viewport1;
  GtkWidget *hbox2;
  GtkWidget *scrolledwindow1;
  GtkWidget *treeview1;
  GtkWidget *vseparator1;
  GtkWidget *label7;
  GtkWidget *label3;
  GtkWidget *expander3;
  GtkWidget *notebook1;
  GtkWidget *label12;
  GtkWidget *label9;
  GtkWidget *label13;
  GtkWidget *label10;
  GtkWidget *label14;
  GtkWidget *label11;
  GtkWidget *label8;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window1), _("window1"));

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_container_add (GTK_CONTAINER (window1), hbox1);

  label1 = gtk_label_new (_("label1"));
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (hbox1), label1, FALSE, FALSE, 0);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox1, TRUE, TRUE, 0);

  expander1 = gtk_expander_new (NULL);
  gtk_widget_show (expander1);
  gtk_box_pack_start (GTK_BOX (vbox1), expander1, FALSE, FALSE, 0);

  label5 = gtk_label_new (_("label5"));
  gtk_widget_show (label5);
  gtk_container_add (GTK_CONTAINER (expander1), label5);

  label2 = gtk_label_new (_("label2"));
  gtk_widget_show (label2);
  gtk_expander_set_label_widget (GTK_EXPANDER (expander1), label2);

  expander2 = gtk_expander_new (NULL);
  gtk_widget_show (expander2);
  gtk_box_pack_start (GTK_BOX (vbox1), expander2, FALSE, FALSE, 0);
  gtk_expander_set_expanded (GTK_EXPANDER (expander2), TRUE);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport1);
  gtk_container_add (GTK_CONTAINER (expander2), viewport1);

  hbox2 = gtk_hbox_new (FALSE, 8);
  gtk_widget_show (hbox2);
  gtk_container_add (GTK_CONTAINER (viewport1), hbox2);
  gtk_container_set_border_width (GTK_CONTAINER (hbox2), 8);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (hbox2), scrolledwindow1, TRUE, TRUE, 0);

  treeview1 = gtk_tree_view_new ();
  gtk_widget_show (treeview1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), treeview1);
  gtk_tree_view_set_enable_search (GTK_TREE_VIEW (treeview1), FALSE);

  vseparator1 = gtk_vseparator_new ();
  gtk_widget_show (vseparator1);
  gtk_box_pack_start (GTK_BOX (hbox2), vseparator1, FALSE, FALSE, 0);

  label7 = gtk_label_new (_("label7"));
  gtk_widget_show (label7);
  gtk_box_pack_start (GTK_BOX (hbox2), label7, TRUE, FALSE, 0);

  label3 = gtk_label_new (_("label3"));
  gtk_widget_show (label3);
  gtk_expander_set_label_widget (GTK_EXPANDER (expander2), label3);

  expander3 = gtk_expander_new (NULL);
  gtk_widget_show (expander3);
  gtk_box_pack_start (GTK_BOX (vbox1), expander3, TRUE, TRUE, 0);
  gtk_expander_set_expanded (GTK_EXPANDER (expander3), TRUE);

  notebook1 = gtk_notebook_new ();
  gtk_widget_show (notebook1);
  gtk_container_add (GTK_CONTAINER (expander3), notebook1);

  label12 = gtk_label_new (_("label12"));
  gtk_widget_show (label12);
  gtk_container_add (GTK_CONTAINER (notebook1), label12);

  label9 = gtk_label_new (_("label9"));
  gtk_widget_show (label9);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label9);

  label13 = gtk_label_new (_("label13"));
  gtk_widget_show (label13);
  gtk_container_add (GTK_CONTAINER (notebook1), label13);

  label10 = gtk_label_new (_("label10"));
  gtk_widget_show (label10);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label10);

  label14 = gtk_label_new (_("label14"));
  gtk_widget_show (label14);
  gtk_container_add (GTK_CONTAINER (notebook1), label14);

  label11 = gtk_label_new (_("label11"));
  gtk_widget_show (label11);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), label11);

  label8 = gtk_label_new (_("label8"));
  gtk_widget_show (label8);
  gtk_expander_set_label_widget (GTK_EXPANDER (expander3), label8);

*/
