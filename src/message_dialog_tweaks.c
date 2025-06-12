/*

	gtk3-throwback
	GtkMessageDialog tweaks
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

#include "message_dialog_tweaks.h"

static void (*orig_message_dialog_realize)(GtkWidget*);
static void message_dialog_realize(GtkMessageDialog*);

static void add_message_dialog_image(GtkMessageDialog*);
static void align_message_dialog_text(GtkWidget*, gpointer);

void
override_message_dialog_class(void)
{
	/*
	  This function overrides the GtkMessageDialog class, allowing us to hook in to the realize
	  class function and add custom behaviour immediately before the message dialog is realized.
	*/

	// Retrieve the GtkWidgetClass
	GtkWidgetClass *message_dialog_class = GTK_WIDGET_CLASS(g_type_class_ref(GTK_TYPE_MESSAGE_DIALOG));

	// Store the original realize function and replace it
	orig_message_dialog_realize = message_dialog_class->realize;
	message_dialog_class->realize = (void (*)(GtkWidget*))message_dialog_realize;
	
	g_type_class_unref(message_dialog_class);
	
}

static void
message_dialog_realize(GtkMessageDialog *dialog)
{
	
	// Add an image to the dialog if needed
	add_message_dialog_image(dialog);

	// Update spacing and border width of first GtkBox child of message dialog
	GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	if (content_area && GTK_IS_BOX(content_area)) {
		gtk_box_set_spacing(GTK_BOX(content_area), 22);
		gtk_container_set_border_width(GTK_CONTAINER(content_area), 12);
	}

	// Update button layout and spacing
	GtkWidget* action_area = gtk_dialog_get_action_area(GTK_DIALOG(dialog));
	if (action_area && GTK_IS_BUTTON_BOX(action_area)) {
		gtk_button_box_set_layout(GTK_BUTTON_BOX(action_area), GTK_BUTTONBOX_END);
		gtk_box_set_spacing(GTK_BOX(action_area), 6);
		gtk_container_set_border_width(GTK_CONTAINER(action_area), 0);
	}

	GtkWidget* message_area = gtk_message_dialog_get_message_area(dialog);
	// Now we need to iterate over the children of the message_area and change their alignment

	if (message_area && GTK_IS_BOX(message_area)) {

		gtk_box_set_spacing(GTK_BOX(message_area), 12);

		gtk_container_foreach(GTK_CONTAINER(message_area), align_message_dialog_text, NULL);

		GtkWidget* message_area_parent = gtk_widget_get_parent(message_area);

		if (message_area_parent && GTK_IS_BOX(message_area)) {
			g_object_set(G_OBJECT(message_area_parent), "margin", 0, NULL);
			gtk_container_set_border_width(GTK_CONTAINER(message_area_parent), 0);
			gtk_box_set_spacing(GTK_BOX(message_area_parent), 12);
		}
		
	}

	orig_message_dialog_realize(GTK_WIDGET(dialog));

}

static void
add_message_dialog_image(GtkMessageDialog *dialog)
{
    if (!gtk_message_dialog_get_image(dialog)) {

		GtkWidget* image = NULL;
		
		GtkMessageType message_type;
		g_object_get(G_OBJECT(dialog), "message-type", &message_type, NULL);

		switch (message_type) {
		case GTK_MESSAGE_INFO:
			image = gtk_image_new_from_icon_name("dialog-info", GTK_ICON_SIZE_DIALOG);
			break;
		case GTK_MESSAGE_WARNING:
			image = gtk_image_new_from_icon_name("dialog-warning", GTK_ICON_SIZE_DIALOG);
			break;
		case GTK_MESSAGE_QUESTION:
			image = gtk_image_new_from_icon_name("dialog-question", GTK_ICON_SIZE_DIALOG);
			break;
		case GTK_MESSAGE_ERROR:
			image = gtk_image_new_from_icon_name("dialog-error", GTK_ICON_SIZE_DIALOG);
			break;
		}

		if (image) {
			gtk_message_dialog_set_image(dialog, image);
			gtk_widget_set_visible(image, TRUE);
		}
		
	}
}

static void
align_message_dialog_text(GtkWidget *widget, gpointer data)
{
	if (GTK_IS_LABEL(widget)) {
		gtk_widget_set_halign(widget, GTK_ALIGN_START);
	}
}
