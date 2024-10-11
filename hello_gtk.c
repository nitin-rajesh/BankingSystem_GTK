#include <gtk/gtk.h>
#include "bankdata.h"

int main(int argc, char *argv[]) {
    GtkWidget *window;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Set window properties
    gtk_window_set_title(GTK_WINDOW(window), "GTK Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Show the window
    gtk_widget_show(window);

    // GTK main loop
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();

    return 0;
}

