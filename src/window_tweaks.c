#include "window_tweaks.h"

static const gchar *application_name;
static gchar *window_title;
static gchar *window_subtitle;
static gulong title_change_handler;
static gulong subtitle_change_handler;

static void (*orig_window_realize)(GtkWidget*);
static void window_realize(GtkWindow*);

static void disable_csd(GtkWindow*);
static void csd_titlebar_foreach(GtkWidget*, gpointer);
static void tweak_headerbar(GtkHeaderBar*, GtkWindow*);
static void update_window_title(GtkWindow*);
static void on_headerbar_title_change(GObject*, GParamSpec*, gpointer);
static void on_headerbar_subtitle_change(GObject*, GParamSpec*, gpointer);
static void on_headerbar_has_subtitle_change(GObject*, GParamSpec*, gpointer);

void
override_window_class(void)
{
	/*
	  This function overrides the GtkWindow class, allowing us to hook in to the realize
	  class function and add custom behaviour immediately before the window is realized
	*/

	// Find the application name, needed for later
    application_name = g_get_application_name(); 
	
	// Retrieve the GtkWidgetClass
	GtkWidgetClass *window_class = GTK_WIDGET_CLASS(g_type_class_ref(GTK_TYPE_WINDOW));

	// Store the original realize function and replace it
	orig_window_realize = window_class->realize;
	window_class->realize = (void (*)(GtkWidget*))window_realize;
	
	g_type_class_unref(window_class);
	
}

static void
window_realize(GtkWindow *window)
{
	disable_csd(window);
	
	orig_window_realize(GTK_WIDGET(window));
}

static void
disable_csd(GtkWindow *window)
{
	/*
	  This function disables the CSDs (if enabled) for a given GtkWindow
	*/

	// libhandy windows do not support disabling CSDs, so we need to check for that
	GType HdyWindow_type = g_type_from_name("HdyWindow");
	if (HdyWindow_type) {
	    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(window));
		if (gtk_style_context_has_class(context, "unified")) {
			// libhandy windows *always* have the unified class, so if we detect the HdyWindow type
			// and the window has the unified type, it's most likely a libhandy window
			return;
		}
	}
	
	GtkWidget *titlebar = gtk_window_get_titlebar(window);

	if (titlebar) {
		g_object_ref(titlebar);
		
		gtk_window_set_titlebar(window, NULL);

		GtkWidget *window_contents = gtk_bin_get_child(GTK_BIN(window)); // The contents of the window (excluding titlebar)


		if (window_contents) {
			g_object_ref(window_contents);

			// Create a new box to store the titlebar and the window's contents
			GtkWidget *window_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

			// Add the titlebar to this box
			gtk_box_pack_start(GTK_BOX(window_box), titlebar, FALSE, FALSE, 0);

			// Place the window's contents in to the same box where the titlebar is located
			gtk_container_remove(GTK_CONTAINER(window), window_contents);
			gtk_box_pack_start(GTK_BOX(window_box), window_contents, TRUE, TRUE, 0);

			// Add this new box as the child of the window
			gtk_container_add(GTK_CONTAINER(window), window_box);
			gtk_widget_show(window_box);

			g_object_unref(window_contents);

		} else {
			// Whilst a window *should* always have contents, in the case that it doesn't just set the
			// titlebar as the child of the window
			gtk_container_add(GTK_CONTAINER(window), titlebar);
		}
				
		g_object_unref(titlebar);

		if (GTK_IS_HEADER_BAR(titlebar)) {
			
			tweak_headerbar(GTK_HEADER_BAR(titlebar), window);
		} else {
			// Some applications (e.g. gedit) have the headerbar as an immediate child of the title bar
			if (GTK_IS_CONTAINER(titlebar)) {
				gtk_container_foreach(GTK_CONTAINER(titlebar), csd_titlebar_foreach, window);
			}
		}
		
	}
}

static void
csd_titlebar_foreach(GtkWidget *widget, gpointer data)
{
	
	if (GTK_IS_HEADER_BAR(widget)) {
	    tweak_headerbar(GTK_HEADER_BAR(widget), GTK_WINDOW(data));
	}
}

static void
tweak_headerbar(GtkHeaderBar *bar, GtkWindow *window)
{
	/*
	  Makes the required modifications to the headerbar. Additionally, takes
	  the parent window as the second argument
	*/

	// Remove minimise/maximise buttons
	gtk_header_bar_set_show_close_button(bar, FALSE);

	const gchar *current_window_title = gtk_window_get_title(window);
	const gchar *headerbar_title = gtk_header_bar_get_title(bar);
	const gchar *headerbar_subtitle = gtk_header_bar_get_subtitle(bar);

	if (g_strcmp0(current_window_title, headerbar_title) == 0) {
		window_title = g_strdup(headerbar_title);
		window_subtitle = g_strdup(headerbar_subtitle);
	    update_window_title(window);

		if (headerbar_title) {
			gtk_header_bar_set_title(bar, NULL);
		}

		if (headerbar_subtitle) {
			gtk_header_bar_set_subtitle(bar, NULL);
		}
		
		title_change_handler = g_signal_connect(bar, "notify::title", G_CALLBACK(on_headerbar_title_change), window);
		subtitle_change_handler = g_signal_connect(bar, "notify::subtitle", G_CALLBACK(on_headerbar_subtitle_change), window);
		
	}
	
}

static void
update_window_title(GtkWindow *window)
{
	/*
	  Update the title of the header bar and parent window
	  Parameters:
	      bar: The header bar itself
		  window: The parent window
		  title: The current headerbar title
	*/

	const gchar *window_name;

	if (window_title && window_subtitle) {

	    if (application_name) {
			window_name = g_strconcat(window_title, " (", window_subtitle, ") - ", application_name, NULL);
		} else {
			window_name = g_strconcat(window_title, " (", window_subtitle, ")",  NULL);
		}
		
	} else if (window_title) {

	    if (application_name) {
			window_name = g_strconcat(window_title, " - ", application_name, NULL);
		} else {
			window_name = g_strdup(window_title);
		}
		
	} else if (window_subtitle) {

		if (application_name) {
			window_name = g_strconcat(window_subtitle, " - ", application_name, NULL);
		} else {
			window_name = g_strdup(window_subtitle);
		}
		
	} else if (application_name) {
		window_name = g_strdup(application_name);
	}

	if (window_name) {
		gtk_window_set_title(window, window_name);
	}
}

static void
on_headerbar_title_change(GObject *object, GParamSpec *pspec, gpointer user_data)
{
	GtkHeaderBar *bar = GTK_HEADER_BAR(object);
	GtkWindow *window = GTK_WINDOW(user_data);

	const gchar *title = gtk_header_bar_get_title(bar);
	window_title = g_strdup(title);
	update_window_title(window);
	
	if (title) {
		g_signal_handler_block(bar, title_change_handler);
		gtk_header_bar_set_title(bar, FALSE);
		g_signal_handler_unblock(bar, title_change_handler);
	}
	
}

static void
on_headerbar_subtitle_change(GObject *object, GParamSpec *pspec, gpointer user_data)
{
	GtkHeaderBar *bar = GTK_HEADER_BAR(object);
	GtkWindow *window = GTK_WINDOW(user_data);

	const gchar *subtitle = gtk_header_bar_get_subtitle(bar);
	window_subtitle = g_strdup(subtitle);
	update_window_title(window);

	if (subtitle) {
		g_signal_handler_block(bar, subtitle_change_handler);
		gtk_header_bar_set_subtitle(bar, FALSE);
		g_signal_handler_unblock(bar, subtitle_change_handler);
	}

}


