#include "button_tweaks.h"

static int button_icon_size;

static void (*orig_button_realize)(GtkWidget*);
static void button_realize(GtkButton*);

static void on_button_image_change(GObject*, GParamSpec*, gpointer);
static void on_button_label_change(GObject*, GParamSpec*, gpointer);

static void resize_button_image(GtkImage*);
static void add_button_image(GtkButton*);

void
override_button_class(void)
{
	/*
	  This function overrides the GtkButton class, allowing us to hook in to the realize
	  class function and add custom behaviour immediately before the button is realized.
	*/

	const gchar *button_icon_size_env = g_getenv("GTK_BUTTON_ICON_SIZE");
	if (button_icon_size_env) {
		char *endptr;
		button_icon_size = (int)strtol(button_icon_size_env, &endptr, 10);
		if (*endptr || button_icon_size<=0) {
			g_warning("Invalid icon size! Defaulting to 20px...\n");
		    button_icon_size = 20;
		}
	} else {
		button_icon_size = 20;
	}
	
	// Retrieve the GtkWidgetClass
	GtkWidgetClass *button_class = GTK_WIDGET_CLASS(g_type_class_ref(GTK_TYPE_BUTTON));

	// Store the original realize function and replace it
	orig_button_realize = button_class->realize;
	button_class->realize = (void (*)(GtkWidget*))button_realize;
	
	g_type_class_unref(button_class);
	
}

static void
button_realize(GtkButton *button)
{

	GtkWidget *image = gtk_button_get_image(button);
	const gchar *label = gtk_button_get_label(button);

	if (image && GTK_IS_IMAGE(image)) { // Check if a valid GtkImage exists
		resize_button_image(GTK_IMAGE(image)); // Resize the button image from 16px to 20px
	} else if (label) {
		add_button_image(button);
	}	

	// Add handlers in case the label or image is updated
	g_signal_connect(button, "notify::image", G_CALLBACK(on_button_image_change), NULL);
	g_signal_connect(button, "notify::label", G_CALLBACK(on_button_label_change), NULL);
	
	orig_button_realize(GTK_WIDGET(button));
}

static void
on_button_image_change(GObject *object, GParamSpec *pspec, gpointer user_data)
{
	GtkButton *button = GTK_BUTTON(object);
	GtkWidget *image = gtk_button_get_image(button);
	if (image && GTK_IS_IMAGE(image)) {
		resize_button_image(GTK_IMAGE(image));
	}
}

static void
on_button_label_change(GObject *object, GParamSpec *pspec, gpointer user_data)
{
	add_button_image(GTK_BUTTON(object));
}

static void
resize_button_image(GtkImage *image)
{

	// Retrieve the icon size of the button
	GtkIconSize icon_size;
	g_object_get(image, "icon-size", &icon_size, NULL);

	if (icon_size == GTK_ICON_SIZE_BUTTON) {

		switch (gtk_image_get_storage_type(image)) {
		case GTK_IMAGE_ICON_NAME:
			// If the storage type is GTK_IMAGE_ICON_NAME, resizing icons is easy

			if (gtk_image_get_pixel_size(image) == -1) {
				// If the pixel size != -1, then it has already been set by an application and
				// thus shouldn't be changed
				gtk_image_set_pixel_size(image, button_icon_size);
			}

			break;

		case GTK_IMAGE_STOCK:
			// If the storage type is GTK_IMAGE_STOCK, things are slightly more complicated.
			// Stock images do not support changing the pixel size. To be able to get a 20px
			// icon, replace the GTK_IMAGE_STOCK image with a 20px GTK_IMAGE_ICON_NAME image.

			gchar *stock_id;
			gtk_image_get_stock(image, &stock_id, &icon_size);

			const gchar *icon_name = g_strdup(stock_id);

			if (icon_name) {
				// Only replace the icon if the new icon name is not NULL
				gtk_image_set_from_icon_name(image, icon_name, GTK_ICON_SIZE_BUTTON);
				gtk_image_set_pixel_size(image, button_icon_size);
				g_free((gchar*)icon_name);
			}

			break;
		}
		
	}
}

static void
add_button_image(GtkButton *button)
{
	/*
	  Checks the button's label and applies the image corresponding to the button's label
	*/

	const gchar *label = gtk_button_get_label(button);
	if (!label) {
		return;
	}
	
	const gchar *icon_name = g_hash_table_lookup(label_icon_map, label);

	if (icon_name) {
		GtkWidget *image = gtk_image_new_from_icon_name(icon_name, GTK_ICON_SIZE_BUTTON);
		gtk_image_set_pixel_size(GTK_IMAGE(image), button_icon_size);
		gtk_button_set_image(button, image);
	} 
	
}
