#ifndef GTK_GUI_H
#define GTK_GUI_H

#include <gtk/gtk.h>
#include "cairo_animation.h"

const size_t FIR_SEC_ANIM_DURATION = 500;
const size_t GTK_BOX_PASSING = 10;

struct first_screen_t {
    GtkWidget *status_bar;
    GtkWidget *screen;
    GtkWidget *button;
    GtkWidget *box;
};

struct second_screen_t {
    GtkWidget *screen;
    GtkWidget *image;
    GtkWidget *status_bar;
    GtkWidget *entry;
};

struct main_window_t {
    GtkWidget *window;
    size_t window_width;
    size_t window_height;

    GtkWidget *stack;

    matrix_anim_data_t matrix_anim_data;
    first_screen_t     first_screen;
    second_screen_t    second_screen;
};

void check_entered_name(GtkWidget *entry, gpointer data);
void on_button_clicked(GtkWidget *widget, gpointer data);
void first_screen_create(main_window_t *main_window, enum gtk_err *error=NULL);
void second_screen_create(main_window_t *main_window, enum gtk_err *error=NULL);
void main_window_create(main_window_t *main_window,  const size_t window_width, const size_t window_height, enum gtk_err *error=NULL);

#endif // GTK_GUI_H