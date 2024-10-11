#include <gtk/gtk.h>

// Callback function for when the login button is clicked
void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    // Retrieve the user data (widgets) passed to this function
    GtkWidget **widgets = (GtkWidget **)data;
    
    // Read the username from the username_entry
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(widgets[0]));

    // Read the password from the password_entry
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(widgets[1]));

    // Read the selected user type from the dropdown (combo box)
    const gchar *user_type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets[2]));

    // Print the collected information to the console
    g_print("Username: %s\n", username);
    g_print("Password: %s\n", password);
    g_print("User Type: %s\n", user_type ? user_type : "None");

    // Free the string if it exists (to avoid memory leaks in real-world applications)
    if (user_type) {
        g_free((gchar *)user_type);
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *username_label, *password_label, *user_type_label;
    GtkWidget *username_entry, *password_entry, *user_type_combo;
    GtkWidget *login_button;
    GtkWidget *vbox; // Box to center the grid
    GtkWidget *title_label; // Title label

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Login Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 250);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to center the content
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);  // Spacing between elements
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER); // Vertically center the box
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER); // Horizontally center the box
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create the title label with large font
    title_label = gtk_label_new(NULL);  // Create an empty label
    gtk_label_set_markup(GTK_LABEL(title_label), "<span font_desc='20'>SS Banking</span>");
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 10);  // Add title label with padding

    // Create a grid to arrange widgets
    grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    // Create username label and entry
    username_label = gtk_label_new("Username:");
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 0, 1, 1);

    username_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 0, 1, 1);

    // Create password label and entry (set entry invisible for password input)
    password_label = gtk_label_new("Password:");
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);

    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Hide the password
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 1, 1, 1);

    // Create user type label and dropdown (combo box)
    user_type_label = gtk_label_new("User Type:");
    gtk_grid_attach(GTK_GRID(grid), user_type_label, 0, 2, 1, 1);

    user_type_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(user_type_combo), NULL, "Customer");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(user_type_combo), NULL, "Banker");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(user_type_combo), NULL, "Manager");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(user_type_combo), NULL, "Admin");

    gtk_grid_attach(GTK_GRID(grid), user_type_combo, 1, 2, 1, 1);

    // Create login button
    login_button = gtk_button_new_with_label("Login");

    // Pack widgets (entries and combo box) into an array to pass to the callback
    GtkWidget *widgets[3] = {username_entry, password_entry, user_type_combo};

    // Connect the login button "clicked" event to the callback function
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), widgets);
    gtk_grid_attach(GTK_GRID(grid), login_button, 1, 3, 1, 1);

    // Show all widgets in the window
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
