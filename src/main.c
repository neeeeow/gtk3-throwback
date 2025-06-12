#include <gtk/gtk.h>

#include "window_tweaks.h"
#include "button_tweaks.h"
#include "menu_item_tweaks.h"
#include "message_dialog_tweaks.h"
#include "toolbar_tweaks.h"
#include "statusbar_tweaks.h"


// Hash tables for mapping labels to icon names
GHashTable *label_icon_map = NULL;
GHashTable *icon_label_map = NULL;

G_MODULE_EXPORT void
gtk_module_init(gint *argc, gchar ***argv[])
{

	// Initialise and populate our hash tables
	if (!label_icon_map) {
		label_icon_map = g_hash_table_new(g_str_hash, g_str_equal);
		g_hash_table_insert(label_icon_map, "_About", "help-about");
		g_hash_table_insert(label_icon_map, "_Add", "list-add");
		g_hash_table_insert(label_icon_map, "_Apply", "gtk-apply");
		g_hash_table_insert(label_icon_map, "_Bold", "format-text-bold");
		g_hash_table_insert(label_icon_map, "_Cancel", "gtk-cancel");
		g_hash_table_insert(label_icon_map, "_CD-ROM", "media-optical");
		g_hash_table_insert(label_icon_map, "_Clear", "edit-clear");
		g_hash_table_insert(label_icon_map, "_Close", "window-close");
		g_hash_table_insert(label_icon_map, "_Convert", "gtk-convert");
		g_hash_table_insert(label_icon_map, "C_onnect", "gtk-connect");
		g_hash_table_insert(label_icon_map, "_Copy", "edit-copy");
		g_hash_table_insert(label_icon_map, "C_redits", "help-about");
		g_hash_table_insert(label_icon_map, "Cu_t", "edit-cut");
		g_hash_table_insert(label_icon_map, "_Delete", "edit-delete");
		g_hash_table_insert(label_icon_map, "_Discard", "gtk-discard");
		g_hash_table_insert(label_icon_map, "_Disconnect", "gtk-disconnect");
		g_hash_table_insert(label_icon_map, "_Execute", "system-run");
		g_hash_table_insert(label_icon_map, "_Edit", "gtk-edit");
		g_hash_table_insert(label_icon_map, "_File", "text-x-generic");
		g_hash_table_insert(label_icon_map, "_Find", "edit-find");
		g_hash_table_insert(label_icon_map, "Find and _Replace", "edit-find-replace");
		g_hash_table_insert(label_icon_map, "_Floppy", "media-floppy");
		g_hash_table_insert(label_icon_map, "_Fullscreen", "view-fullscreen");
		g_hash_table_insert(label_icon_map, "_Leave Fullscreen", "view-restore");
		g_hash_table_insert(label_icon_map, "_Bottom", "go-bottom");
		g_hash_table_insert(label_icon_map, "_First", "go-first");
		g_hash_table_insert(label_icon_map, "_Last", "go-last");
		g_hash_table_insert(label_icon_map, "_Top", "go-top");
		g_hash_table_insert(label_icon_map, "_Back", "go-previous");
		g_hash_table_insert(label_icon_map, "_Down", "go-down");
		g_hash_table_insert(label_icon_map, "_Forward", "go-next");
		g_hash_table_insert(label_icon_map, "_Up", "go-up");
		g_hash_table_insert(label_icon_map, "_Hard Disk", "drive-harddisk");
		g_hash_table_insert(label_icon_map, "_Help", "help-contents");
		g_hash_table_insert(label_icon_map, "_Home", "go-home");
		g_hash_table_insert(label_icon_map, "Increase Indent", "format-indent-more");
		g_hash_table_insert(label_icon_map, "Decrease Indent", "format-indent-less");
		g_hash_table_insert(label_icon_map, "_Index", "gtk-index");
		g_hash_table_insert(label_icon_map, "_Information", "dialog-information");
		g_hash_table_insert(label_icon_map, "_Italic", "format-text-italic");
		g_hash_table_insert(label_icon_map, "_Jump to", "go-jump");
		g_hash_table_insert(label_icon_map, "_Network", "network-idle");
		g_hash_table_insert(label_icon_map, "_New", "document-new");
		g_hash_table_insert(label_icon_map, "_No", "gtk-no");
		g_hash_table_insert(label_icon_map, "_OK", "gtk-ok");
		g_hash_table_insert(label_icon_map, "_Open", "document-open");
		g_hash_table_insert(label_icon_map, "Landscape", "gtk-orientation-landscape");
		g_hash_table_insert(label_icon_map, "Portrait", "gtk-orientation-portrait");
		g_hash_table_insert(label_icon_map, "Reverse landscape", "gtk-orientation-reverse-landscape");
		g_hash_table_insert(label_icon_map, "Reverse portrait", "gtk-orientation-reverse-portrait");
		g_hash_table_insert(label_icon_map, "Page Set_up", "gtk-page-setup");
		g_hash_table_insert(label_icon_map, "_Paste", "edit-paste");
		g_hash_table_insert(label_icon_map, "_Preferences", "gtk-preferences");
		g_hash_table_insert(label_icon_map, "_Print", "document-print");
		g_hash_table_insert(label_icon_map, "Print Pre_view", "document-print-preview");
		g_hash_table_insert(label_icon_map, "_Properties", "document-properties");
		g_hash_table_insert(label_icon_map, "_Quit", "application-exit");
		g_hash_table_insert(label_icon_map, "_Redo", "edit-redo");
		g_hash_table_insert(label_icon_map, "_Refresh", "view-refresh");
		g_hash_table_insert(label_icon_map, "_Remove", "list-remove");
		g_hash_table_insert(label_icon_map, "_Revert", "document-revert");
		g_hash_table_insert(label_icon_map, "_Save", "document-save");
		g_hash_table_insert(label_icon_map, "Save _As", "document-save-as");
		g_hash_table_insert(label_icon_map, "Select _All", "edit-select-all");
		g_hash_table_insert(label_icon_map, "_Color", "gtk-select-color");
		g_hash_table_insert(label_icon_map, "_Font", "gtk-select-font");
		g_hash_table_insert(label_icon_map, "_Ascending", "view-sort-ascending");
		g_hash_table_insert(label_icon_map, "_Descending", "view-sort-descending");
		g_hash_table_insert(label_icon_map, "_Spell Check", "tools-check-spelling");
		g_hash_table_insert(label_icon_map, "_Stop", "process-stop");
		g_hash_table_insert(label_icon_map, "_Strikethrough", "format-text-strikethrough");
		g_hash_table_insert(label_icon_map, "_Undelete", "gtk-undelete");
		g_hash_table_insert(label_icon_map, "_Underline", "format-text-underline");
		g_hash_table_insert(label_icon_map, "_Undo", "edit-undo");
		g_hash_table_insert(label_icon_map, "_Yes", "gtk-yes");
		g_hash_table_insert(label_icon_map, "_Normal Size", "zoom-original");
		g_hash_table_insert(label_icon_map, "Best _Fit", "zoom-fit-best");
		g_hash_table_insert(label_icon_map, "Zoom _In", "zoom-in");
		g_hash_table_insert(label_icon_map, "Zoom _Out", "zoom-out");
	}

	if (!icon_label_map) {
		icon_label_map = g_hash_table_new(g_str_hash, g_str_equal);
		g_hash_table_insert(icon_label_map, "help-about", "_About");
		g_hash_table_insert(icon_label_map, "list-add", "_Add");
		g_hash_table_insert(icon_label_map, "gtk-apply", "_Apply");
		g_hash_table_insert(icon_label_map, "format-text-bold", "_Bold");
		g_hash_table_insert(icon_label_map, "gtk-cancel", "_Cancel");
		g_hash_table_insert(icon_label_map, "media-optical", "_CD-ROM");
		g_hash_table_insert(icon_label_map, "edit-clear", "_Clear");
		g_hash_table_insert(icon_label_map, "window-close", "_Close");
		g_hash_table_insert(icon_label_map, "gtk-convert", "_Convert");
		g_hash_table_insert(icon_label_map, "gtk-connect", "C_onnect");
		g_hash_table_insert(icon_label_map, "edit-copy", "_Copy");
		g_hash_table_insert(icon_label_map, "edit-cut", "Cu_t");
		g_hash_table_insert(icon_label_map, "edit-delete", "_Delete");
		g_hash_table_insert(icon_label_map, "gtk-discard", "_Discard");
		g_hash_table_insert(icon_label_map, "gtk-disconnect", "_Disconnect");
		g_hash_table_insert(icon_label_map, "system-run", "_Execute");
		g_hash_table_insert(icon_label_map, "gtk-edit", "_Edit");
		g_hash_table_insert(icon_label_map, "text-x-generic", "_File");
		g_hash_table_insert(icon_label_map, "edit-find", "_Find");
		g_hash_table_insert(icon_label_map, "edit-find-replace", "Find and _Replace");
		g_hash_table_insert(icon_label_map, "media-floppy", "_Floppy");
		g_hash_table_insert(icon_label_map, "view-fullscreen", "_Fullscreen");
		g_hash_table_insert(icon_label_map, "view-restore", "_Leave Fullscreen");
		g_hash_table_insert(icon_label_map, "go-bottom", "_Bottom");
		g_hash_table_insert(icon_label_map, "go-first", "_First");
		g_hash_table_insert(icon_label_map, "go-last", "_Last");
		g_hash_table_insert(icon_label_map, "go-top", "_Top");
		g_hash_table_insert(icon_label_map, "go-previous", "_Back");
		g_hash_table_insert(icon_label_map, "go-down", "_Down");
		g_hash_table_insert(icon_label_map, "go-next", "_Forward");
		g_hash_table_insert(icon_label_map, "go-up", "_Up");
		g_hash_table_insert(icon_label_map, "drive-harddisk", "_Hard Disk");
		g_hash_table_insert(icon_label_map, "help-contents", "_Help");
		g_hash_table_insert(icon_label_map, "go-home", "_Home");
		g_hash_table_insert(icon_label_map, "format-indent-more", "Increase Indent");
		g_hash_table_insert(icon_label_map, "format-indent-less", "Decrease Indent");
		g_hash_table_insert(icon_label_map, "gtk-index", "_Index");
		g_hash_table_insert(icon_label_map, "dialog-information", "_Information");
		g_hash_table_insert(icon_label_map, "format-text-italic", "_Italic");
		g_hash_table_insert(icon_label_map, "go-jump", "_Jump to");
		g_hash_table_insert(icon_label_map, "network-idle", "_Network");
		g_hash_table_insert(icon_label_map, "document-new", "_New");
		g_hash_table_insert(icon_label_map, "gtk-no", "_No");
		g_hash_table_insert(icon_label_map, "gtk-ok", "_OK");
		g_hash_table_insert(icon_label_map, "document-open", "_Open");
		g_hash_table_insert(icon_label_map, "gtk-orientation-landscape", "Landscape");
		g_hash_table_insert(icon_label_map, "gtk-orientation-portrait", "Portrait");
		g_hash_table_insert(icon_label_map, "gtk-orientation-reverse-landscape", "Reverse landscape");
		g_hash_table_insert(icon_label_map, "gtk-orientation-reverse-portrait", "Reverse portrait");
		g_hash_table_insert(icon_label_map, "gtk-page-setup", "Page Set_up");
		g_hash_table_insert(icon_label_map, "edit-paste", "_Paste");
		g_hash_table_insert(icon_label_map, "gtk-preferences", "_Preferences");
		g_hash_table_insert(icon_label_map, "document-print", "_Print");
		g_hash_table_insert(icon_label_map, "document-print-preview", "Print Pre_view");
		g_hash_table_insert(icon_label_map, "document-properties", "_Properties");
		g_hash_table_insert(icon_label_map, "application-exit", "_Quit");
		g_hash_table_insert(icon_label_map, "edit-redo", "_Redo");
		g_hash_table_insert(icon_label_map, "view-refresh", "_Refresh");
		g_hash_table_insert(icon_label_map, "list-remove", "_Remove");
		g_hash_table_insert(icon_label_map, "document-revert", "_Revert");
		g_hash_table_insert(icon_label_map, "document-save", "_Save");
		g_hash_table_insert(icon_label_map, "document-save-as", "Save _As");
		g_hash_table_insert(icon_label_map, "edit-select-all", "Select _All");
		g_hash_table_insert(icon_label_map, "gtk-select-color", "_Color");
		g_hash_table_insert(icon_label_map, "gtk-select-font", "_Font");
		g_hash_table_insert(icon_label_map, "view-sort-ascending", "_Ascending");
		g_hash_table_insert(icon_label_map, "view-sort-descending", "_Descending");
		g_hash_table_insert(icon_label_map, "tools-check-spelling", "_Spell Check");
		g_hash_table_insert(icon_label_map, "process-stop", "_Stop");
		g_hash_table_insert(icon_label_map, "format-text-strikethrough", "_Strikethrough");
		g_hash_table_insert(icon_label_map, "gtk-undelete", "_Undelete");
		g_hash_table_insert(icon_label_map, "format-text-underline", "_Underline");
		g_hash_table_insert(icon_label_map, "edit-undo", "_Undo");
		g_hash_table_insert(icon_label_map, "gtk-yes", "_Yes");
		g_hash_table_insert(icon_label_map, "zoom-original", "_Normal Size");
		g_hash_table_insert(icon_label_map, "zoom-fit-best", "Best _Fit");
		g_hash_table_insert(icon_label_map, "zoom-in", "Zoom _In");
		g_hash_table_insert(icon_label_map, "zoom-out", "Zoom _Out");
	}

	const gchar *csd_env = g_getenv("GTK_CSD");
	if (csd_env) {

		if (g_strcmp0(csd_env, "0") == 0) {
			override_window_class();
		} else if (g_strcmp0(csd_env, "1") != 0) {
			g_warning("Invalid option for GTK_CSD environment variable. Disabling CSDs...");
			override_window_class();
		}
		
	} else {
		override_window_class();
	}	
	
	override_button_class();
	override_menu_item_class();
	override_message_dialog_class();
	override_toolbar_class();
	override_statusbar_class();
	
}

G_MODULE_EXPORT const gchar *
g_module_check_init(GModule *module)
{
	// Ensures that the module is never unloaded
	g_module_make_resident(module);
	return NULL;
}
