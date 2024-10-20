#define BANKER_EVENT_HANDLERS
#include"banker_event_handlers.h"

void assignLoan(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        int loanId, bankerId;
        const char *transfer = gtk_entry_get_text(entry);  
        sscanf(transfer,"%d:%d",&loanId,&bankerId);
        DataBlock block = {ASSIGN_BANKER_FOR_LOAN,loanId,0.0,0,NULL};
        copyToPayload(bankerId,block);
        free(queryBankingServer(&block));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  

}

void activateUser(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {

        const char *username = gtk_entry_get_text(entry);  
        DataBlock block = {GET_USER_BY_NAME,0,0,0,NULL};
        char unameArr[strlen(username)];
        strcpy(unameArr,username);
        copyArrToPayload(unameArr,block);

        DataBlock *logs = queryBankingServer(&block);

        UserRecord *rec = (UserRecord*)logs->payload;
        rec->isActive = ACTIVE;
        UserRecord data = *rec;

        block.crudOp = MODIFY_USER_ENTRY;
        block.id = rec->userId;
        copyToPayload(data,block);

        free(queryBankingServer(&block));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void deactivateUser(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {

        const char *username = gtk_entry_get_text(entry);  
        DataBlock block = {GET_USER_BY_NAME,0,0,0,NULL};
        char unameArr[strlen(username)];
        strcpy(unameArr,username);
        copyArrToPayload(unameArr,block);

        DataBlock *logs = queryBankingServer(&block);

        UserRecord *rec = (UserRecord*)logs->payload;
        rec->isActive = INACTIVE;
        UserRecord data = *rec;

        block.crudOp = MODIFY_USER_ENTRY;
        block.id = rec->userId;
        copyToPayload(data,block);

        free(queryBankingServer(&block));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void on_bankers_button_clicked(GtkButton *button, gpointer user_data) {
    clear_list_view();

    DataBlock block = {GET_USERS_BY_ROLE,0,0.0,0,NULL};
    int role = BANKER;
    copyToPayload(role,block);

    DataBlock *rspBlock = queryBankingServer(&block);

    UserRecord *records = (UserRecord*)rspBlock->payload;

    char msg[256] = "";
    for(int i = 0; records[i].role == role; i++){
        snprintf(msg,256,"Banker %d - %s\t %s \t[%s]",records[i].userId,records[i].username,records[i].emailId,records[i].isActive?"ACTIVE":"INACTIVE"); 
        add_message_to_list(msg);
    }
}

void on_activate_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter username to activate",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new(); 

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(activateUser), entry);

    gtk_widget_show_all(dialog);
 
}

void on_deactivate_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter username to deactivate",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new(); 

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(deactivateUser), entry);

    gtk_widget_show_all(dialog);
}

void on_assign_loan_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Assign banker - <loan_id>:<banker_id>",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new(); 

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(assignLoan), entry);

    gtk_widget_show_all(dialog);
}