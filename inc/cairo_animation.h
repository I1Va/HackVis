#ifndef CAIRO_ANIMATION_H

#include <string.h>
#include <gtk/gtk.h>

const size_t NUM_COLUMNS = 50;

typedef struct {
    int x;
    int y;
    int speed;
} MatrixDrop;

void init_matrix_rain();
gboolean update_animation(GtkWidget *widget);
gboolean draw_matrix_rain(GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean shake_window(gpointer window);

#endif // CAIRO_ANIMATION_H