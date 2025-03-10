#include <cassert>
#include <gtk/gtk.h>
#include <cairo.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bin_patcher.h"
#include "cairo_animation.h"
#include "glib-object.h"
#include "glib.h"


GtkWidget *window;
GtkWidget *status_bar;



static void check_entered_name(GtkWidget *entry, gpointer data) {
    const gchar *file_path = gtk_entry_get_text(GTK_ENTRY(entry));

    int crackme_sz = get_file_sz(file_path);
    if (crackme_sz < 0) {
        gtk_statusbar_push(GTK_STATUSBAR(status_bar), GPOINTER_TO_INT(data), "can't open file");
        printf("can't open file '%s'\n", file_path);
        g_timeout_add(50, shake_window, window);
        return;
    }

    FILE *crackme_file_ptr = fopen(file_path, "r");
    if (crackme_file_ptr == NULL) {
        printf("can't open file '%s'\n", file_path);
        gtk_statusbar_push( GTK_STATUSBAR(status_bar), GPOINTER_TO_INT(data), "can't open file");
        g_timeout_add(50, shake_window, window);
        return;
    }

    enum proc_crackme_states hack_res = proccess_crackme_file(crackme_file_ptr, crackme_sz);
    switch (hack_res) {
        case WRONG_HASH:
            gtk_statusbar_push( GTK_STATUSBAR(status_bar), GPOINTER_TO_INT(data), "the file is not suitable for hacking");
            break;
        case HACK_DONE:
            gtk_statusbar_push( GTK_STATUSBAR(status_bar), GPOINTER_TO_INT(data), "HACK IS DONE: crackedfile.com was created");
            break;
    }

    fclose(crackme_file_ptr);
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *stack = GTK_WIDGET(data);
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "second_screen");
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    matrix_anim_data_t matrix_anim_data = {};

    init_matrix_anim(&matrix_anim_data);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "HackVis");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);




    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 500);

    gtk_container_add(GTK_CONTAINER(window), stack);

    GtkWidget *first_screen = gtk_overlay_new();
    gtk_widget_set_name(first_screen, "first_screen");
    gtk_stack_add_named(GTK_STACK(stack), first_screen, "first_screen");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    GtkWidget *button = gtk_button_new_with_label("Start hacking");
    gtk_widget_set_name(button, "next_button");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), stack);

    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 10);
    gtk_overlay_add_overlay(GTK_OVERLAY(first_screen), matrix_anim_data.drawing_area);
    gtk_overlay_add_overlay(GTK_OVERLAY(first_screen), box);


    GtkWidget *second_screen = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(second_screen, "second_screen");
    gtk_stack_add_named(GTK_STACK(stack), second_screen, "second_screen");

    GtkWidget *image = gtk_image_new_from_file("imgs/kap.png");

    status_bar = gtk_statusbar_new();
    GtkWidget *entry = gtk_entry_new();

    g_object_ref_sink(image);
    g_object_ref_sink(status_bar);


    g_signal_connect(entry, "activate", G_CALLBACK(check_entered_name), NULL);

    gtk_box_pack_start(GTK_BOX(second_screen), image, TRUE, TRUE, 10);
    // gtk_box_pack_start(GTK_BOX(second_screen), entry, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(second_screen), status_bar, FALSE, FALSE, 10);

    gtk_widget_show_all(window);


    gtk_main();

    return 0;
}