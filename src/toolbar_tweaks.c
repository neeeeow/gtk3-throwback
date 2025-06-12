/*

	gtk3-throwback
	GtkToolbar tweaks
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

#include "toolbar_tweaks.h"

static GtkToolbarStyle toolbar_style; 

static void (*orig_toolbar_realize)(GtkWidget*);
static void toolbar_realize(GtkToolbar*);
static void update_toolbar_style(GtkToolbar*);

void
override_toolbar_class(void)
{
	/*
	  This function overrides the GtkToolbar class, allowing us to hook in to the realize
	  class function and add custom behaviour immediately before the toolbar is realized.
	*/

	const gchar *toolbar_style_env = g_getenv("GTK_TOOLBAR_STYLE");

	if (toolbar_style_env) {

		if (g_strcmp0(toolbar_style_env, "0") == 0) {
			toolbar_style = GTK_TOOLBAR_ICONS;
		} else if (g_strcmp0(toolbar_style_env, "1") == 0) {
			toolbar_style = GTK_TOOLBAR_TEXT;
		} else if (g_strcmp0(toolbar_style_env, "2") == 0) {
			toolbar_style = GTK_TOOLBAR_BOTH;
		} else if (g_strcmp0(toolbar_style_env, "3") == 0) {
			toolbar_style = GTK_TOOLBAR_BOTH_HORIZ;
		} else {
			g_warning("Invalid toolbar style. Defaulting to both icons and text...");
			toolbar_style = GTK_TOOLBAR_BOTH;
		}
		
	} else {
		toolbar_style = GTK_TOOLBAR_BOTH;
	}
	
	// Retrieve the GtkWidgetClass
	GtkWidgetClass *toolbar_class = GTK_WIDGET_CLASS(g_type_class_ref(GTK_TYPE_TOOLBAR));

	// Store the original realize function and replace it
	orig_toolbar_realize = toolbar_class->realize;
	toolbar_class->realize = (void (*)(GtkWidget*))toolbar_realize;
	
	g_type_class_unref(toolbar_class);
	
}

static void
toolbar_realize(GtkToolbar *toolbar)
{

	GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(toolbar));

	if (gtk_style_context_has_class(context, "primary-toolbar")) {
		update_toolbar_style(toolbar);
	}
	
	orig_toolbar_realize(GTK_WIDGET(toolbar));
}

static void
update_toolbar_style(GtkToolbar *toolbar)
{
	gboolean all_toolbuttons_have_icons = TRUE;
	gboolean all_toolbuttons_have_labels = TRUE;
	
	// Iterate over toolbar items
	gint n_items = gtk_toolbar_get_n_items(toolbar);
	for (gint i=0; i < n_items; i++) {

		GtkToolItem *tool_item = gtk_toolbar_get_nth_item(toolbar, i);

		if (tool_item && GTK_IS_TOOL_BUTTON(tool_item)) {
			// If the item is a GtkToolButton, attempt to add label/image if it is missing

			GtkToolButton *button = GTK_TOOL_BUTTON(tool_item);
			gtk_tool_button_set_use_underline(button, TRUE);
			
			const gchar *icon_name = gtk_tool_button_get_icon_name(button);
			const gchar *label = gtk_tool_button_get_label(button);

			if (label) {
				if (!icon_name) {
					const gchar *new_icon_name = g_hash_table_lookup(label_icon_map, label);
					if (new_icon_name) {
						gtk_tool_button_set_icon_name(button, new_icon_name);
					}
				}
				
			} else {
				if (icon_name) {
					const gchar *new_label = g_hash_table_lookup(icon_label_map, icon_name);
					if (new_label) {
						gtk_tool_button_set_label(button, new_label);
					}
				}
			}

			if (!gtk_tool_button_get_icon_name(button) && all_toolbuttons_have_icons) {
				all_toolbuttons_have_icons = FALSE;
			}

			if (!gtk_tool_button_get_label(button) && all_toolbuttons_have_labels) {
				all_toolbuttons_have_labels = FALSE;
			}
			
		}
		
	}

	switch (toolbar_style) {

	case GTK_TOOLBAR_ICONS:
		if (all_toolbuttons_have_icons) {
			gtk_toolbar_set_style(toolbar, GTK_TOOLBAR_ICONS);
		}
		break;

	case GTK_TOOLBAR_TEXT:
		if (all_toolbuttons_have_labels) {
			gtk_toolbar_set_style(toolbar, GTK_TOOLBAR_TEXT);
		}
		break;

	case GTK_TOOLBAR_BOTH:
		if (all_toolbuttons_have_icons && all_toolbuttons_have_labels) {
			gtk_toolbar_set_style(toolbar, GTK_TOOLBAR_BOTH);
		}
		break;

	case GTK_TOOLBAR_BOTH_HORIZ:
		if (all_toolbuttons_have_icons && all_toolbuttons_have_labels) {
			gtk_toolbar_set_style(toolbar, GTK_TOOLBAR_BOTH_HORIZ);
		}
		break;
		
	}
}
