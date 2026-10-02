#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "accounts.dat"

struct BankAccount {
    int accountNumber;
    char name[50];
    float balance;
};


void createAccount();
void deposit();
void withdrawMoney();
void balanceEnquiry();
void displayAccount(struct BankAccount acc);

int accountExists(int accNo);

int main() {
    int choice;

    while (1) {
    
        printf(" BANK ACCOUNT MANAGEMENT\n");
    
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Balance Enquiry\n");
        printf("5. Exit\n");
    
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;

            case 2:
                deposit();
                break;

            case 3:
                withdrawMoney();
                break;

            case 4:
                balanceEnquiry();
                break;

            case 5:
                printf("\nThank you for using our banking system!\n");
                exit(0);

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}


int accountExists(int accNo) {
    FILE *fp;
    struct BankAccount acc;

    fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        return 0;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNumber == accNo) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


void createAccount() {
    FILE *fp;
    struct BankAccount acc;

    printf("\n----- CREATE ACCOUNT -----\n");

    printf("Enter account number: ");
    scanf("%d", &acc.accountNumber);

    if (accountExists(acc.accountNumber)) {
        printf("Account number already exists!\n");
        return;
    }

    printf("Enter account holder name: ");
    scanf(" %[^\n]", acc.name);

    printf("Enter initial deposit: ");
    scanf("%f", &acc.balance);

    if (acc.balance < 0) {
        printf("Initial deposit cannot be negative.\n");
        return;
    }

    fp = fopen(FILE_NAME, "ab");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);

    printf("\nAccount created successfully!\n");
}


void deposit() {
    FILE *fp;
    struct BankAccount acc;
    int accNo;
    float amount;
    int found = 0;

    printf("\n----- DEPOSIT MONEY -----\n");

    printf("Enter account number: ");
    scanf("%d", &accNo);

    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Amount must be greater than 0.\n");
        return;
    }

    fp = fopen(FILE_NAME, "rb+");

    if (fp == NULL) {
        printf("No account records found.\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {

        if (acc.accountNumber == accNo) {
            acc.balance += amount;

            
            fseek(fp, -sizeof(acc), SEEK_CUR);

            fwrite(&acc, sizeof(acc), 1, fp);

            found = 1;

            printf("\nAmount deposited successfully!\n");
            printf("New balance: %.2f\n", acc.balance);
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Account not found!\n");
    }
}


void withdrawMoney() {
    FILE *fp;
    struct BankAccount acc;
    int accNo;
    float amount;
    int found = 0;

    printf("\n----- WITHDRAW MONEY -----\n");

    printf("Enter account number: ");
    scanf("%d", &accNo);

    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Amount must be greater than 0.\n");
        return;
    }

    fp = fopen(FILE_NAME, "rb+");

    if (fp == NULL) {
        printf("No account records found.\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {

        if (acc.accountNumber == accNo) {
            found = 1;

            if (amount > acc.balance) {
                printf("\nInsufficient balance!\n");
            } else {
                acc.balance -= amount;

                
                fseek(fp, -sizeof(acc), SEEK_CUR);

                fwrite(&acc, sizeof(acc), 1, fp);

                printf("\nWithdrawal successful!\n");
                printf("Remaining balance: %.2f\n", acc.balance);
            }

            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Account not found!\n");
    }
}


void balanceEnquiry() {
    FILE *fp;
    struct BankAccount acc;
    int accNo;
    int found = 0;

    printf("\n----- BALANCE ENQUIRY -----\n");

    printf("Enter account number: ");
    scanf("%d", &accNo);

    fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        printf("No account records found.\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {

        if (acc.accountNumber == accNo) {
            found = 1;
            displayAccount(acc);
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Account not found!\n");
    }
}


void displayAccount(struct BankAccount acc) {
    
    printf("Account Number : %d\n", acc.accountNumber);
    printf("Account Holder : %s\n", acc.name);
    printf("Balance        : %.2f\n", acc.balance);
    
}

