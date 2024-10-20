#ifndef CUSTOMER_EVENT_HANDLERS
#define CUSTOMER_EVENT_HANDLERS

#include<gtk/gtk.h>
#include<stdlib.h>
#include"banking_client.h"

// Global pointer to the ListStore model
GtkListStore *list_store;

/* Utility functions */

void deposit_amount_in_bank(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        const char *amount = gtk_entry_get_text(entry);  
        DataBlock block = {DEPOSIT_CASH,getUserId(),atoi(amount),0,NULL};
        free(queryBankingServer(&block));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void withdraw_amount_from_bank(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        const char *amount = gtk_entry_get_text(entry);  
        DataBlock block = {WITHDRAW_CASH,getUserId(),atoi(amount),0,NULL};
        free(queryBankingServer(&block));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void request_for_loan(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        const char *amount = gtk_entry_get_text(entry);  
        DataBlock block = {APPLY_FOR_LOAN,getUserId(),atoi(amount),0,NULL};
        free(queryBankingServer(&block));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  
}

void transfer_funds(GtkDialog *dialog, gint response_id, GtkEntry *entry) {
    if (response_id == GTK_RESPONSE_OK) {
        int destUserId; double amount;
        const char *transfer = gtk_entry_get_text(entry);  
        sscanf(transfer,"%d:%lf",&destUserId,&amount);
        DataBlock block = {TRANSFER_CASH,getUserId(),amount,0,NULL};
        copyToPayload(destUserId,block);
        free(queryBankingServer(&block));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));  

}

// Fn to clear list view contents
void clear_list_view() {

    gtk_list_store_clear(list_store);
}

// Fn to add a message to the list view
void add_message_to_list(const char *message) {
    GtkTreeIter iter;

    gtk_list_store_prepend(list_store, &iter);

    gtk_list_store_set(list_store, &iter, 0, message, -1);
}


/* Event handler */

void on_withdraw_clicked(GtkButton *button, gpointer user_data) {

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter Withdrawal Amount",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new(); 

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(withdraw_amount_from_bank), entry);

    gtk_widget_show_all(dialog);

}


void on_deposit_clicked(GtkButton *button, gpointer user_data) {

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter Deposit Amount",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new();  

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(deposit_amount_in_bank), entry);

    gtk_widget_show_all(dialog);

}

void on_transfer_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Transfer funds - <user_id>:<amount_to_transfer>",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new();  

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(transfer_funds), entry);

    gtk_widget_show_all(dialog);
}

void on_loan_req_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter Loan Request Amount",
        GTK_WINDOW(user_data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new();  

    gtk_container_add(GTK_CONTAINER(content_area), entry);

    g_signal_connect(dialog, "response", G_CALLBACK(request_for_loan), entry);

    gtk_widget_show_all(dialog);
}

void on_transactions_clicked(GtkButton *button, gpointer user_data) {
    clear_list_view();

    DataBlock block = {GET_TXN_HISTORY,getUserId(),0,0,NULL};

    DataBlock *logs = queryBankingServer(&block);

    TxnLogs *txnLogs = (TxnLogs*)logs->payload;

    char msg[256] = "";

    for(int i = 0; txnLogs[i].userId > 0; i++){
        if((int)txnLogs[i].txnAmount == 0)
            continue;
        snprintf(msg, 256, "%s:\t%0.2f", get_txn_type_name(txnLogs[i].transaction), txnLogs[i].txnAmount);
        add_message_to_list(msg);
    }

    free(logs);

}

void on_loans_clicked(GtkButton *button, gpointer user_data) {
    clear_list_view();

    DataBlock block = {GET_LOAN_DATA,getUserId(),0,0,NULL};

    DataBlock *logs = queryBankingServer(&block);

    LoanData *loanData= (LoanData*)logs->payload;

    char msg[256] = "";

    for(int i = 0; loanData[i].userId > 0; i++){
        if((int)loanData[i].loanRequest == 0)
            continue;

        snprintf(msg, 256, "Loan %d\t Amt:\t%0.2f\t Status: %s",
            loanData[i].loanId,loanData[i].loanRequest,loanData[i].loanApproved?"APPROVED":"PENDING");
        add_message_to_list(msg);
    }

    free(logs);
}

void on_refresh_clicked(GtkButton *button, gpointer user_data) {
    clear_list_view();

    DataBlock block = {GET_BALANCE,getUserId(),0,0,NULL};

    DataBlock *logs = queryBankingServer(&block);
    
    char msg[256] = "";

    snprintf(msg, 256, "Account Balance:\t%0.2f", logs->amount);
    add_message_to_list(msg);

    free(logs);

}

#endif