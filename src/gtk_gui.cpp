#include <assert.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bin_patcher.h"
#include "gtk_gui.h"
#include "glib-object.h"
#include "glib.h"

void check_entered_name(GtkWidget *entry, gpointer data) {
    g_assert(entry);
    g_assert(data);

    main_window_t *main_window = (main_window_t *) data;

    const gchar *file_path = gtk_entry_get_text(GTK_ENTRY(entry));

    int crackme_sz = get_file_sz(file_path);
    if (crackme_sz < 0) {
        gtk_statusbar_push(GTK_STATUSBAR(main_window->second_screen.status_bar), GPOINTER_TO_INT(data), "can't open file");
        printf("can't open file '%s'\n", file_path);
        g_timeout_add(50, shake_window, main_window->window);
        return;
    }

    FILE *crackme_file_ptr = fopen(file_path, "r");
    if (crackme_file_ptr == NULL) {
        printf("can't open file '%s'\n", file_path);
        gtk_statusbar_push( GTK_STATUSBAR(main_window->second_screen.status_bar), GPOINTER_TO_INT(data), "can't open file");
        g_timeout_add(50, shake_window, main_window->window);
        return;
    }

    enum proc_crackme_states hack_res = proccess_crackme_file(crackme_file_ptr, crackme_sz);
    switch (hack_res) {
        case WRONG_HASH:
            gtk_statusbar_push( GTK_STATUSBAR(main_window->second_screen.status_bar), GPOINTER_TO_INT(data), "the file is not suitable for hacking");
            break;
        case HACK_DONE:
            gtk_statusbar_push( GTK_STATUSBAR(main_window->second_screen.status_bar), GPOINTER_TO_INT(data), "HACK IS DONE: crackedfile.com was created");
            break;
    }

    fclose(crackme_file_ptr);
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *stack = GTK_WIDGET(data);
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "second_screen");
}

void first_screen_create(main_window_t *main_window, enum gtk_err *error) {
    assert(main_window);

    main_window->first_screen.screen = gtk_overlay_new();
    g_object_ref_sink(main_window->first_screen.screen);

    CHECK_REFERENCE(main_window, error, GTK_OVERLAY_NEW_ERR)

    gtk_widget_set_name(main_window->first_screen.screen, "first_screen");
    gtk_stack_add_named(GTK_STACK(main_window->stack), main_window->first_screen.screen, "first_screen");

    main_window->first_screen.box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    CHECK_REFERENCE(main_window->first_screen.box, error, GTK_BOX_NEW_ERR)

    gtk_widget_set_halign(main_window->first_screen.box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(main_window->first_screen.box, GTK_ALIGN_CENTER);

    main_window->first_screen.button = gtk_button_new_with_label("Start hacking");
    CHECK_REFERENCE(main_window->first_screen.button, error, GTK_BUTTON_NEW_ERR)

    gtk_widget_set_name(main_window->first_screen.button, "next_button");
    g_signal_connect(main_window->first_screen.button, "clicked", G_CALLBACK(on_button_clicked), main_window->stack);

    g_object_ref_sink(main_window->first_screen.button);
    g_object_ref_sink(main_window->matrix_anim_data.drawing_area);
    g_object_ref_sink(main_window->first_screen.box);

    gtk_box_pack_start(GTK_BOX(main_window->first_screen.box), main_window->first_screen.button, FALSE, FALSE, 10);
    gtk_overlay_add_overlay(GTK_OVERLAY(main_window->first_screen.screen), main_window->matrix_anim_data.drawing_area);
    gtk_overlay_add_overlay(GTK_OVERLAY(main_window->first_screen.screen), main_window->first_screen.box);
}

void second_screen_create(main_window_t *main_window, enum gtk_err *error) {
    assert(main_window);

    main_window->second_screen.screen = gtk_box_new(GTK_ORIENTATION_VERTICAL, GTK_BOX_PASSING);
    CHECK_REFERENCE(main_window->second_screen.screen, error, GTK_BOX_NEW_ERR)

    gtk_widget_set_name(main_window->second_screen.screen, "second_screen");
    gtk_stack_add_named(GTK_STACK(main_window->stack), main_window->second_screen.screen, "second_screen");

    main_window->second_screen.image = gtk_image_new_from_file("imgs/kap.png");
    CHECK_REFERENCE(main_window->second_screen.image, error, GTK_IMAGE_NEW_ERR)

    main_window->second_screen.status_bar = gtk_statusbar_new();
    CHECK_REFERENCE(main_window->second_screen.status_bar, error, GTK_STATUSBAR_NEW_ERR)

    main_window->second_screen.entry = gtk_entry_new();
    CHECK_REFERENCE(main_window->second_screen.entry, error, GTK_ENTRY_NEW_ERR)

    g_signal_connect(main_window->second_screen.entry, "activate", G_CALLBACK(check_entered_name), main_window);

    g_object_ref_sink(main_window->second_screen.image);
    g_object_ref_sink(main_window->second_screen.entry);
    g_object_ref_sink(main_window->second_screen.status_bar);
    gtk_box_pack_start(GTK_BOX(main_window->second_screen.screen), main_window->second_screen.image, FALSE, FALSE, GTK_BOX_PASSING);
    gtk_box_pack_start(GTK_BOX(main_window->second_screen.screen), main_window->second_screen.entry, FALSE, FALSE, GTK_BOX_PASSING);
    gtk_box_pack_start(GTK_BOX(main_window->second_screen.screen), main_window->second_screen.status_bar, TRUE, TRUE, GTK_BOX_PASSING);
}

void main_window_create(main_window_t *main_window,  const size_t window_width, const size_t window_height, enum gtk_err *error) {
    g_assert(main_window);

    main_window->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    CHECK_REFERENCE(main_window->window, error, GTK_WINDOW_NEW_ERR)

    main_window->window_width = window_width;
    main_window->window_height = window_height;

    main_window->stack = gtk_stack_new();
    CHECK_REFERENCE(main_window->stack, error, GTK_STACK_NEW_ERR)

    init_matrix_anim(&main_window->matrix_anim_data, error);
    HANDLE_FUNC_ERROR(error, GTK_INIT_MATRIX_ANIM_ERR)

    first_screen_create(main_window, error);
    HANDLE_FUNC_ERROR(error, GTK_CREATE_FIRST_SCREEN)

    second_screen_create(main_window, error);
    HANDLE_FUNC_ERROR(error, GTK_CREATE_SECOND_SCREEN)


    gtk_window_set_title(GTK_WINDOW(main_window->window), "HackVis");
    gtk_window_set_default_size(GTK_WINDOW(main_window->window), window_width, window_height);
    g_signal_connect(main_window->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_stack_set_transition_type(GTK_STACK(main_window->stack), GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(main_window->stack), FIR_SEC_ANIM_DURATION);

    g_object_ref_sink(main_window->window);
    gtk_container_add(GTK_CONTAINER(main_window->window), main_window->stack);
}
