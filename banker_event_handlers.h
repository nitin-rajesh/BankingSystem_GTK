#define BANKER_EVENT_HANDLERS
#include"customer_event_handlers.h"


void process_loan(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        clear_list_view();

        const char *uid = gtk_entry_get_text(entry);  
        DataBlock block = {PROCESS_LOAN,atoi(uid),0,0,NULL};
        DataBlock *logs = queryBankingServer(&block);


        free(queryBankingServer(&dataBlock));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void modify_customer(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        clear_list_view();

        const char *customer_info = gtk_entry_get_text(entry);  
        UserRecord record = {0,"","","New Customer","",CUSTOMER,INACTIVE};

        char username[128], password[128], email[128];
        int uid;
        sscanf(customer_info,"%d %s %s %s",&uid,username,password,email);
        record.userId = uid;
        strcpy(record.username,username);
        strcpy(record.password,password);
        strcpy(record.emailId,email);

        DataBlock dataBlock = {MODIFY_USER_ENTRY,0,0.0,0,NULL};
        copyToPayload(record,dataBlock);

        free(queryBankingServer(&dataBlock));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void create_new_customer(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        clear_list_view();

        const char *customer_info = gtk_entry_get_text(entry);  
        UserRecord record = {0,"","","New Customer","",CUSTOMER,INACTIVE};

        char username[128], password[128], email[128];
        sscanf(customer_info,"%s %s %s",username,password,email);
        strcpy(record.username,username);
        strcpy(record.password,password);
        strcpy(record.emailId,email);

        DataBlock dataBlock = {ADD_USER_ENTRY,0,0.0,0,NULL};
        copyToPayload(record,dataBlock);

        free(queryBankingServer(&dataBlock));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void get_customer_txn_history(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        clear_list_view();

        const char *uid = gtk_entry_get_text(entry);  
        DataBlock block = {GET_TXN_HISTORY,atoi(uid),0,0,NULL};
        DataBlock *logs = queryBankingServer(&block);

        TxnLogs *txnLogs = (TxnLogs*)logs->payload;

        char msg[256] = "";

        for(int i = 0; txnLogs[i].userId > 0; i++){
            if((int)txnLogs[i].txnAmount == 0)
                continue;
            snprintf(msg, 256, "User %s - %s:\t%0.2f", uid, get_txn_type_name(txnLogs[i].transaction), txnLogs[i].txnAmount);
            add_message_to_list(msg);
        }
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void on_customers_button_clicked(GtkButton *button, gpointer user_data) {
    clear_list_view();

    DataBlock block = {GET_USERS_BY_ROLE,0,0.0,0,NULL};
    int role = CUSTOMER;
    copyToPayload(role,block);

    DataBlock *rspBlock = queryBankingServer(&block);

    UserRecord *records = (UserRecord*)rspBlock->payload;

    char msg[256] = "";
    for(int i = 0; records[i].role == role; i++){
        snprintf(msg,256,"Customer %d - %s\t %s \t[%s]",records[i].userId,records[i].username,records[i].emailId,records[i].isActive?"ACTIVE":"INACTIVE"); 
        add_message_to_list(msg);
    }
}

// Handler for "Loans" button
void on_loans_button_clicked(GtkButton *button, gpointer user_data) {
    clear_list_view();

    DataBlock block = {GET_ACTIVE_LOANS,0,0.0,0,NULL};

    DataBlock *rspBlock = queryBankingServer(&block);

    LoanData *records = (LoanData*)rspBlock->payload;

    char msg[256] = "";
    for(int i = 0; records[i].loanApproved >= 0; i++){
        if((int)records[i].loanId == 0)
            continue;
        snprintf(msg,256,"Loan %d: Acc %d - Request amt: %0.2f  [Banker:%d]",
            records[i].loanId,records[i].userId,records[i].loanRequest,records[i].assignedBankerId); 
        add_message_to_list(msg);
    }
}

// Handler for "Transactions" button
void on_transactions_button_clicked(GtkButton *button, gpointer user_data) {

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter customer ID for txn history",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new(); 

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(get_customer_txn_history), entry);

    gtk_widget_show_all(dialog);
}

// Handler for "Add Customer" button
void on_add_customer_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter customer info - <username> <password> <email ID>",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new(); 

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(create_new_customer), entry);

    gtk_widget_show_all(dialog);
}

// Handler for "Modify Customer" button
void on_mod_customer_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter customer info - <id> <username> <password> <email ID>",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new(); 

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(modify_customer), entry);

    gtk_widget_show_all(dialog);

}

// Handler for "Process Loan" button
void on_process_loan_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter customer info - <id> <username> <password> <email ID>",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new(); 

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(process_loan), entry);

    gtk_widget_show_all(dialog);
}