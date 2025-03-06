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
#include "gdk/gdk.h"
#include "gio/gio.h"
#include "glib-object.h"
#include "gtk/gtkcssprovider.h"

GtkWidget *window;
GtkWidget *status_bar;

void check_entered_name(GtkWidget *entry, gpointer data) {
    const gchar *file_path = gtk_entry_get_text(GTK_ENTRY(entry));

    int crackme_sz = get_file_sz(file_path);
    if (crackme_sz < 0) {
        gtk_statusbar_push( GTK_STATUSBAR(status_bar), GPOINTER_TO_INT(data), "can't open file");
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

void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *stack = GTK_WIDGET(data);
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "second_screen");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    init_matrix_rain();

    GtkCssProvider *provider = gtk_css_provider_new();
    const gchar *css_style_file = "./css/style.css";
    GFile *css_fp               = g_file_new_for_path(css_style_file);
    GError *error               = 0;
    gtk_css_provider_load_from_file(provider, css_fp, &error);


    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_window_set_title(GTK_WINDOW(window), "HackVis");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 500);
    gtk_container_add(GTK_CONTAINER(window), stack);

    GtkWidget *first_screen = gtk_overlay_new();
    gtk_widget_set_name(first_screen, "first_screen");

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 400, 300);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_matrix_rain), NULL);
    g_timeout_add(100, (GSourceFunc)update_animation, drawing_area);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);


    GtkWidget *button = gtk_button_new_with_label("Start hacking");
    context = gtk_widget_get_style_context(button);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_name(button, "next_button");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), stack);

    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 10);

    gtk_overlay_add_overlay(GTK_OVERLAY(first_screen), drawing_area);
    gtk_overlay_add_overlay(GTK_OVERLAY(first_screen), box);
    gtk_stack_add_named(GTK_STACK(stack), first_screen, "first_screen");

    GtkWidget *second_screen = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(second_screen, "second_screen");

    GtkWidget *image = gtk_image_new_from_file("imgs/kap.png");

    GtkWidget *entry = gtk_entry_new();
    status_bar = gtk_statusbar_new();

    g_signal_connect(entry, "activate", G_CALLBACK(check_entered_name), NULL);

    gtk_box_pack_start(GTK_BOX(second_screen), image, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(second_screen), entry, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(second_screen), status_bar, FALSE, FALSE, 10);

    gtk_stack_add_named(GTK_STACK(stack), second_screen, "second_screen");
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "first_screen");























    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
