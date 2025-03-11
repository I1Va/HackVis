#include <assert.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HackVis/bin_patcher.h"
#include "HackVis/gtk_gui.h"
#include "glib-object.h"
#include "glib.h"

void check_entered_name(GtkWidget *entry, gpointer data) {
    g_assert(entry);
    g_assert(data);

    main_window_t *main_window = (main_window_t *) data;

    const gchar *file_path = gtk_entry_get_text(GTK_ENTRY(entry));

    int crackme_sz = get_file_sz(file_path);
    if (crackme_sz < 0) {
        gtk_statusbar_push(GTK_STATUSBAR(main_window->hacking_box.status_bar), GPOINTER_TO_INT(data), "can't open file");
        printf("can't open file '%s'\n", file_path);
        g_timeout_add(50, shake_window, main_window->window);
        return;
    }

    FILE *crackme_file_ptr = fopen(file_path, "r");
    if (crackme_file_ptr == NULL) {
        printf("can't open file '%s'\n", file_path);
        gtk_statusbar_push( GTK_STATUSBAR(main_window->hacking_box.status_bar), GPOINTER_TO_INT(data), "can't open file");
        g_timeout_add(50, shake_window, main_window->window);
        return;
    }

    enum proc_crackme_states hack_res = proccess_crackme_file(crackme_file_ptr, crackme_sz);
    switch (hack_res) {
        case WRONG_HASH:
            gtk_statusbar_push( GTK_STATUSBAR(main_window->hacking_box.status_bar), GPOINTER_TO_INT(data), "the file is not suitable for hacking");
            break;
        case HACK_DONE:
            gtk_statusbar_push( GTK_STATUSBAR(main_window->hacking_box.status_bar), GPOINTER_TO_INT(data), "HACK IS DONE: crackedfile.com was created");
            break;
    }

    fclose(crackme_file_ptr);
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *stack = GTK_WIDGET(data);
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "hacking_box");
}

void start_dialog_box_create(main_window_t *main_window, enum gtk_err *error) {
    assert(main_window);

    main_window->start_dialog_box.screen = gtk_overlay_new();
    g_object_ref_sink(main_window->start_dialog_box.screen);

    HACKVIS_CHECK_REFERENCE(main_window, error, GTK_OVERLAY_NEW_ERR)

    gtk_widget_set_name(main_window->start_dialog_box.screen, "start_dialog_box");
    gtk_stack_add_named(GTK_STACK(main_window->stack), main_window->start_dialog_box.screen, "start_dialog_box");

    main_window->start_dialog_box.box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    HACKVIS_CHECK_REFERENCE(main_window->start_dialog_box.box, error, GTK_BOX_NEW_ERR)

    gtk_widget_set_halign(main_window->start_dialog_box.box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(main_window->start_dialog_box.box, GTK_ALIGN_CENTER);

    main_window->start_dialog_box.button = gtk_button_new_with_label("Start hacking");
    HACKVIS_CHECK_REFERENCE(main_window->start_dialog_box.button, error, GTK_BUTTON_NEW_ERR)

    gtk_widget_set_name(main_window->start_dialog_box.button, "next_button");
    g_signal_connect(main_window->start_dialog_box.button, "clicked", G_CALLBACK(on_button_clicked), main_window->stack);

    g_object_ref_sink(main_window->start_dialog_box.button);
    g_object_ref_sink(main_window->matrix_anim_data.drawing_area);
    g_object_ref_sink(main_window->start_dialog_box.box);

    gtk_box_pack_start(GTK_BOX(main_window->start_dialog_box.box), main_window->start_dialog_box.button, FALSE, FALSE, 10);
    gtk_overlay_add_overlay(GTK_OVERLAY(main_window->start_dialog_box.screen), main_window->matrix_anim_data.drawing_area);
    gtk_overlay_add_overlay(GTK_OVERLAY(main_window->start_dialog_box.screen), main_window->start_dialog_box.box);
}

void hackvis_hacking_box_create(main_window_t *main_window, enum gtk_err *error) {
    assert(main_window);

    main_window->hacking_box.screen = gtk_box_new(GTK_ORIENTATION_VERTICAL, GTK_BOX_PASSING);
    HACKVIS_CHECK_REFERENCE(main_window->hacking_box.screen, error, GTK_BOX_NEW_ERR)

    gtk_widget_set_name(main_window->hacking_box.screen, "hacking_box");
    gtk_stack_add_named(GTK_STACK(main_window->stack), main_window->hacking_box.screen, "hacking_box");

    main_window->hacking_box.image = gtk_image_new_from_file("imgs/kap.png");
    HACKVIS_CHECK_REFERENCE(main_window->hacking_box.image, error, GTK_IMAGE_NEW_ERR)

    main_window->hacking_box.status_bar = gtk_statusbar_new();
    HACKVIS_CHECK_REFERENCE(main_window->hacking_box.status_bar, error, GTK_STATUSBAR_NEW_ERR)

    main_window->hacking_box.entry = gtk_entry_new();
    HACKVIS_CHECK_REFERENCE(main_window->hacking_box.entry, error, GTK_ENTRY_NEW_ERR)

    g_signal_connect(main_window->hacking_box.entry, "activate", G_CALLBACK(check_entered_name), main_window);

    g_object_ref_sink(main_window->hacking_box.image);
    g_object_ref_sink(main_window->hacking_box.entry);
    g_object_ref_sink(main_window->hacking_box.status_bar);
    gtk_box_pack_start(GTK_BOX(main_window->hacking_box.screen), main_window->hacking_box.image, FALSE, FALSE, GTK_BOX_PASSING);
    gtk_box_pack_start(GTK_BOX(main_window->hacking_box.screen), main_window->hacking_box.entry, FALSE, FALSE, GTK_BOX_PASSING);
    gtk_box_pack_start(GTK_BOX(main_window->hacking_box.screen), main_window->hacking_box.status_bar, TRUE, TRUE, GTK_BOX_PASSING);
}

void hackvis_main_window_create(main_window_t *main_window,  const size_t window_width, const size_t window_height, enum gtk_err *error) {
    g_assert(main_window);

    main_window->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    HACKVIS_CHECK_REFERENCE(main_window->window, error, GTK_WINDOW_NEW_ERR)

    main_window->window_width = window_width;
    main_window->window_height = window_height;

    main_window->stack = gtk_stack_new();
    HACKVIS_CHECK_REFERENCE(main_window->stack, error, GTK_STACK_NEW_ERR)

    init_matrix_anim(&main_window->matrix_anim_data, error);
    HACKVIS_HANDLE_FUNC_ERROR(error, GTK_INIT_MATRIX_ANIM_ERR)

    start_dialog_box_create(main_window, error);
    HACKVIS_HANDLE_FUNC_ERROR(error, GTK_CREATE_START_DIALOG_BOX_ERR)

    hackvis_hacking_box_create(main_window, error);
    HACKVIS_HANDLE_FUNC_ERROR(error, GTK_CREATE_HACKING_BOX)


    gtk_window_set_title(GTK_WINDOW(main_window->window), "HackVis");
    gtk_window_set_default_size(GTK_WINDOW(main_window->window), window_width, window_height);
    g_signal_connect(main_window->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_stack_set_transition_type(GTK_STACK(main_window->stack), GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(main_window->stack), FIR_SEC_ANIM_DURATION);

    g_object_ref_sink(main_window->window);
    gtk_container_add(GTK_CONTAINER(main_window->window), main_window->stack);
}
