#ifndef GTK_GUI_H
#define GTK_GUI_H

#include <gtk/gtk.h>
#include "HackVis/cairo_animation.h"

const size_t FIR_SEC_ANIM_DURATION = 500;
const size_t GTK_BOX_PASSING = 10;

struct start_dialog_box_t {
    GtkWidget *screen;
    GtkWidget *button;
    GtkWidget *box;
};

struct hacking_box_t {
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
    start_dialog_box_t     start_dialog_box;
    hacking_box_t    hacking_box;
};

void check_entered_name(GtkWidget *entry, gpointer data);
void on_button_clicked(GtkWidget *widget, gpointer data);
void start_dialog_box_create(main_window_t *main_window, enum gtk_err *error=NULL);
void hacking_box_create(main_window_t *main_window, enum gtk_err *error=NULL);
void main_window_create(main_window_t *main_window,  const size_t window_width, const size_t window_height, enum gtk_err *error=NULL);

#endif // GTK_GUI_H