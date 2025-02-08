#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAXC 50
#define TYPE 10
#define CREATED_TIME 20
#define BUFFER 256

#define MAX_BOOK_CODE 20
#define MAX_BOOK_NAME 32
#define MAX_BOOK_TYPE 10
#define MAX_BOOK_PRICE 10
#define MAX_RECORD_TYPE 10
#define MAX_BUYER 20
#define MAX_SALE_DATE 20
#define TEMPORARY_FILE_NAME "./tempdatabuku.txt"

typedef struct
{
    char bookCode[MAX_BOOK_CODE]; // AUTO GENERATED SINCE IT IS A UNIQUE
    char bookName[MAX_BOOK_NAME];
    char bookType[MAX_BOOK_TYPE];
    unsigned int price; // price should not be minus.
    char createdTime[CREATED_TIME];
    char recordType[TYPE]; // Store data type: "buku" or "penjualan"    
    union{
        struct{
            char buyer[MAX_BUYER];
            char saleDate[CREATED_TIME];
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

void createdTime(Book *book, const char *desiredRecordType)
{
    //set createdTime to current time source : https://stackoverflow.com/questions/12784903/timenull-returning-different-time
    time_t currentTime = time(NULL); // holds a value representing the number of seconds since the UNIX epoch
    //Epoch was 1.1.1970, 00:00:00 in Greenwich, UK. So in fact time() does not return a time, but a time difference.
    struct tm *tm = localtime(&currentTime); //localtime untuk mendapatkan waktu lokal, bisakah kita ganti tm ke tipe bukan struct ?

    if ( strcmp(desiredRecordType, "buku") == 0 )
    {
        strftime(book->createdTime, sizeof(book->createdTime), "%Y-%m-%d %H:%M:%S", tm); //strftime untuk mengubah waktu ke dalam string
    }
    else if ( strcmp (desiredRecordType, "penjualan") == 0 )
    {
        strftime(book->additionalData.dataPenjualan.saleDate, sizeof(book->additionalData.dataPenjualan.saleDate), "%Y-%m-%d %H:%M:%S", tm); //strftime untuk mengubah waktu ke dalam string    
    }
}

// Remove trailing newline from fgets
void removeTrailingNewLine(char *str)
{
    size_t len = strlen(str);
    if(len > 0 && str[len-1] == '\n')
    {
        str[len-1] = '\0';
    }
}

void clearInputBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int insertBook(const char *filename, Book *book)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    printf("Insert book name (max %d characters) : ", MAX_BOOK_NAME);
    fflush(stdin); // untuk membersihkan buffer input
    // GFG, https://www.geeksforgeeks.org/taking-string-input-space-c-3-different-methods/, 19/01
    // TAKING SPACE BETWEEN NAME AS INPUT using fgets
    if (fgets(book->bookName, sizeof(book->bookName), stdin) == NULL)
    {
        fprintf(stderr, "Error reading book name.\n");
        return 1;
    }

    removeTrailingNewLine(book->bookName);

    printf("Insert book type (max %d characters): ", MAX_BOOK_TYPE );
    fflush(stdin);
    if (fgets(book->bookType, sizeof(book->bookType), stdin) == NULL)
    {
        fprintf(stderr, "Error reading book type.\n");
        return 1;
    }

    // Remove trailing newline from book type
    removeTrailingNewLine(book->bookType);

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
    createdTime(book, "buku");

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

void displayData(const char *filename, const char *desiredRecordType)
{
    FILE *file = fopen(filename, "r");
    
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    char line[BUFFER];
    int isEmpty = 1;

    // Read and display the header
    if (fgets(line, sizeof(line), file) != NULL)
    {
        if ( strcmp(desiredRecordType, "penjualan") == 0 )
        {
            printf("\n------- -------------------- ----------------------------------- ------------- ----------- -------------------- ---------- -------------------- --------------------\n");
            printf("%-7s %-20s %-35s %-13s %-11s %-20s %-11s %-20s %-20s\n","index","bookCode","bookName","bookType","bookPrice","createdTime","recordType","buyer","saleDate\n");
            printf("------- -------------------- ----------------------------------- ------------- ----------- -------------------- ---------- -------------------- --------------------\n");
        }
        else if( strcmp(desiredRecordType, "buku") == 0 )
        {
            printf("\n------- -------------------- ----------------------------------- ------------- ----------- -------------------- ----------\n");
            printf("%-7s %-20s %-35s %-13s %-11s %-20s %-11s\n","index","bookCode","bookName","bookType","bookPrice","createdTime","recordType\n");
            printf("------- -------------------- ----------------------------------- ------------- ----------- -------------------- ----------\n");
        }
    }

    //membaca dan memproses setiap baris data
    int index = 1;
    while (fgets(line, sizeof(line), file)) 
    {
        char *bookCode = strtok(line, ","); //strtok untuk memisahkan string berdasarkan delimiter. parameter line bertujuan untuk memisahkan string line berdasarkan delimiter ","
        char *bookName = strtok(NULL, ","); // parameter NULL pada strtok bertujuan untuk melanjutkan pemisahan string dari posisi terakhir pemisahan
        char *bookType = strtok(NULL, ","); // strtok(NULL, ",") berarti melanjutkan pemisahan string dari posisi terakhir pemisahan
        char *bookPrice = strtok(NULL, ",");
        char *createdTime = strtok(NULL, ",");
        char *recordType = strtok(NULL, ",");
        char *buyer = strtok(NULL, ",");
        char *saleDate = strtok(NULL, ",");
        char *isDeleted = strtok(NULL, ",");

        int isDeletedInt = atoi(isDeleted); //atoi untuk mengubah string menjadi integer

        //memeriksa kondisi isDeleted == 0 dan recordType == desiredRecordType
        //strcmp untuk membandingkan dua string
        if ( isDeletedInt == 0 && strcmp(recordType, desiredRecordType) == 0 && strcmp("penjualan", desiredRecordType) == 0 )
        {
            printf("%-7d %-20s %-35s %-13s %-11s %-20s %-11s %-20s %-20s\n", index, bookCode, bookName, bookType, bookPrice, createdTime, recordType, buyer, saleDate);
            isEmpty = 0;
            index++;
        }
        else if ( isDeleted == 0 && strcmp(recordType, desiredRecordType) == 0 && strcmp("buku", desiredRecordType) == 0)
        {
            printf("%-7d %-20s %-35s %-13s %-11s %-20s %-11s\n",index, bookCode, bookName, bookType, bookPrice, createdTime, recordType );
            isEmpty = 0;
        }
    }

    if (isEmpty)
    {
        setConsoleFontColor(160);
        printf(" ------------------------------------------ No data available. ----------------------------------------------\n");
        resetConsoleFontColor();
    }

    fclose(file);
}

int insertSaleBook(const char *filename, Book *book)
{
    FILE *file;
    // Menampilkan daftar buku
    displayData(filename, "buku");
    while (1)
    {
        file = fopen(filename, "r");
        if (file == NULL)
        {
            fprintf(stderr, "Error opening file.\n");
            return 1;
        }

        // Meminta kode buku dari user
        char selectedBookCode[MAX_BOOK_CODE];
        printf("Masukkan kode buku yang akan dijual: ");
        fgets(selectedBookCode, sizeof(selectedBookCode), stdin);
        removeTrailingNewLine(selectedBookCode);

        int found = 0;
        Book tempBook;

        char line[BUFFER];
        fgets(line, sizeof(line), file); // Copy header to temp

        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%[^,],%[^,],%[^,],%u,%[^,],%[^,],,,%d", 
                    tempBook.bookCode, tempBook.bookName, tempBook.bookType, 
                    &tempBook.price, tempBook.createdTime, tempBook.recordType, &tempBook.isDeleted);

            if (strcmp(tempBook.bookCode, selectedBookCode) == 0 && strcmp(tempBook.recordType, "buku") == 0 && tempBook.isDeleted == 0)
            {
                found = 1;
                *book = tempBook;
            }
        }
        fclose(file);

        if (!found)
        {
            setConsoleFontColor(160);
            fprintf(stderr, "Kode buku tidak ditemukan. Silakan coba lagi.\n");
            resetConsoleFontColor();
        }
        else
        {
            break;
        }
    }

    // Meminta nama pembeli
    printf("Masukkan nama pembeli: ");
    fgets(book->additionalData.dataPenjualan.buyer, sizeof(book->additionalData.dataPenjualan.buyer), stdin);
    removeTrailingNewLine(book->additionalData.dataPenjualan.buyer);

    strcpy(book->recordType, "penjualan");
    createdTime(book, "penjualan");
    book->isDeleted = 0;

    // Menulis data ke file
    file = fopen(filename, "a");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }
    fprintf(file, "%s,%s,%s,%u,%s,%s,%s,%s,%d\n", 
            book->bookCode, book->bookName, book->bookType, book->price, book->createdTime, book->recordType, book->additionalData.dataPenjualan.buyer, book->additionalData.dataPenjualan.saleDate, book->isDeleted);
    fclose(file);

    return 0;
}

int deleteDataPenjualan(const char *filename, int deletedIndex) {
    FILE *file;
    FILE *tempFile;
    while (1)
    {
        file = fopen(filename, "r");
        if (file == NULL)
        {
            fprintf(stderr, "Error opening file.\n");
            return 1;
        }

        int found = 0;
        Book tempBook;

        char line[BUFFER];
        fgets(line, sizeof(line), file); // Copy header to temp

        int indexPenjualan = 0;
        char buyer[BUFFER];
        char saleDate[BUFFER];

        tempFile = fopen(TEMPORARY_FILE_NAME, "w"); // mempersiapkan file temporary untuk menyimpan data terupdate setelah di delete
        if (tempFile == NULL)
        {
            fprintf(stderr, "Error opening tempFile.\n");
            return 1;
        }
        fprintf(tempFile, "bookCode,bookName,bookType,bookPrice,createdTime,recordType,buyer,saleDate,isDeleted\n"); // mempersiapkan header file di temporary file
        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%[^,],%[^,],%[^,],%u,%[^,],%[^,],%[^,],%[^,],%d", 
                    tempBook.bookCode, tempBook.bookName, tempBook.bookType, 
                    &tempBook.price, tempBook.createdTime, tempBook.recordType, buyer, saleDate, &tempBook.isDeleted);
            if (strcmp(tempBook.recordType, "penjualan") == 0 && tempBook.isDeleted == 0) { // memastikan hanya melakukan looping terhadap record penjualan dan belum pernah di delete
                indexPenjualan++;
                if (indexPenjualan == deletedIndex) { // melakukan pengecekan apakah index yg ingin di delete sudah sesuai
                    found = 1;
                    fprintf(tempFile, "%s,%s,%s,%u,%s,%s,%s,%s,%d\n", tempBook.bookCode, tempBook.bookName, tempBook.bookType, tempBook.price, tempBook.createdTime, tempBook.recordType, buyer, saleDate, 1); // menyimpan baris yg akan didelete dengan isDeleted 1
                    continue;
                }
            }

            fprintf(tempFile, "%s,%s,%s,%u,%s,%s,%s,%s,%d\n", 
            tempBook.bookCode, tempBook.bookName, tempBook.bookType, tempBook.price, tempBook.createdTime, tempBook.recordType, buyer, saleDate, tempBook.isDeleted); // menyimpan data tanpa perubahan
        }
        fclose(file);
        fclose(tempFile);
        /*
            Setelah membuat file temporary berisi data yg sudah diupdate dengan isDeleted menjadi 1, kita akan melakukan overwrite kepada file utama sehingga data akan selalu terupdate.
        */
        remove(filename); // Delete file data buku
        rename(TEMPORARY_FILE_NAME, filename); // Ubah nama file temporary menjadi databuku

        if (!found)
        {
            setConsoleFontColor(160);
            fprintf(stderr, "Indeks tidak ditemukan. Silakan coba lagi.\n");
            resetConsoleFontColor();
            return 1;
        }
        else
        {
            break;
        }
    }
    return 0;
}

void deleteDataPenjualanMenu(const char *filename) {
    displayData(filename, "penjualan");

    printf("\n=========================================================\n");
    printf("Silahkan input indeks yg ingin di delete: ");
    int deletedIndex;
    scanf("%d", &deletedIndex);
    int resultDelete = deleteDataPenjualan(filename, deletedIndex);
    if (resultDelete == 0) {
        setConsoleFontColor(177);
        printf("Data Penjualan indeks ke %d berhasil dihapus", deletedIndex);
        resetConsoleFontColor();
    }
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
        printf("1. Insert Book Data.");
        setConsoleFontColor(154);
        printf("Pada menu ini, user dapat menginput data buku\n");
        resetConsoleFontColor();
        
        printf("2. View History Penjualan.");
        setConsoleFontColor(154);
        printf("Pada menu ini akan ditampilkan data history penjualan\n");
        resetConsoleFontColor();

        printf("3. View Book Data.");
        setConsoleFontColor(154);
        printf("Pada menu ini akan ditampilkan seluruh data buku\n");
        resetConsoleFontColor();
        
        printf("4. Delete History Penjualan.");
        setConsoleFontColor(154);
        printf("Pada menu ini akan di tampilkan history penjualan dan user bisa memilih untuk mendelete\n");
        resetConsoleFontColor();

        printf("5. Delete Book.");
        setConsoleFontColor(154);
        printf("Pada menu ini sistem akan menampilkan data buku untuk di delete\n");
        resetConsoleFontColor();

        printf("6. Insert Data Penjualan.");
        setConsoleFontColor(154);
        printf("Pada menu ini user dapat menginput data penjualan\n");
        resetConsoleFontColor();

        printf("7. Exit\n");
        printf("Choose an option: ");
        if (scanf("%d", &choice) != 1)
        {
            fprintf(stderr, "Invalid input. Please input numeric value\n");
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
            displayData(fileName, "penjualan");
            break;
        case 3:
            displayData(fileName, "buku");
            break;
        case 4:
            deleteDataPenjualanMenu(fileName);
            break;
        case 5:
            break;
        case 6:
            if (insertSaleBook(fileName, &book) == 0)
            {
                setConsoleFontColor(106);
                printf(" ------ Data Penjualan Buku inserted successfully. ------ \n");
                resetConsoleFontColor();
            }
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

// void deleteDataBukuMenu(const char *filename) {
//     displayData(filename, "buku");

//     printf("\n=========================================================\n");
//     printf("Silahkan input indeks yg ingin di delete: ");
//     int deletedIndex;
//     scanf("%d", &deletedIndex);
//     int resultDelete = deleteDataBuku(filename, deletedIndex);
//     if (resultDelete == 0) {
//         printf("Data Buku indeks ke %d berhasil dihapus", deletedIndex);
//     }
// void deleteBuku() {
//     viewBuku();
//     if (jumlahBuku == 0) return;
    
//     int index;
//     printf("Masukkan index buku yang ingin dihapus: ");
//     scanf("%d", &index);
    
//     if (index < 1 || index > jumlahBuku) {
//         printf("Index tidak valid!\n");
//         return;
//     }
    
//     for (int i = index - 1; i < jumlahBuku - 1; i++) {
//         daftarBuku[i] = daftarBuku[i + 1];
//     }
//     jumlahBuku--;
//     saveBuku();
//     printf("Data Successfully deleted..\n");
// }

// void menu() {
//     int pilihan;
//     while (1) {
//         printf("\nMenu:\n");
//         printf("1. Input\n");
//         printf("2. View History\n");
//         printf("3. View Buku\n");
//         printf("4. Delete History\n");
//         printf("5. Delete Buku\n");
//         printf("6. Exit\n");
//         printf("Pilihan: ");
//         scanf("%d", &pilihan);
        
//         switch (pilihan) {
//             case 1:
//                 printf("Fitur Input belum diimplementasikan.\n");
//                 break;
//             case 2:
//                 printf("Fitur View History belum diimplementasikan.\n");
//                 break;
//             case 3:
//                 viewBuku();
//                 break;
//             case 4:
//                 printf("Fitur Delete History belum diimplementasikan.\n");
//                 break;
//             case 5:
//                 deleteBuku();
//                 break;
//             case 6:
//                 printf("Keluar dari program.\n");
//                 return;
//             default:
//                 printf("Pilihan tidak valid!\n");
//  }
//     }
// }
// int main() 
// {
//     loadBuku();
//     menu();
//     return 0;
// }
