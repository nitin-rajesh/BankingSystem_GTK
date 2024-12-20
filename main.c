#include<gtk/gtk.h>
#include<string.h>
#include"banking_client.h" 
#include"sup_event_handlers.h"


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

void initTreeViewAndLabel(GtkBuilder *builder, GtkWidget *window){
    GtkTreeView *tree_view;

    // Get the TreeView widget from the builder
    tree_view = GTK_TREE_VIEW(gtk_builder_get_object(builder, "list_view"));

    // Create a ListStore model with one column of type string
    list_store = gtk_list_store_new(1, G_TYPE_STRING);
    // Set the ListStore as the model for the TreeView
    gtk_tree_view_set_model(tree_view, GTK_TREE_MODEL(list_store));

    // Create a column for the TreeView
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
        "Messages", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(tree_view, column);

    GtkLabel *balance_label = GTK_LABEL(gtk_builder_get_object(builder, "balance_label"));
    char msg[128] = "";
    snprintf(msg,128,"Account %d",getUserId());
    gtk_label_set_text(balance_label,msg);

}

void open_new_customer_window(GtkWidget *first_window) {
    const char *ui_page = "bankpages/customer_home.ui";
    GtkBuilder *builder;
    GtkWidget *window;
    
    builder = gtk_builder_new_from_file(ui_page);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    initTreeViewAndLabel(builder,window);

    // Connect buttons to their event handlers
    g_signal_connect(gtk_builder_get_object(builder, "withdraw_button"), "clicked", G_CALLBACK(on_withdraw_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "deposit_button"), "clicked", G_CALLBACK(on_deposit_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "transfer_button"), "clicked", G_CALLBACK(on_transfer_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "loan_req_button"), "clicked", G_CALLBACK(on_loan_req_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "transactions_button"), "clicked", G_CALLBACK(on_transactions_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "loans_button"), "clicked", G_CALLBACK(on_loans_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "refresh_button"), "clicked", G_CALLBACK(on_refresh_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "feedback_button"), "clicked", G_CALLBACK(on_feedback_clicked), NULL);

    // Connect the destroy signal to quit the GTK main loop
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show the window
    gtk_widget_show_all(window);
}

void open_new_banker_window(GtkWidget *first_window) {
    const char *ui_page = "bankpages/banker_home.ui";
    GtkBuilder *builder;
    GtkWidget *window;
    
    builder = gtk_builder_new_from_file(ui_page);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    initTreeViewAndLabel(builder,window);

    // Connect buttons to their event handlers
    g_signal_connect(gtk_builder_get_object(builder, "customers_button"), "clicked", G_CALLBACK(on_customers_button_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "loans_button"), "clicked", G_CALLBACK(on_loans_button_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "transactions_button"), "clicked", G_CALLBACK(on_transactions_button_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "add_customer"), "clicked", G_CALLBACK(on_add_customer_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "mod_customer"), "clicked", G_CALLBACK(on_mod_customer_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "process_loan"), "clicked", G_CALLBACK(on_process_loan_clicked), NULL);

    // Connect the destroy signal to quit the GTK main loop
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show the window
    gtk_widget_show_all(window);
}

void open_new_manager_window(GtkWidget *first_window) {
    const char *ui_page = "bankpages/manager_home.ui";
    GtkBuilder *builder;
    GtkWidget *window;
    
    builder = gtk_builder_new_from_file(ui_page);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    initTreeViewAndLabel(builder,window);

    // Connect buttons to their event handlers
    g_signal_connect(gtk_builder_get_object(builder, "customers_button"), "clicked", G_CALLBACK(on_customers_button_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "bankers_button"), "clicked", G_CALLBACK(on_bankers_button_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "loans_button"), "clicked", G_CALLBACK(on_loans_button_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "activate_customer"), "clicked", G_CALLBACK(on_activate_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "deactivate_customer"), "clicked", G_CALLBACK(on_deactivate_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "assign_loan"), "clicked", G_CALLBACK(on_assign_loan_clicked), NULL);

    // Connect the destroy signal to quit the GTK main loop
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show the window
    gtk_widget_show_all(window);
}

void open_new_admin_window(GtkWidget *first_window) {
    const char *ui_page = "bankpages/admin_home.ui";
    GtkBuilder *builder;
    GtkWidget *window;
    
    builder = gtk_builder_new_from_file(ui_page);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    initTreeViewAndLabel(builder,window);

    // Connect buttons to their event handlers
    g_signal_connect(gtk_builder_get_object(builder, "customers_button"), "clicked", G_CALLBACK(on_customers_button_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "bankers_button"), "clicked", G_CALLBACK(on_bankers_button_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "managers_button"), "clicked", G_CALLBACK(on_managers_button_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "add_user"), "clicked", G_CALLBACK(on_add_user_clicked), NULL);
    g_signal_connect(gtk_builder_get_object(builder, "mod_user"), "clicked", G_CALLBACK(on_modify_user_clicked), NULL);

    // Connect the destroy signal to quit the GTK main loop
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show the window
    gtk_widget_show_all(window);
}

void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    // Retrieve widgets from the builder
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkEntry *username_entry = GTK_ENTRY(gtk_builder_get_object(builder, "username_entry"));
    GtkEntry *password_entry = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry"));
    GtkComboBox *user_type_combo = GTK_COMBO_BOX(gtk_builder_get_object(builder, "user_type_combo"));

    GtkWidget *first_window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    // Get text from the entries and combo box
    const gchar *username = gtk_entry_get_text(username_entry);
    const gchar *password = gtk_entry_get_text(password_entry);
    const gint user_type = gtk_combo_box_get_active(user_type_combo);


    // Print the collected data
    g_print("Username: %s\n", username);
    //g_print("Password: %s\n", password);
    g_print("User Type: %d\n", user_type);

    Usertype role;

    if((role = validateUser(username,password))){
        if(role == ADMIN)
            role = user_type+1;

        switch(role){
            case CUSTOMER: {
                open_new_customer_window(first_window);
                break;
            }
            case BANKER: {
                open_new_banker_window(first_window);
                break;
            }    
            case MANAGER: {
                open_new_manager_window(first_window);
                break;
            }    
            case ADMIN: {
                open_new_admin_window(first_window);
                break;
            }    
        }
        gtk_widget_set_sensitive(first_window, FALSE);
    }

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