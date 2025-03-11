#include <gtk/gtk.h>
#include <cairo.h>

#include "HackVis/general.h"
#include "HackVis/gtk_error_proc.h"
#include "HackVis/gtk_gui.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    enum gtk_err error = GTK_OK;

    main_window_t main_window = {};
    hackvis_main_window_create(&main_window, MATRIX_WIDTH, MATRIX_HEIGHT, &error);
    if (error != GTK_OK) {
        error = GTK_CREATE_MAIN_WINDOW_ERR;
        debug("%s", hackvis_get_descr(&error));
        return EXIT_FAILURE;
    }

    gtk_widget_show_all(main_window.window);

    gtk_main();

    free(NULL);

    return 0;
}
