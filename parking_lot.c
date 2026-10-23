
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX 10
#define WAITING 10

struct Vehicle
{
    char number[20];
    char owner[50];
    time_t entryTime;
};

struct Vehicle parking[MAX];
struct Vehicle waitingQueue[WAITING];

int count = 0;
int front = 0;
int rear = -1;
int waitingCount = 0;

void clearInput()
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

int vehicleExists(char number[])
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (strcmp(parking[i].number, number) == 0)
        {
            return 1;
        }
    }

    for (i = 0; i < waitingCount; i++)
    {
        int index = (front + i) % WAITING;

        if (strcmp(waitingQueue[index].number, number) == 0)
        {
            return 1;
        }
    }

    return 0;
}

void saveData()
{
    FILE *fp = fopen("parking.txt", "w");

    if (fp == NULL)
    {
        printf("File cannot be opened!\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "P|%s|%s|%lld\n",
                parking[i].number,
                parking[i].owner,
                (long long)parking[i].entryTime);
    }

    for (int i = 0; i < waitingCount; i++)
    {
        int index = (front + i) % WAITING;

        fprintf(fp, "W|%s|%s|%lld\n",
                waitingQueue[index].number,
                waitingQueue[index].owner,
                (long long)waitingQueue[index].entryTime);
    }

    fclose(fp);
}

void loadData()
{
    FILE *fp = fopen("parking.txt", "r");

    if (fp == NULL)
    {
        return;
    }

    char line[150];

    count = 0;
    front = 0;
    rear = -1;
    waitingCount = 0;

    while (fgets(line, sizeof(line), fp))
    {
        char *type = strtok(line, "|");
        char *number = strtok(NULL, "|");
        char *owner = strtok(NULL, "|");
        char *entry = strtok(NULL, "|\n");

        if (type == NULL || number == NULL ||
            owner == NULL || entry == NULL)
        {
            continue;
        }

        struct Vehicle v;

        strcpy(v.number, number);
        strcpy(v.owner, owner);
        v.entryTime = (time_t)atoll(entry);

        if (type[0] == 'P' && count < MAX)
        {
            parking[count++] = v;
        }
        else if (type[0] == 'W' &&
                 waitingCount < WAITING)
        {
            rear = (rear + 1) % WAITING;
            waitingQueue[rear] = v;
            waitingCount++;
        }
    }

    fclose(fp);
}

void parkVehicle()
{
    struct Vehicle v;

    if (count == MAX && waitingCount == WAITING)
    {
        printf("Parking and waiting queue are full!\n");
        return;
    }

    printf("Enter Vehicle Number: ");

    if (scanf("%19s", v.number) != 1)
    {
        printf("Invalid vehicle number!\n");
        clearInput();
        return;
    }

    if (vehicleExists(v.number))
    {
        printf("Vehicle already exists!\n");
        return;
    }

    printf("Enter Owner Name: ");

    if (scanf(" %49[^\n]", v.owner) != 1)
    {
        printf("Invalid owner name!\n");
        clearInput();
        return;
    }

    v.entryTime = time(NULL);

    if (count < MAX)
    {
        parking[count++] = v;

        printf("Vehicle parked successfully!\n");
    }
    else if (waitingCount < WAITING)
    {
        rear = (rear + 1) % WAITING;

        waitingQueue[rear] = v;

        waitingCount++;

        printf("Parking full!\n");
        printf("Vehicle added to waiting queue.\n");
    }
}

void displayVehicles()
{
    if (count == 0)
    {
        printf("No vehicles parked.\n");
        return;
    }

    printf("\n--- Parked Vehicles ---\n");

    for (int i = 0; i < count; i++)
    {
        printf("\nVehicle Number: %s\n",
               parking[i].number);

        printf("Owner Name: %s\n",
               parking[i].owner);

        printf("Entry Time: %s",
               ctime(&parking[i].entryTime));
    }
}

void availableSpaces()
{
    printf("\nTotal Parking Spaces: %d\n", MAX);

    printf("Occupied Spaces: %d\n", count);

    printf("Available Spaces: %d\n", MAX - count);

    printf("Waiting Vehicles: %d\n", waitingCount);
}

void searchVehicle()
{
    char number[20];

    printf("Enter Vehicle Number to Search: ");

    if (scanf("%19s", number) != 1)
    {
        printf("Invalid vehicle number!\n");
        clearInput();
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (strcmp(parking[i].number, number) == 0)
        {
            printf("\nVehicle found in parking!\n");

            printf("Vehicle Number: %s\n",
                   parking[i].number);

            printf("Owner Name: %s\n",
                   parking[i].owner);

            return;
        }
    }

    for (int i = 0; i < waitingCount; i++)
    {
        int index = (front + i) % WAITING;

        if (strcmp(waitingQueue[index].number, number) == 0)
        {
            printf("\nVehicle found in waiting queue!\n");

            printf("Vehicle Number: %s\n",
                   waitingQueue[index].number);

            printf("Owner Name: %s\n",
                   waitingQueue[index].owner);

            return;
        }
    }

    printf("Vehicle not found!\n");
}

void displayWaitingQueue()
{
    if (waitingCount == 0)
    {
        printf("Waiting queue is empty.\n");
        return;
    }

    printf("\n--- Waiting Queue ---\n");

    for (int i = 0; i < waitingCount; i++)
    {
        int index = (front + i) % WAITING;

        printf("\nVehicle Number: %s\n",
               waitingQueue[index].number);

        printf("Owner Name: %s\n",
               waitingQueue[index].owner);
    }
}

void exitVehicle()
{
    char number[20];

    int found = -1;

    printf("Enter Vehicle Number to Exit: ");

    if (scanf("%19s", number) != 1)
    {
        printf("Invalid vehicle number!\n");
        clearInput();
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (strcmp(parking[i].number, number) == 0)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("Vehicle not found in parking!\n");
        return;
    }

    time_t exitTime = time(NULL);

    double seconds = difftime(
        exitTime,
        parking[found].entryTime
    );

    int hours = (int)(seconds / 3600);

    if (seconds > hours * 3600)
    {
        hours++;
    }

    if (hours < 1)
    {
        hours = 1;
    }

    int fee = hours * 20;

    printf("\n--- Parking Ticket ---\n");

    printf("Vehicle Number: %s\n",
           parking[found].number);

    printf("Owner Name: %s\n",
           parking[found].owner);

    printf("Parking Hours: %d\n", hours);

    printf("Total Fee: Rs. %d\n", fee);

    for (int i = found; i < count - 1; i++)
    {
        parking[i] = parking[i + 1];
    }

    count--;

    printf("Vehicle exited successfully!\n");

    if (waitingCount > 0)
    {
        parking[count] = waitingQueue[front];

        parking[count].entryTime = time(NULL);

        printf("\nWaiting vehicle moved to parking:\n");

        printf("Vehicle Number: %s\n",
               parking[count].number);

        count++;

        front = (front + 1) % WAITING;

        waitingCount--;
    }
}

int main()
{
    int choice;

    loadData();

    while (1)
    {
        printf("\n===== PARKING LOT MANAGEMENT SYSTEM =====\n");

        printf("1. Park Vehicle\n");
        printf("2. Display Parked Vehicles\n");
        printf("3. Available Parking Spaces\n");
        printf("4. Search Vehicle\n");
        printf("5. Exit Vehicle\n");
        printf("6. Display Waiting Queue\n");
        printf("0. Exit Program\n");

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number.\n");

            clearInput();

            continue;
        }

        switch (choice)
        {
            case 1:
                parkVehicle();
                saveData();
                break;

            case 2:
                displayVehicles();
                break;

            case 3:
                availableSpaces();
                break;

            case 4:
                searchVehicle();
                break;

            case 5:
                exitVehicle();
                saveData();
                break;

            case 6:
                displayWaitingQueue();
                break;

            case 0:
                saveData();

                printf("Data saved successfully!\n");

                printf("Program closed.\n");

                return 0;

            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
