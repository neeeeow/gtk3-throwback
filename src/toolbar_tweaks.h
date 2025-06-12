#ifndef __TOOLBAR_TWEAKS__
#define __TOOLBAR_TWEAKS__

#include <gtk/gtk.h>

extern GHashTable *label_icon_map;
extern GHashTable *icon_label_map;

void override_toolbar_class(void);

#endif /* __TOOLBAR_TWEAKS__ */
