#include <cassert>
#include <cstdlib>
#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <stdlib.h>
#include "general.h"
#include "glib-object.h"
#include "gtk_error_proc.h"
#include "gtk_gui.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    enum gtk_err error = GTK_OK;

    main_window_t main_window = {};
    main_window_create(&main_window, MATRIX_WIDTH, MATRIX_HEIGHT, &error);
    if (error != GTK_OK) {
        error = GTK_CREATE_MAIN_WINDOW_ERR;
        debug("%s", get_descr(&error));
        return EXIT_FAILURE;
    }

    gtk_widget_show_all(main_window.window);

    gtk_main();

    return 0;
}
