#include <cassert>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cairo.h"
#include "glib.h"
#include "glibconfig.h"
#include "HackVis/cairo_animation.h"

gboolean update_animation(gpointer data) {
    g_assert(data);

    matrix_anim_data_t *matrix_anim_data = (matrix_anim_data_t *) data;

    for (int i = 0; i < NUM_COLUMNS; i++) {
        matrix_anim_data->drops[i].y += matrix_anim_data->drops[i].speed;
        if (matrix_anim_data->drops[i].y > MATRIX_HEIGHT) {
            matrix_anim_data->drops[i].y = 0;
        }
    }

    gtk_widget_queue_draw(matrix_anim_data->drawing_area);
    return TRUE;
}


gboolean draw_matrix_rain(GtkWidget *widget, cairo_t *cr, gpointer data) {
    g_assert(data);
    g_assert(widget);
    g_assert(cr);

    matrix_anim_data_t *matrix_anim_data = (matrix_anim_data_t *) data;

    cairo_set_source_rgb(cr, MATRIX_BACK_RGB[0], MATRIX_BACK_RGB[1], MATRIX_BACK_RGB[2]);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, MATRIX_LET_RGB[0], MATRIX_LET_RGB[1], MATRIX_LET_RGB[2]);
    cairo_select_font_face(cr, MATRIX_FONT, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, MATRIX_FONT_SIZE);

    for (int i = 0; i < NUM_COLUMNS; i++) {
        cairo_move_to(cr, matrix_anim_data->drops[i].x, matrix_anim_data->drops[i].y);
        char ch = 'A' + (rand() % ENG_LETTERS_CNT);
        char text[] = {ch, '\0'};
        cairo_show_text(cr, text);
    }

    return FALSE;
}

void init_matrix_anim(matrix_anim_data_t *matrix_anim_data, enum gtk_err *error=NULL) {
    g_assert(matrix_anim_data);
    ;

    srand(time(NULL));

    for (int i = 0; i < NUM_COLUMNS; i++) {
        matrix_anim_data->drops[i].x = i * MATRIX_X_SCALE;
        matrix_anim_data->drops[i].y = rand() % MATRIX_HEIGHT;
        matrix_anim_data->drops[i].speed = (rand() % MATRIX_MOD_SPEED) + MATRIX_MIN_SPEDD;
    }

    matrix_anim_data->drawing_area = gtk_drawing_area_new();
    HACKVIS_CHECK_REFERENCE(matrix_anim_data->drawing_area, error, GTK_DRAWING_AREA_NEW_ERR)

    gtk_widget_set_size_request(matrix_anim_data->drawing_area, MATRIX_WIDTH, MATRIX_HEIGHT);
    g_signal_connect(G_OBJECT(matrix_anim_data->drawing_area), "draw", G_CALLBACK(draw_matrix_rain), matrix_anim_data);
    g_timeout_add(MATRIX_RAIN_TIMEOUT, (GSourceFunc) update_animation, matrix_anim_data);
}

gboolean shake_window(gpointer window) {
    g_assert(window);

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
