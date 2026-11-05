#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int prices[MAX];
int spans[MAX];
int stack[MAX];

int count = 0;
int spanCalculated = 0;

void enterPrices();
void calculateSpan();
void displayHistory();
void visualizeStack();
void maximumPrice();
void maximumSpan();
void addPrice();
void resetData();

void calculateSpan()
{
    int top = -1;

    for (int i = 0; i < count; i++)
    {
        while (top != -1 && prices[stack[top]] <= prices[i])
        {
            top--;
        }

        if (top == -1)
            spans[i] = i + 1;
        else
            spans[i] = i - stack[top];

        stack[++top] = i;
    }

    spanCalculated = 1;
}

void enterPrices()
{
    int n;

    printf("\nEnter number of days (1-%d): ", MAX);
    scanf("%d", &n);

    if (n < 1 || n > MAX)
    {
        printf("\nInvalid number of days!\n");
        return;
    }

    count = 0;
    spanCalculated = 0;

    printf("\nEnter stock prices:\n");

    for (int i = 0; i < n; i++)
    {
        printf("Day %d: ", i + 1);
        scanf("%d", &prices[i]);

        if (prices[i] <= 0)
        {
            printf("Invalid price! Enter a positive price.\n");
            i--;
            continue;
        }

        count++;
    }

    printf("\nStock prices entered successfully!\n");
}

void displayHistory()
{
    if (count == 0)
    {
        printf("\nNo stock data available!\n");
        return;
    }

    if (!spanCalculated)
        calculateSpan();

    printf("\n========== STOCK HISTORY ==========\n");
    printf("%-10s %-15s %-15s\n", "Day", "Price", "Span");

    for (int i = 0; i < count; i++)
    {
        printf("%-10d %-15d %-15d\n",
               i + 1, prices[i], spans[i]);
    }

    printf("===================================\n");
}

void visualizeStack()
{
    if (count == 0)
    {
        printf("\nNo stock data available!\n");
        return;
    }

    int tempStack[MAX];
    int top = -1;

    printf("\n======= STACK VISUALIZATION =======\n");

    for (int i = 0; i < count; i++)
    {
        printf("\nDay %d | Price = %d\n", i + 1, prices[i]);

        while (top != -1 && prices[tempStack[top]] <= prices[i])
        {
            printf("POP: Day %d (Price = %d)\n",
                   tempStack[top] + 1,
                   prices[tempStack[top]]);
            top--;
        }

        if (top == -1)
            spans[i] = i + 1;
        else
            spans[i] = i - tempStack[top];

        printf("Calculated Span = %d\n", spans[i]);

        tempStack[++top] = i;

        printf("Current Stack (Bottom to Top): ");

        for (int j = 0; j <= top; j++)
        {
            printf("%d ", prices[tempStack[j]]);
        }

        printf("\n");
    }

    spanCalculated = 1;

    printf("\n===================================\n");
}

void maximumPrice()
{
    if (count == 0)
    {
        printf("\nNo stock data available!\n");
        return;
    }

    int max = prices[0];
    int day = 1;

    for (int i = 1; i < count; i++)
    {
        if (prices[i] > max)
        {
            max = prices[i];
            day = i + 1;
        }
    }

    printf("\nMaximum Stock Price = %d\n", max);
    printf("Recorded on Day = %d\n", day);
}

void maximumSpan()
{
    if (count == 0)
    {
        printf("\nNo stock data available!\n");
        return;
    }

    if (!spanCalculated)
        calculateSpan();

    int max = spans[0];
    int day = 1;

    for (int i = 1; i < count; i++)
    {
        if (spans[i] > max)
        {
            max = spans[i];
            day = i + 1;
        }
    }

    printf("\nMaximum Stock Span = %d\n", max);
    printf("Recorded on Day = %d\n", day);
}

void addPrice()
{
    if (count >= MAX)
    {
        printf("\nStock history is full!\n");
        return;
    }

    int price;

    printf("\nEnter new stock price: ");
    scanf("%d", &price);

    if (price <= 0)
    {
        printf("\nInvalid stock price!\n");
        return;
    }

    prices[count] = price;
    count++;
    spanCalculated = 0;

    calculateSpan();

    printf("\nNew stock price added successfully!\n");
    printf("New Day = %d\n", count);
    printf("Stock Price = %d\n", price);
    printf("Calculated Span = %d\n", spans[count - 1]);
}

void resetData()
{
    count = 0;
    spanCalculated = 0;

    printf("\nAll stock data has been reset!\n");
}

int main()
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("========================================\n");
        printf("      STOCK SPAN PROBLEM VISUALIZER\n");
        printf("========================================\n");
        printf("1. Enter Stock Prices\n");
        printf("2. Calculate Stock Span\n");
        printf("3. Display Stock Price History\n");
        printf("4. Visualize Stack Operations\n");
        printf("5. Show Maximum Stock Price\n");
        printf("6. Show Maximum Stock Span\n");
        printf("7. Add New Stock Price\n");
        printf("8. Reset Data\n");
        printf("9. Exit\n");
        printf("========================================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                enterPrices();
                break;

            case 2:
                if (count == 0)
                    printf("\nPlease enter stock prices first!\n");
                else
                {
                    calculateSpan();
                    printf("\nStock spans calculated successfully!\n");
                }
                break;

            case 3:
                displayHistory();
                break;

            case 4:
                visualizeStack();
                break;

            case 5:
                maximumPrice();
                break;

            case 6:
                maximumSpan();
                break;

            case 7:
                addPrice();
                break;

            case 8:
                resetData();
                break;

            case 9:
                printf("\nThank you for using Stock Span Visualizer!\n");
                exit(0);

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}
