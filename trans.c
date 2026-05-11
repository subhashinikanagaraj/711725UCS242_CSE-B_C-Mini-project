#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void searchRecord(FILE *fPtr);
void displayAll(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    // open/create file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        struct clientData blank = {0, "", "", 0.0};

        for (int i = 0; i < MAX; i++)
        {
            fwrite(&blank, sizeof(struct clientData), 1, cfPtr);
        }

        printf("New file created!\n");
    }

    printf("Welcome to the Bank Management System!\n");

    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1: textFile(cfPtr); break;
        case 2: updateRecord(cfPtr); break;
        case 3: newRecord(cfPtr); break;
        case 4: deleteRecord(cfPtr); break;
        case 5: searchRecord(cfPtr); break;
        case 6: displayAll(cfPtr); break;
        default: printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    printf("Thank you for using the Bank Management System!\n");
    return 0;
}

// MENU
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\nMenu:\n");
    printf("1. Store accounts in text file\n");
    printf("2. Update account\n");
    printf("3. Add new account\n");
    printf("4. Delete account\n");
    printf("5. Search account\n");
    printf("6. Display all accounts\n");
    printf("7. Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}

// TEXT FILE
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    writePtr = fopen("accounts.txt", "w");
    rewind(readPtr);

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "ID", "Last", "First", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                    client.acctNum, client.lastName,
                    client.firstName, client.balance);
        }
    }

    fclose(writePtr);
    printf("accounts.txt created\n");
}

// UPDATE
void updateRecord(FILE *fPtr)
{
    struct clientData client;
    unsigned int acc;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &acc);

    if (acc < 1 || acc > MAX)
    {
        printf("Invalid account\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("No record found\n");
        return;
    }

    printf("Balance: %.2f\n", client.balance);
    printf("Enter amount (+/-): ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated Balance: %.2f\n", client.balance);
}

// DELETE
void deleteRecord(FILE *fPtr)
{
    struct clientData client, blank = {0, "", "", 0};
    unsigned int acc;

    printf("Enter account: ");
    scanf("%u", &acc);

    if (acc < 1 || acc > MAX)
    {
        printf("Invalid account\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
        printf("Not found\n");
    else
    {
        fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blank, sizeof(struct clientData), 1, fPtr);
        printf("Deleted\n");
    }
}

// NEW
void newRecord(FILE *fPtr)
{
    struct clientData client;
    unsigned int acc;

    printf("Enter account number: ");
    scanf("%u", &acc);

    if (acc < 1 || acc > MAX)
    {
        printf("Invalid account\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account exists\n");
        return;
    }

    printf("Enter last first balance: ");
    scanf("%14s %9s %lf",
          client.lastName,
          client.firstName,
          &client.balance);

    client.acctNum = acc;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account added\n");
}

// SEARCH
void searchRecord(FILE *fPtr)
{
    struct clientData client;
    unsigned int acc;

    printf("Enter account: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
        printf("Not found\n");
    else
        printf("%d %s %s %.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);
}

// DISPLAY ALL
void displayAll(FILE *fPtr)
{
    struct clientData client;
    rewind(fPtr);

    printf("\nAll Accounts:\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%d %s %s %.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}