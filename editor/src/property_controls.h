#ifndef PROPERTY_CONTROLS_HPP
#define PROPERTY_CONTROLS_HPP 1

#include <string>
#include <vector>

#include <gtk/gtk.h>

class PropertySet;

class Kit
{
    public:
       class Item
       {
             friend class Kit;
             public:
                    void shown();
                    void hidden();
					Item() : item(NULL) {}; //MW - HACK :)
                    void switching();
                    void setText(const char *s) { gtk_entry_set_text(GTK_ENTRY(g_object_get_data(G_OBJECT(item),"entry")),s); }; //MW - HACK :)
                    const char* getText() { return gtk_entry_get_text(GTK_ENTRY(g_object_get_data(G_OBJECT(item),"entry"))); }; //MW - HACK :)
             private:
                     GtkWidget *item;
                     Item(GtkWidget *_item) : item(_item) {}
       };
        Kit();
        static GtkBox* external_widget;
        Item addTextEntry ( const char *prop_name, const char *default_val );
        Item addIntEntry ( const char* prop_name, int min, int max, int step, int default_val );
        Item addButton ( const char *prop_name, bool toggle, bool toggle_state );
        Item addFileSel ( const char *prop_name, const char *dir_path, const char *pattern, const char *default_val );
        Item addFileSel ( const char *prop_name, const char *dir_path, std::vector<std::string> patterns, const char *default_val, int store_size, bool editable, std::vector<std::string> embedded_vals );
        /*
          dir_path: sciezka ustawiana w otwieranym dialogu, NULL oznacza samo combo
          patterns: vector ze stringami typu "*.exe"
          store_size: liczba elementow do zapamietania w rozwijanym menu (oprocz embd)
          editable: czy entry powinno przyjmowac wpisy reczne
          embedded_vals: stale pozycje do rozwijanego menu
        */

        void activate();
        static void disable();
        void setOwnerWithRead(PropertySet *set);
        void setOwnerWithWrite(PropertySet *set);

    private:
        void registerPropertyItem ( GtkBox *prop_item, 
                                    void(*write_prop_func)(GtkBox*),
                                    void(*read_prop_func)(GtkBox*),
                                    const char *prop_name );
        static void readProperty(GtkWidget *propWidget, gpointer owner);
        static void writeProperty(GtkWidget *propWidget, gpointer owner);
        static const char *getPropName(GtkBox*);
        static PropertySet *getOwner(GtkBox*);
        void setOwner(PropertySet*);
        static GtkWidget *active;
        static void update_visibility ( GtkWidget *prop_item, gpointer data );
        int elems;
        GtkBox *kit_widget;
};

/*
Organizacja widgetow:

    external_widget:
            VBox, do ktorego sa dodawane wszystkie utworzone zestawy,
            zawiera ciag kit_widget'ow

    kit_widget:
            widget zawierajacy kontrolki danego zestawu (property_item)
    
    property_item:
            widget reprezentujacy jedna kontrolke (np TextEntry),
            przechowyje takze funkcje do komunikacji z wlascicielem
            (PropertySet*, odczyt/zapis wlasciwosci) oraz nazwe
            przypisanej wlasciwosci (w niewidzialnym GtkLabel)
*/

#endif
