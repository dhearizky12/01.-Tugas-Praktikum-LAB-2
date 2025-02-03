// Perhitungan Kupon Diskon

#include <stdio.h> // Melakukan impor standar library C yg berisi fungsi prototipe untuk input/output standar. Dalam kode program ini fungsi yg di impor dari standar library ini adalah printf, scanf, fputs, fflush, fgets dan puts
#include <ctype.h> // Melakukan impor standar library C yg berisi fungsi untuk pemrosesan karakter. Contoh dalam kode program ini adalah isalpha
#include <stdlib.h> // Melakukan impor standar library C yg berisi fungsi untuk konversi tipe data, alokasi memori, dan fungsi utilitas lainnya. Fungsi yg diimpor dari library ini adalah strtof

/* Penggunaan makro (#define) membantu membuat kode lebih mudah dibaca dan dipelihara. 
   Jika perlu mengubah nilai seperti batas maksimum atau persentase diskon, 
   hal ini hanya perlu mengubahnya di satu tempat, dan perubahan tersebut akan diterapkan ke seluruh program */

#define MAXC 1024000 // Konstanta yg menyimpan nilai setara dengan 1MB
#define MIN_TOTAL_AMOUNT 1000 // Konstanta yg menyimpan nilai minimum total pembelian
#define DISCOUNT_PERCENTAGE 0.05 // Konstanta yg menyimpan nilai setara dengan 5%, untuk kebutuhan kalkulasi diskon

// Deklarasi fungsi
void InputMemberCustomer(); // Deklarasi fungsi yg akan dipanggil jika customer memiliki member
void InputAmountPaymentCustomer(char member[]); // Deklarasi fungsi yg akan dipanggil untuk user melakukan input total pembelian dan melakukan perhitungan diskon
void PrintInvoiceCustomer(char member[], float totalAmount, int kupon, float priceDiscount, float totalPaymentAfterDiscount); // Deklarasi fungsi yg akan dipanggil pada akhir program, untuk menampilkan invoice atau summary dari total pembelian

/* kode program ini meminta pengguna untuk menentukan apakah mereka memiliki keanggotaan di supermarket 
dan kemudian memanggil fungsi yang sesuai berdasarkan pilihan mereka */

int main() // Fungsi utama dari program C
{ // Awal dari blok fungsi main
    int choice; // Variabel untuk menampung input pilihan dari user
    do // Keyword untuk perulangan do-while, untuk mendefinisikan kode yg akan diulangi hingga kondisi tidak terpenuhi
    { // Awal dari blok do
        printf("\n=================================================="); // Menampilkan output sebagai header dari menu utama program
        printf("\n| SELAMAT DATANG di SUPERMARKET BINUS KELOMPOK 4 |"); // Menampilkan output sebagai header dari menu utama program
        printf("\n=================================================="); // Menampilkan output sebagai header dari menu utama program
        printf("\n| Apakah customer memiliki member ? |"); // Menampilkan output pertanyaan kepemilikan member
        printf("\n| 1. Ya | "); // Menampilkan output opsi dengan Ya atau memiliki member
        printf("\n| 2. Tidak |"); // Menampilkan output opsi dengan Tidak atau tidak memiliki member

        printf("\n\nPilihan anda : "); // Menampilkan output sebelum menerima input user
        scanf("%d", &choice); // Menerima input dari user dan menyimpan ke dalam variabel choice

        switch (choice) // Keyword untuk proses seleksi berdasarkan nilai dari variabel choice
        { // Awal dari blok switch
        case 1: // Mendefinisikan blok kode yg akan dijalankan jika variabel choice bernilai 1
            InputMemberCustomer(); // Mengeksekusi fungsi InputMemberCustomer
            break; // Kode penanda akhir dari case 2 
        case 2: // Mendefinisikan blok kode yg akan dijalankan jika variabel choice bernilai 2
            InputAmountPaymentCustomer(""); // Mengeksekusi fungsi InputAmountPaymentCustomer dengan string kosong, yang mengindikasikan tidak memiliki member
            break; // Kode penanda akhir dari case 2

        default: // Mendefinisikan blok kode yg akan dijalankan jika variabel choice tidak bernilai 1 atau 2
            printf("\nPilihan tidak valid. Silahkan coba lagi. \n");
        } // Akhir dari blok switch
    } while (choice != 2); // Akhir dari blok do, dan kode akan diulang hingga input dari user sama dengan 2

    return 0; // Line terakhir dari fungsi main, untuk menutup program tanpa error
} // Akhir dari blok fungsi main

/* kode ini mengelola input ID member dan meneruskannya ke fungsi berikutnya untuk pemrosesan lebih lanjut */

void InputMemberCustomer() // Mendefinisikan fungsi dengan return type void dan nama fungsi InputMemberCustomer dan tanpa parameter
{ // Awal dari blok fungsi InputMemberCustomer
    char member[10] = ""; // Deklarasi array member yg akan menampung 10 karakter, dan inisiasi variabel member dengan string kosong
    // Input member
    printf("Masukkan ID member :"); // Menampilkan output sebelum menerima input member dari user
    scanf("%s", member); // Menerima input dari user dan menyimpan ke dalam variabel member
    InputAmountPaymentCustomer(member); // Mengeksekusi fungsi InputAmountPaymentCustomer dengan variabel member yg berisi input dari user
} // Akhir dari blok fungsi InputMemberCustomer

/* kode ini memastikan bahwa pengguna memasukkan jumlah pembelian yang valid (menerima dan memvalidasi input total pembelian dari pengguna) 
sebelum melanjutkan ke langkah berikutnya dalam proses pembayaran. */

void InputAmountPaymentCustomer(char member[]) // Mendefinisikan fungsi dengan return type void dan nama fungsi InputAmountPaymentCustomer yg menerima char array sebagai parameter
{ // Awal dari blok fungsi InputAmountPaymentCustomer
    int kupon; // Deklarasi variabel untuk jumlah kupon yang didapatkan
    float totalAmount, priceDiscount, totalPaymentAfterDiscount; // Deklarasi variabel untuk menampung total pembelian, jumlah diskon, dan total setelah diskon
    char inputTotal[MAXC]; // Deklarasi variabel untuk menampung input user

    while (1) // Keyword untuk perulangan while, dimana perulangan ini tidak akan berhenti jika program tidak menemukan keyword break. Atau biasa disebut dengan infinite loop
    { // Awal dari blok while
        fputs("\nMasukkan total pembelian (min Rp 1000): Rp ", stdout); /* prompt for input */

        fflush(stdin); // Mengeksekusi fungsi fflush dari standar library stdio.h, untuk menghapus dan mereset input buffer

        if (fgets(inputTotal, MAXC, stdin) == NULL) // Melakukan eksekusi terhadap fungsi fgets dari library stdio.h, untuk melakukan validasi apakah program berhasil membaca input dari user
        { // Awal dari blok if
            puts("(user canceled input)"); /* read entire line */
            return;                        /* handle manual EOF */
        } // Akhir dari blok if

        /* check char in array to see if it is valid, if so, break loop */
        int i = 0; // Melakukan deklarasi variabel i untuk menampung
        int isValid = 1; // Melakukan deklarasi variabel isValid dengan nilai 1 yg mengindikasikan input valid
        while (inputTotal[i] != '\n') // Melakukan perulangan hingga elemen ke i dari inputTotal adalah new line atau \n yg mengindikasikan akhir dari input user
        { // Awal dari blok while
            if (isalpha(inputTotal[i])) // Melakukan pengecekan terhadap elemen indeks ke i pada array inputTotal apakah karakter merupakan alfabet
            { // Awal dari blok if
                fputs("\nMohon input total pembelian yang benar (min Rp 1000).\n", stderr); // Menampilkan output untuk meminta user melakukan input ulang
                isValid = 0; // Memberikan nilai 0 pada variabel isValid yg mengindikasikan input user tidak sesuai
                break; // Keyword untuk menghentikan perulangan while pengecekan array input user
            } // Akhir dari blok if
            i++; // Melakukan increment sejumlah 1 terhadap variabel i
        } // Akhir dari blok while

        if (isValid) // Keyword untuk proses seleksi if, jika isValid tidak sama dengan 0 maka blok kode akan dijalankan
        { // Awal dari blok if
            char *end; // Deklarasi pointer end yg dibutuhkan untuk eksekusi fungsi strtof
            totalAmount = strtof(inputTotal, &end); // Mengeksekusi fungsi strtof untuk mengkonversi tipe data char menjadi float, dan menyimpan hasil konversi kedalam variabel totalAmount

            if (totalAmount != 0 && totalAmount >= MIN_TOTAL_AMOUNT) // Melakukan pengecekan terhadap input user yg sudah dikonversi kedalam tipe data float
            { // Awal dari blok if
                break; // keyword untuk menghentikan perulangan while
            } // Akhir dari blok if
            else // Blok kode yg akan dieksekusi jika totalAmount tidak sama dengan 0 dan totalAmount lebih dari sama dengan 1000
            { // Awal dari blok else
                fputs("\nMohon input total pembelian yang benar (min Rp 1000).\n", stderr); // Menampilkan output untuk meminta user melakukan input ulang
            } // Akhir dari blok else
        } // Akhir dari blok if
    } // Akhir dari blok while

    /* Kode ini menangani logika untuk menentukan apakah pelanggan berhak mendapatkan diskon berdasarkan jumlah total pembelian mereka, 
       menghitung diskon yang berlaku, dan mempersiapkan informasi yang diperlukan untuk mencetak faktur.*/
    
    if (totalAmount >= 100000) // Keyword untuk proses seleksi if dengan kondisi totalAmount lebih besar sama dengan 100000
    { // Awal dari blok if
        kupon = (int)(totalAmount / 100000); // Memberikan nilai pada variabel kupon berdasarkan hasil perhitungan totalAmount dibagi 100000
        priceDiscount = totalAmount * DISCOUNT_PERCENTAGE; // Melakukan kalkulasi untuk mendapatkan harga diskon
        totalPaymentAfterDiscount = totalAmount - priceDiscount; // Melakukan kalkulasi jumlah yg harus dibayar setelah diskon
    } // Akhir dari blok if
    else // Blok kode yg akan dieksekusi jika totalAmount kurang dari 100000
    { // Awal dari blok else
        kupon = 0; // Memberikan nilai default 0 untuk variabel kupon jika totalAmount kurang dari 100000
        priceDiscount = 0; // Memberikan nilai default 0 untuk variabel priceDiscount jika totalAmount kurang dari 100000
        totalPaymentAfterDiscount = totalAmount; // Memberikan nilai untuk variabel totalPaymentAfterDiscount sejumlah totalAmount karena tidak mendapatkan diskon
    } // Akhir dari blok else

    PrintInvoiceCustomer(member, totalAmount, kupon, priceDiscount, totalPaymentAfterDiscount); // Mengeksekusi fungsi PrintInvoiceCustomer
} // Akhir dari blok fungsi InputAmountPaymentCustomer

/* kode ini menyusun dan mencetak faktur yang jelas dan terstruktur untuk pelanggan, menampilkan informasi penting seperti ID member, 
   total pembelian, jumlah kupon, diskon, dan total yang harus dibayar. */

void PrintInvoiceCustomer(char member[], float totalAmount, int kupon, float priceDiscount, float totalPaymentAfterDiscount) // Mendefinisikan fungsi dengan return type void dan nama fungsi PrintInvoiceCustomer
{ // Awal dari blok fungsi PrintInvoiceCustomer
    // Output hasil
    printf("\n|================================================|"); // Menampilkan output sebagai header dari invoice
    printf("\n| +++++++++++++++++ | INVOICE| ++++++++++++++++++|"); // Menampilkan output sebagai header dari invoice
    printf("\n|================================================|"); // Menampilkan output sebagai header dari invoice
    printf("\nMember : %s", member); // Menampilkan kembali isi variabel member dari input user jika customer memilih menu 1 di menu utama
    printf("\nTotal Pembelian: Rp %.2f\n", totalAmount); // Menampilkan kembali isi variabel totalAmount dari input user
    printf("Jumlah Kupon: %d\n", kupon); // Menampilkan jumlah kupon yang didapatkan
    printf("Diskon: Rp %.2f\n", priceDiscount); // Menampilkan jumlah diskon yang didapatkan
    printf("Total yang harus dibayar: Rp %.2f\n", totalPaymentAfterDiscount); // Menampilkan Total harga yang harus dibayar setelah diskon
} // Akhir dari blok fungsi PrintInvoiceCustomer
