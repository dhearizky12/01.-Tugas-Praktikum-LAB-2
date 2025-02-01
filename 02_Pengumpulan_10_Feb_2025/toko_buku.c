#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAXC 100

typedef struct
{
    char bookCode[MAXC]; // AUTO GENERATED SINCE IT IS A UNIQUE
    char bookName[MAXC];
    char bookType[MAXC];
    unsigned int price; // price should not be minus.
} Book;

void createTXTIfNotExists(const char *filename)
{
    // Youtube, https://www.youtube.com/watch?v=QS7AiCN3KjQ, 19/01
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        /*
            -> fopen is part of the C standard library, and it is used to open a file.
            -> fopen is a way for allocated memory for file.
            why we need to allocate memory for file?
            -> because we need to store the file that we open. And store the file in the memory.
            -> if the file didnt open, it will return NULL.
            why we dont user malloc and free for file?
            -> because fopen is a standard library, and it will automatically allocate and deallocate the memory.
            -> and its more efficient to use fopen and fclose instead of malloc and free.
            why its more efficient?
            -> because fopen and fclose is a standard library, and it will automatically allocate and deallocate the memory.
            chatgpt, 19/01
        */
        file = fopen(filename, "w");
        if (file == NULL)
        {
            fprintf(stderr, "Error creating file.\n");
            exit(1);
        }
        fprintf(file, "bookCode,bookName,bookType,bookPrice\n"); // menulis ke file
        fclose(file);                                            // untuk menutup file( membersihkan memory)
    }
    else
    {
        fclose(file);
    }
}

void createBookCode(Book *book)
{
    // FEEL FREE TO CHANGE THE AGORITHM TO CREATE A UNIQUE BOOK NUM
    time_t currentTime = time(NULL);
    struct tm tm = *localtime(&currentTime);
    int todayYear = tm.tm_year + 1900;
    int currentMonth = tm.tm_mon + 1;
    int currentDay = tm.tm_mday;
    int currentHour = tm.tm_hour;
    int currentMinute = tm.tm_min;
    int currentSec = tm.tm_sec;
    sprintf(book->bookCode, "%04d%02d%02d_%02d%02d%02d", todayYear, currentMonth, currentDay, currentHour, currentMinute, currentSec);
}

int insertBook(const char *filename, Book *book)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    printf("Insert book name: \n");
    // GFG, https://www.geeksforgeeks.org/taking-string-input-space-c-3-different-methods/, 19/01
    // TAKING SPACE BETWEEN NAME AS INPUT using fgets
    if (fgets(book->bookName, sizeof(book->bookName), stdin) == NULL)
    {
        fprintf(stderr, "Error reading book name.\n");
        return 1;
    }

    printf("Insert book type: \n");
    if (fgets(book->bookType, sizeof(book->bookType), stdin) == NULL)
    {
        fprintf(stderr, "Error reading book name.\n");
        return 1;
    }

    // Remove trailing newline from book name
    size_t len = strlen(book->bookName);
    if (len > 0 && book->bookName[len - 1] == '\n')
    {
        book->bookName[len - 1] = '\0';
    }

    // Remove trailing newline from book type
    size_t typeLen = strlen(book->bookType);
    if (typeLen > 0 && book->bookType[typeLen - 1] == '\n')
    {
        book->bookType[typeLen - 1] = '\0';
    }

    char priceInput[MAXC];

    while (1)
    {
        int price = 0;
        fputs("Insert book price: \n", stdout);
        fflush(stdin);

        if (fgets(priceInput, MAXC, stdin) == NULL)
        {
            puts("(User canceled input)");
            return -1; // Handle manual EOF or input error
        }

        // Remove trailing newline
        size_t len = strlen(priceInput);
        if (len > 0 && priceInput[len - 1] == '\n')
        {
            priceInput[len - 1] = '\0';
        }

        // Convert input to an integer and validate
        char *end;
        price = strtol(priceInput, &end, 10);

        if (*end != '\0' || priceInput == end)
        {
            fputs("\nPlease enter a valid price.\n", stderr);
            // stderr argument is used to display the error message on the screen because it can procesing dynamic value
        }
        else if (price <= 0)
        {
            printf("Price should above 0\n");
        }
        else
        {
            book->price = price;
            break;
        }
    }

    createBookCode(book);

    fprintf(file, "%s,%s,%s,%d\n", book->bookCode, book->bookName, book->bookType, book->price);
    fclose(file);

    return 0;
}

int main(int argc, char const *argv[])
{
    const char *fileName = "./databuku.txt";
    createTXTIfNotExists(fileName);
    int choice;
    Book book;

    printf("=========================================================\n");
    printf("Welcome to the BINUS Group 4 Book store\n");
    printf("=========================================================\n");

    while (1)
    {
        printf("1. Insert Book\n");
        printf("2. View History\n");
        printf("3. View Book\n");
        printf("4. Delete History\n");
        printf("5. Delete Book\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        if (scanf("%d", &choice) != 1)
        {
            fprintf(stderr, "Invalid input.\n");
            return 1;
        }

        // Clear input buffer
        // is a common way to clear the input buffer to handle cases where leftover characters
        // remain in the input stream after a scanf, fgets, or similar input functions.
        // CHATGPT, 19/01
        while (getchar() != '\n' && getchar() != EOF)
            ;

        switch (choice)
        {
        case 1:
            if (insertBook(fileName, &book) == 0)
            {
                printf("Book inserted successfully.\n");
            }
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
