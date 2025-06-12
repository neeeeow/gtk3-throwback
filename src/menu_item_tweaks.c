/*

	gtk3-throwback
	GtkMenuItem tweaks
	Copyright (C) 2025 neeeeow (https://github.com/neeeeow/gtk3-throwback)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.

 */

#include "menu_item_tweaks.h"

static GObject *(*orig_menu_item_constructor)(GType, guint, GObjectConstructParam*);
static GObject *menu_item_constructor(GType, guint, GObjectConstructParam*);

static void (*orig_menu_item_realize)(GtkWidget*);
static void menu_item_realize(GtkMenuItem*);

void
override_menu_item_class(void)
{
	/*
	  This function overrides the GtkMenuItem type class, allowing us to easily
	  hook in to various class functions.
	*/

    // Retrieve the necessary classes
	GObjectClass *menu_item_class = g_type_class_ref(GTK_TYPE_MENU_ITEM);
	GtkWidgetClass *menu_item_widget_class = GTK_WIDGET_CLASS(menu_item_class); 

    // Store the original class functions and overwrite them with the modified versions
	orig_menu_item_constructor = menu_item_class->constructor;
	menu_item_class->constructor = menu_item_constructor;

	orig_menu_item_realize = menu_item_widget_class->realize;
	menu_item_widget_class->realize = (void (*)(GtkWidget*))menu_item_realize;
	
	g_type_class_unref(menu_item_class);
	
}

static GObject
*menu_item_constructor(
	GType type,
	guint n_construct_properties,
	GObjectConstructParam *construct_properties
	)
{
	// Replace GtkMenuItems with GtkImageMenuItems
	if (type == GTK_TYPE_MENU_ITEM) {
		type = GTK_TYPE_IMAGE_MENU_ITEM;
	}

	GObject *obj = orig_menu_item_constructor(type, n_construct_properties, construct_properties);

	if (GTK_IS_IMAGE_MENU_ITEM(GTK_WIDGET(obj))) {
		// This is needed to fix a weird bug in some applications (e.g. GIMP)
		gtk_widget_set_visible(GTK_WIDGET(obj), TRUE);
	}
	
	return obj;
}

static void
menu_item_realize(GtkMenuItem *menu_item)
{
	
	// Only attempt to add an image if the menu item is an GtkImageMenuItem and isn't an immediate child of
	// a menu bar
	if (GTK_IS_IMAGE_MENU_ITEM(menu_item) && !GTK_IS_MENU_BAR(gtk_widget_get_parent(GTK_WIDGET(menu_item)))) {
		
		// Find the menu item label
		const gchar *label = gtk_menu_item_get_label(menu_item);

		if (label) {
			// Use our hash table to find an icon name that corresponds to our label
			const gchar *icon_name = g_hash_table_lookup(label_icon_map, label);

			if (icon_name) {
				// If an icon name is found, set the menu item image
				GtkWidget *image = gtk_image_new_from_icon_name(icon_name, GTK_ICON_SIZE_MENU);
				gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_item), image);
			}
			
		}
		
	}

	// Call the original menu item realize function
	orig_menu_item_realize(GTK_WIDGET(menu_item));
}
