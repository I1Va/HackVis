#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cairo_animation.h"
#include "glib.h"
#include "glibconfig.h"

MatrixDrop drops[NUM_COLUMNS];

static const int SHAKE_POSITIONS[] = {0, -4, 4, -4, 4, -3, 3, -2, 2, 0};
static const size_t SH_POS_SIZE = sizeof(SHAKE_POSITIONS) / sizeof(int);

void init_matrix_rain() {
    srand(time(NULL));
    for (int i = 0; i < NUM_COLUMNS; i++) {
        drops[i].x = i * 10;
        drops[i].y = rand() % 300;
        drops[i].speed = (rand() % 5) + 2;
    }
}

gboolean update_animation(GtkWidget *widget) {
    for (int i = 0; i < NUM_COLUMNS; i++) {
        drops[i].y += drops[i].speed;
        if (drops[i].y > 300) {
            drops[i].y = 0;
        }
    }
    gtk_widget_queue_draw(widget);
    return TRUE;
}

gboolean draw_matrix_rain(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0, 1, 0);
    cairo_select_font_face(cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);

    for (int i = 0; i < NUM_COLUMNS; i++) {
        cairo_move_to(cr, drops[i].x, drops[i].y);
        char ch = 'A' + (rand() % 26);
        char text[] = {ch, '\0'};
        cairo_show_text(cr, text);
    }

    return FALSE;
}


gboolean shake_window(gpointer window) {
    static size_t index = 0;
    static int x, y;

    if (index == 0) {
        gtk_window_get_position(GTK_WINDOW(window), &x, &y);
        x += SHAKE_FIX_DELTA;
        y += SHAKE_FIX_DELTA;
    }

    gtk_window_move(GTK_WINDOW(window), x + SHAKE_POSITIONS[index], y);

    index++;
    if (index >= SH_POS_SIZE) {
        index = 0;
        return FALSE;
    }

    return TRUE;
}
