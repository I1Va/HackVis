#include <cassert>
#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <stdlib.h>
#include "gtk_gui.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    main_window_t main_window = {};
    main_window_create(&main_window, MATRIX_WIDTH, MATRIX_HEIGHT);

    gtk_widget_show_all(main_window.window);

    gtk_main();

    return 0;
}
