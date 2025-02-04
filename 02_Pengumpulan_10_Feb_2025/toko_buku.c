#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAXC 100
#define TYPE 10
#define DATE 30

typedef struct
{
    char bookCode[MAXC]; // AUTO GENERATED SINCE IT IS A UNIQUE
    char bookName[MAXC];
    char bookType[MAXC];
    unsigned int price; // price should not be minus.
    char createdTime[DATE];
    char recordType[TYPE]; // Store data type: "buku" or "penjualan"    
    union{
        struct{
            char buyer[MAXC];
            char saleDate[DATE];
        }dataPenjualan;
    } additionalData; //berisi data misal data buku, data penjualan
    int isDeleted; //flag untuk menandai apakah entri terhapus
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
            -> if the file didn't open, it will return NULL.
            why we dont user malloc and free for file?
            -> because fopen is a standard library, and it will automatically allocate and deallocate the memory.
            -> and its more efficient to use fopen and fclose instead of malloc and free.
            chatgpt, 19/01
        */
        file = fopen(filename, "w");
        if (file == NULL)
        {
            fprintf(stderr, "Error creating file.\n");
            exit(1);
        }
        //menulis header untuk file.
        fprintf(file, "bookCode,bookName,bookType,bookPrice,createdTime,recordType,buyer,saleDate,isDeleted\n"); // menulis ke file
        fclose(file);  //untuk menutup file (membersihkan memori)                                         // untuk menutup file( membersihkan memory)
    }
    else
    {
        fclose(file);
    }
}

void createBookCode(Book *book)
{
    //Algorithm for creating unique book code
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

void createdTime(Book *book)
{
    //set createdTime to current time source : https://stackoverflow.com/questions/12784903/timenull-returning-different-time
    time_t currentTime = time(NULL); // holds a value representing the number of seconds since the UNIX epoch
    //Epoch was 1.1.1970, 00:00:00 in Greenwich, UK. So in fact time() does not return a time, but a time difference.
    struct tm *tm = localtime(&currentTime); //localtime untuk mendapatkan waktu lokal, bisakah kita ganti tm ke tipe bukan struct ?

    strftime(book->createdTime, sizeof(book->createdTime), "%Y-%m-%d %H:%M:%S", tm); //strftime untuk mengubah waktu ke dalam string
}

int insertBook(const char *filename, Book *book)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    printf("Insert book name: ");
    // GFG, https://www.geeksforgeeks.org/taking-string-input-space-c-3-different-methods/, 19/01
    // TAKING SPACE BETWEEN NAME AS INPUT using fgets
    if (fgets(book->bookName, sizeof(book->bookName), stdin) == NULL)
    {
        fprintf(stderr, "Error reading book name.\n");
        return 1;
    }

    printf("Insert book type: ");
    if (fgets(book->bookType, sizeof(book->bookType), stdin) == NULL)
    {
        fprintf(stderr, "Error reading book type.\n");
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
        fputs("Insert book price (Please use numeric value in IDR): ", stdout);
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

    //set record type to "buku"
    strcpy(book->recordType, "buku"); //strcpy untuk mengcopy string "buku" ke dalam recordType

    //set isDeleted to 0 (not deleted)
    book->isDeleted = 0;

    //autogenerate book code
    createBookCode(book);

    //auto set created time data
    createdTime(book);

    fprintf(file, "%s,%s,%s,%d,%s,%s,,,%d\n", book->bookCode, book->bookName, book->bookType, book->price, book->createdTime, book->recordType, book->isDeleted);
    fclose(file);

    return 0;
}

void setConsoleFontColor(int colour)
{
    // escape ANSI https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
    //\033 adalah escape sequence yang mewakili karakter ESC (Escape) dalam ASCII.
    //38 kode untuk warna teks
    // ;5  => kode untuk warna teks 256 palet warna
    // m : perintah yang mengatur atribut grafis (SGR - Select Graphic Rendition).
    printf("\033[38;5;%dm", colour);
}

void resetConsoleFontColor()
{
    printf("\033[0m");
}

int main()
{
    const char *fileName = "../databuku.txt";
    createTXTIfNotExists(fileName);
    int choice;
    Book book;

    setConsoleFontColor(106); 
    printf("\n=========================================================\n");
    printf("******** Welcome to the BINUS Group 4 Book store ********\n");
    printf("=========================================================\n");
    resetConsoleFontColor();

    while (1)
    {
        printf("\n=========================================================\n");
        printf("           ******** Please Select MENU ********      \n");
        printf("=========================================================\n");
        printf("1. Insert Book Data. Pada menu ini, user dapat menginput data buku\n");
        printf("2. View History Penjualan. Pada menu ini akan ditampilkan data history penjualan\n");
        printf("3. View Book Data. Pada menu ini akan ditampilkan seluruh data buku\n");
        printf("4. Delete History Penjualan. Pada menu ini akan di tampilkan history penjualan dan user bisa memilih untuk mendelete\n");
        printf("5. Delete Book. Pada menu ini sistem akan menampilkan data buku untuk di delete\n");
        printf("6. Insert Data Penjualan. Pada menu ini user dapat menginput data penjualan\n");
        printf("7. Exit\n");
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
        while (getchar() != '\n' && getchar() != EOF);

        switch (choice)
        {
        case 1:
            if (insertBook(fileName, &book) == 0)
            {
                setConsoleFontColor(106);
                printf(" ------ Book inserted successfully. ------ \n");
                resetConsoleFontColor();
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
            break;
        case 7:
            exit(0);
        default:
            setConsoleFontColor(160);
            printf("Invalid choice. Please try again.\n");
            resetConsoleFontColor();
        }
    }

    return 0;
}
