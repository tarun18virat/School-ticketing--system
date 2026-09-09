#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "tickets.dat"

typedef struct
{
    int id;
    char name[50];
    char className[20];
    char problem[100];
    char status[20];
} Ticket;

void addTicket();
void viewTickets();
void searchTicket();
void updateTicket();
void deleteTicket();
void menu();

int main()
{
    menu();
    return 0;
}

void menu()
{
    int choice;

    while (1)
    {
        printf("\n==============================\n");
        printf("   SCHOOL TICKETING SYSTEM\n");
        printf("==============================\n");
        printf("1. Add Ticket\n");
        printf("2. View Tickets\n");
        printf("3. Search Ticket\n");
        printf("4. Update Ticket\n");
        printf("5. Delete Ticket\n");
        printf("6. Exit\n");
        printf("==============================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                addTicket();
                break;

            case 2:
                viewTickets();
                break;

            case 3:
                searchTicket();
                break;

            case 4:
                updateTicket();
                break;

            case 5:
                deleteTicket();
                break;

            case 6:
                printf("\nProgram closed.\n");
                exit(0);

            default:
                printf("\nInvalid choice.\n");
        }
    }
}

void addTicket()
{
    FILE *fp;
    Ticket t;
    Ticket temp;
    int lastId = 0;

    fp = fopen(FILE_NAME, "rb");

    if (fp != NULL)
    {
        while (fread(&temp, sizeof(Ticket), 1, fp))
        {
            lastId = temp.id;
        }

        fclose(fp);
    }

    t.id = lastId + 1;

    printf("\n========== ADD TICKET ==========\n");

    printf("Student Name: ");
    fgets(t.name, sizeof(t.name), stdin);
    t.name[strcspn(t.name, "\n")] = '\0';

    printf("Class: ");
    fgets(t.className, sizeof(t.className), stdin);
    t.className[strcspn(t.className, "\n")] = '\0';

    printf("Problem: ");
    fgets(t.problem, sizeof(t.problem), stdin);
    t.problem[strcspn(t.problem, "\n")] = '\0';

    strcpy(t.status, "Open");

    fp = fopen(FILE_NAME, "ab");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    fwrite(&t, sizeof(Ticket), 1, fp);

    fclose(fp);

    printf("\nTicket created successfully.\n");
    printf("Your Ticket ID is: %d\n", t.id);
}

void viewTickets()
{
    FILE *fp;
    Ticket t;
    int count = 0;

    fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("\nNo tickets available.\n");
        return;
    }

    printf("\n================ ALL TICKETS ================\n");

    while (fread(&t, sizeof(Ticket), 1, fp))
    {
        printf("\nTicket ID : %d", t.id);
        printf("\nStudent   : %s", t.name);
        printf("\nClass     : %s", t.className);
        printf("\nProblem   : %s", t.problem);
        printf("\nStatus    : %s", t.status);
        printf("\n---------------------------------------------");

        count++;
    }

    fclose(fp);

    printf("\n\nTotal Tickets: %d\n", count);
}

void searchTicket()
{
    FILE *fp;
    Ticket t;
    int id;
    int found = 0;

    printf("\nEnter Ticket ID: ");
    scanf("%d", &id);
    getchar();

    fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("\nNo tickets available.\n");
        return;
    }

    while (fread(&t, sizeof(Ticket), 1, fp))
    {
        if (t.id == id)
        {
            printf("\n========== TICKET FOUND ==========\n");
            printf("Ticket ID : %d\n", t.id);
            printf("Student   : %s\n", t.name);
            printf("Class     : %s\n", t.className);
            printf("Problem   : %s\n", t.problem);
            printf("Status    : %s\n", t.status);

            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("\nTicket not found.\n");
    }
}

void updateTicket()
{
    FILE *fp;
    Ticket t;
    int id;
    int found = 0;

    printf("\nEnter Ticket ID to update: ");
    scanf("%d", &id);
    getchar();

    fp = fopen(FILE_NAME, "r+b");

    if (fp == NULL)
    {
        printf("\nNo tickets available.\n");
        return;
    }

    while (fread(&t, sizeof(Ticket), 1, fp))
    {
        if (t.id == id)
        {
            printf("\nTicket found.\n");
            printf("Student : %s\n", t.name);
            printf("Problem : %s\n", t.problem);
            printf("Status  : %s\n", t.status);

            printf("\nEnter new status: ");
            fgets(t.status, sizeof(t.status), stdin);
            t.status[strcspn(t.status, "\n")] = '\0';

            fseek(fp, -sizeof(Ticket), SEEK_CUR);

            fwrite(&t, sizeof(Ticket), 1, fp);

            found = 1;

            printf("\nTicket updated successfully.\n");
            break;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("\nTicket not found.\n");
    }
}

void deleteTicket()
{
    FILE *fp;
    FILE *temp;
    Ticket t;
    int id;
    int found = 0;

    printf("\nEnter Ticket ID to delete: ");
    scanf("%d", &id);
    getchar();

    fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("\nNo tickets available.\n");
        return;
    }

    temp = fopen("temp.dat", "wb");

    if (temp == NULL)
    {
        fclose(fp);
        printf("\nUnable to create temporary file.\n");
        return;
    }

    while (fread(&t, sizeof(Ticket), 1, fp))
    {
        if (t.id == id)
        {
            found = 1;
        }
        else
        {
            fwrite(&t, sizeof(Ticket), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(FILE_NAME);
        rename("temp.dat", FILE_NAME);

        printf("\nTicket deleted successfully.\n");
    }
    else
    {
        remove("temp.dat");
        printf("\nTicket not found.\n");
    }
}
