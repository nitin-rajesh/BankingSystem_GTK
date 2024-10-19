#include<gtk/gtk.h>
#include<string.h>
#include"banking_client.h" 


void show_alert(GtkWidget *parent_window, char *alertmsg) {
    GtkWidget *dialog;

    // Create a new message dialog
    dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_OK,
                                    "Invalid login credentials");

    gtk_window_set_title(GTK_WINDOW(dialog), "Alert");

    // Run the dialog and wait for a response
    gtk_dialog_run(GTK_DIALOG(dialog));

    // Destroy the dialog after closing
    gtk_widget_destroy(dialog);
}

void open_new_window(GtkWidget *first_window) {
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;
    gtk_widget_destroy(first_window);
    // Load the UI file
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "bankpages/home_page.ui", &error) == 0) {
        g_printerr("Error loading UI file: %s\n", error->message);
        g_clear_error(&error);
    }

    // Get the main window and connect the destroy signal
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
}

void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    // Retrieve widgets from the builder
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkEntry *username_entry = GTK_ENTRY(gtk_builder_get_object(builder, "username_entry"));
    GtkEntry *password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry"));
    GtkComboBoxText *user_type_combo = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "user_type_combo"));

    GtkWidget *first_window = GTK_WIDGET(gtk_builder_get_object(builder, "first_window"));
    // Get text from the entries and combo box
    const gchar *username = gtk_entry_get_text(username_entry);
    const gchar *password = gtk_entry_get_text(password_entry);
    const gchar *user_type = gtk_combo_box_text_get_active_text(user_type_combo);


    // Print the collected data
    g_print("Username: %s\n", username);
    g_print("Password: %s\n", password);
    g_print("User Type: %s\n", user_type ? user_type : "None");

    if(validateUser(username,password))
        open_new_window(first_window);

    else show_alert(first_window,"Invalid login credentials");

}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;

    initBankingClient();

    //Init GTK
    gtk_init(&argc, &argv);

    //Load XML UI file
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "bankpages/login_page.ui", &error) == 0) {
        g_printerr("Error loading UI file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Get main window and connect destroy signal
    window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //Connect login button to callback function
    GtkWidget *login_button = GTK_WIDGET(gtk_builder_get_object(builder, "login_button"));
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), builder);

    gtk_widget_show_all(window);

    gtk_main();

    g_object_unref(builder);

    return 0;
}