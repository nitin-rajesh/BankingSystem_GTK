#define SUP_EVENT_HANDLERS
#include"manager_event_handlers.h"

void on_managers_button_clicked(GtkButton *button, gpointer user_data) {
    clear_list_view();

    DataBlock block = {GET_USERS_BY_ROLE,0,0.0,0,NULL};
    int role = MANAGER;
    copyToPayload(role,block);

    DataBlock *rspBlock = queryBankingServer(&block);

    UserRecord *records = (UserRecord*)rspBlock->payload;

    char msg[256] = "";
    for(int i = 0; records[i].role == role; i++){
        snprintf(msg,256,"Manager %d - %s\t %s \t[%s]",records[i].userId,records[i].username,records[i].emailId,records[i].isActive?"ACTIVE":"INACTIVE"); 
        add_message_to_list(msg);
    }
}

void create_new_user(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        clear_list_view();

        const char *customer_info = gtk_entry_get_text(entry);  
        UserRecord record = {0,"","","New User","",CUSTOMER,ACTIVE};

        char username[128], password[128], email[128];
        int type;
        sscanf(customer_info,"%s %s %s %d",username,password,email,&type);
        strcpy(record.username,username);
        strcpy(record.password,password);
        strcpy(record.emailId,email);
        switch(type){
            case 1: record.role = CUSTOMER; break;
            case 2: record.role = BANKER; break;
            case 3: record.role = MANAGER; break;
            case 4: record.role = ADMIN; break;
            default: record.role = CUSTOMER;
        }

        DataBlock dataBlock = {ADD_USER_ENTRY,0,0.0,0,NULL};
        copyToPayload(record,dataBlock);

        free(queryBankingServer(&dataBlock));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void modify_user(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        clear_list_view();

        const char *customer_info = gtk_entry_get_text(entry);  
        UserRecord record = {0,"","","New User","",CUSTOMER,ACTIVE};

        char username[128], password[128], email[128];
        int userId, type;
        sscanf(customer_info,"%d %s %s %s %d",&userId, username,password,email,&type);
        strcpy(record.username,username);
        strcpy(record.password,password);
        strcpy(record.emailId,email);
        switch(type){
            case 1: record.role = CUSTOMER; break;
            case 2: record.role = BANKER; break;
            case 3: record.role = MANAGER; break;
            case 4: record.role = ADMIN; break;
            default: record.role = CUSTOMER;
        }
        record.userId = userId;
        DataBlock dataBlock = {MODIFY_USER_ENTRY,0,0.0,0,NULL};
        copyToPayload(record,dataBlock);

        free(queryBankingServer(&dataBlock));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void on_add_user_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter user info - <username> <password> <email ID> <type [1:4]>",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new(); 

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(create_new_user), entry);

    gtk_widget_show_all(dialog);
}

void on_modify_user_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter user info - <user ID> <username> <password> <email ID> <type [1:4]>",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new(); 

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(modify_user), entry);

    gtk_widget_show_all(dialog);
}