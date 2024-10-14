#include <gtk/gtk.h>
#include <string.h>
#include "bankcommons/bankdata.h"

void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    // Retrieve widgets from the builder
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkEntry *username_entry = GTK_ENTRY(gtk_builder_get_object(builder, "username_entry"));
    GtkEntry *password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry"));
    GtkComboBoxText *user_type_combo = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "user_type_combo"));

    // Get text from the entries and combo box
    const gchar *username = gtk_entry_get_text(username_entry);
    const gchar *password = gtk_entry_get_text(password_entry);
    const gchar *user_type = gtk_combo_box_text_get_active_text(user_type_combo);

    // Print the collected data
    g_print("Username: %s\n", username);
    g_print("Password: %s\n", password);
    g_print("User Type: %s\n", user_type ? user_type : "None");

    struct UserDetails details;
    strcpy(details.username,username);
    strcpy(details.password,password);

    if(strcmp(user_type,"Customer")==0)
        details.role = CUSTOMER;
    else if(strcmp(user_type,"Admin")==0)
        details.role = ADMIN;
    else if(strcmp(user_type,"Banker")==0)
        details.role = BANKER;
    else if(strcmp(user_type,"Manager")==0)
        details.role = MANAGER;
    else 
        details.role = NA;

}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Load the XML UI file
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "bankpages/login_page.ui", &error) == 0) {
        g_printerr("Error loading UI file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Get the main window and connect the destroy signal
    window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Connect the login button to its callback function
    GtkWidget *login_button = GTK_WIDGET(gtk_builder_get_object(builder, "login_button"));
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), builder);

    // Show the window and all its child widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    // Free the builder object
    g_object_unref(builder);

    return 0;
}