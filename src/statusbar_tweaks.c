#include "statusbar_tweaks.h"

static void (*orig_statusbar_realize)(GtkWidget*);
static void statusbar_realize(GtkStatusbar*);
static void add_resize_grip(GtkStatusbar*, GtkWindow*);

static gboolean on_grip_mouse_enter(GtkWidget*, GdkEventCrossing*, gpointer);
static gboolean on_grip_press(GtkWidget*, GdkEventButton*, gpointer); 

void
override_statusbar_class(void)
{
	/*
	  This function overrides the GtkStatusbar class, allowing us to hook in to the realize
	  class function and add custom behaviour immediately before the status bar is realized.
	*/

	// Retrieve the GtkWidgetClass
	GtkWidgetClass *statusbar_class = GTK_WIDGET_CLASS(g_type_class_ref(GTK_TYPE_STATUSBAR));

	// Store the original realize function and replace it
	orig_statusbar_realize = statusbar_class->realize;
	statusbar_class->realize = (void (*)(GtkWidget*))statusbar_realize;
	
	g_type_class_unref(statusbar_class);
	
}

static void
statusbar_realize(GtkStatusbar *statusbar)
{
	
	GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(statusbar));
	gtk_style_context_add_class(context, "gtk-throwback");
	
	// Set status bar margin to 0
	g_object_set(G_OBJECT(statusbar), "margin", 0, NULL);

	GtkWidget *message_area = gtk_statusbar_get_message_area(statusbar);
	if (message_area) {
		g_object_set(G_OBJECT(message_area), "margin", 0, NULL);
	}

	GtkWindow *window = GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(statusbar), GTK_TYPE_WINDOW));
	if (window) {
		if (gtk_window_get_resizable(window)) {
			add_resize_grip(statusbar, window);
		}			
	}
	
	orig_statusbar_realize(GTK_WIDGET(statusbar));
}

static void
add_resize_grip(GtkStatusbar *statusbar, GtkWindow *window)
{
	/*
	  This function adds the resize grip to a statusbar
	*/

	// Use a GtkEventBox for the resize grip
	GtkWidget *grip = gtk_event_box_new();

	gtk_widget_set_size_request(grip, 19, 19); // 18px was the grip size in GTK 2, add an extra 1px of padding
	gtk_widget_set_name(grip, "resize-grip"); // The grip texture can be set using CSS via the name #resize-grip
	gtk_widget_set_visible(grip, TRUE);
	gtk_box_pack_end(GTK_BOX(statusbar), grip, FALSE, FALSE, 0); // Add the grip to the statusbar
	gtk_box_reorder_child(GTK_BOX(statusbar), grip, 0);

	g_signal_connect(grip, "enter-notify-event", G_CALLBACK(on_grip_mouse_enter), window);
	g_signal_connect(grip, "button-press-event", G_CALLBACK(on_grip_press), window);
		
}

static gboolean
on_grip_mouse_enter(GtkWidget *grip, GdkEventCrossing *event, gpointer user_data)
{

	GdkWindow *window = gtk_widget_get_window(grip);
	GdkCursor *cursor;

	if (gtk_widget_get_direction(grip) != GTK_TEXT_DIR_RTL) {
		cursor = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_BOTTOM_RIGHT_CORNER);
	} else {
		cursor = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_BOTTOM_LEFT_CORNER);
	}
		
	gdk_window_set_cursor(window, cursor);
	g_object_unref(cursor);
	
	return FALSE;
}

static gboolean
on_grip_press(GtkWidget *grip, GdkEventButton *event, gpointer user_data)
{
	if (event->button == GDK_BUTTON_PRIMARY) {

		if (gtk_widget_get_direction(grip) != GTK_TEXT_DIR_RTL) {
			gtk_window_begin_resize_drag(GTK_WINDOW(user_data), GDK_WINDOW_EDGE_SOUTH_EAST,
									 event->button, event->x_root, event->y_root, event->time);
		} else {
			gtk_window_begin_resize_drag(GTK_WINDOW(user_data), GDK_WINDOW_EDGE_SOUTH_WEST,
									 event->button, event->x_root, event->y_root, event->time);
		}
		
		return TRUE;
	} else {
		return FALSE;
	}
	
}
