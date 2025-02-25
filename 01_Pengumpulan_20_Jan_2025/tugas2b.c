// perhitungan gaji pegawai

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> // Melakukan impor standar library C yg berisi fungsi untuk konversi tipe data.

//membuat struct pegawai untuk mengelompokkan struktur variable/atribut pegawai
typedef struct 
{ 
    // inisialisasi variable / atribut terkait instance pegawai
    char nip[20]; //nomor induk pegawai dengan maksimal karakter 20
    char nama[255]; //nama pegawai dengan maksimal karakter 255
    char alamat[255]; //alamat pegawai dengan maksimal karakter 255
    char noHp[16]; //nomor hp pegawai dengan maksimal digit 16
    char jabatan[30]; //jabatan pegawai dengan maksimal karakter 30
    char golongan[3]; //golongan pegawai dengan maksimal karakter 3
    int gajiPokok; //inisialisasi variable gajiPokok dengan tipe integer
    int tarifLemburPerJam; //tarif lembur per jam
} Pegawai;

//prototype fungsi
void InputDetailPegawai( Pegawai *pegawai );
void CalculateGajiPokokPegawai( Pegawai *pegawai );
float HitungGajiLembur(Pegawai *pegawai, float jamLembur);
void PrintDataPegawai(Pegawai *pegawai);
void ProsesHitungLembur(Pegawai *pegawai);
void getValidGolongan( Pegawai * pegawai ); 
void getValidPhoneNumber( Pegawai * pegawai );
void getValidHour ( float * jamLemburPegawai );

int main() 
{
    char pilihan;
    Pegawai pegawai; // deklarasi variable pegawai dengan tipe data Pegawai
    /*
        function - function di bawah ini di beri parameter referensi ke objek Pegawai 
        untuk mengisi detail informasi pegawai seperti NIP, nama, alamat,
        Dengan menggunakan pointer (*pegawai), fungsi ini dapat langsung memodifikasi data 
        dalam memori, sehingga nilai-nilai yang diinput akan disimpan langsung pada variabel pegawai yang diberikan.
    */

    do {
        //input detail pegawai
        InputDetailPegawai( &pegawai );

        //hitung gaji pokok berdasarkan golongan
        CalculateGajiPokokPegawai( &pegawai );

        //cetak data pegawai
        PrintDataPegawai(&pegawai);

        //proses perhitungan lembur
        ProsesHitungLembur(&pegawai);

        // Tanyakan kepada pengguna apakah ingin menginput data lagi
        printf("\nApakah Anda ingin menginput data lagi? (y/n): ");
        scanf("\n %c", &pilihan);
    } while (pilihan == 'y' || pilihan == 'Y');

    return 0;
}

void getValidPhoneNumber( Pegawai * pegawai )
{
    int valid;
    do {
        valid = 1;
        printf("Masukkan No HP: ");
        scanf("%s", pegawai->noHp);

        // Periksa apakah input valid (hanya angka)
        for (int i = 0; pegawai->noHp[i] != '\0'; i++) 
        {
            if (!isdigit(pegawai->noHp[i])) 
            {
                valid = 0;
                printf("Input tidak valid. Silakan masukkan nomor HP yang benar.\n");
                break;
            }
        }
    } while (!valid);
}

void getValidGolongan( Pegawai * pegawai )
{
     // Validasi input golongan menggunakan do-while  
    do {  
        printf("Masukkan Golongan (D1/D2/D3): ");  
        scanf("%2s", pegawai->golongan); // Batasi input menjadi 2 karakter untuk mencegah buffer overflow  

        // Membersihkan buffer input jika ada karakter tambahan  
        // while (getchar() != '\n');  
        // Periksa apakah input valid  
        if (strcmp(pegawai->golongan, "D1") != 0 && strcmp(pegawai->golongan, "D2") != 0 && strcmp(pegawai->golongan, "D3") != 0) 
        {  
            printf("\nInput tidak valid. Silakan coba lagi.\n");  
        }  
    } while (strcmp(pegawai->golongan, "D1") != 0 && strcmp(pegawai->golongan, "D2") != 0 && strcmp(pegawai->golongan, "D3") != 0);
}

void InputDetailPegawai( Pegawai *pegawai )
{
    // Input data pegawai
    printf("\nMasukkan NIP: ");
    scanf("%s", pegawai->nip);
    printf("Masukkan Nama: ");
    scanf(" %[^\n]s", pegawai->nama);  // Untuk membaca string dengan spasi
    printf("Masukkan Alamat: ");
    scanf(" %[^\n]s", pegawai->alamat); // Untuk membaca string dengan spasi
    // Validasi input nomor HP
    getValidPhoneNumber( pegawai );

    printf("Masukkan Jabatan: ");
    scanf(" %[^\n]s", pegawai->jabatan);
    getValidGolongan( pegawai );
}

void CalculateGajiPokokPegawai( Pegawai *pegawai )
{
    // Tentukan gaji pokok dan lembur per jam

    /*
        Fungsi strcmp (string compare) dalam C digunakan untuk membandingkan dua string. 
        Fungsi ini tersedia di pustaka string.h.
        Jika kedua string sama, Fungsi mengembalikan 0.
    */
    if (strcmp(pegawai->golongan, "D1") == 0) 
    {
        pegawai->gajiPokok = 2500000;
        pegawai->tarifLemburPerJam = 10000;
    } 
    else if (strcmp(pegawai->golongan, "D2") == 0) 
    {
        pegawai->gajiPokok = 2000000;
        pegawai->tarifLemburPerJam = 5000;
    } 
    else if (strcmp(pegawai->golongan, "D3") == 0) 
    {
        pegawai->gajiPokok = 1500000;
        pegawai->tarifLemburPerJam = 2500;
    } 
    else 
    {
        printf("Golongan tidak valid.\n");
        pegawai->gajiPokok = 0;
        pegawai->tarifLemburPerJam = 0;
    }
}

float HitungGajiLembur(Pegawai *pegawai, float jamLembur)
{
    return jamLembur * pegawai->tarifLemburPerJam;
}

void PrintDataPegawai(Pegawai *pegawai)
{
    printf("\n============== Detail Data Pegawai =============\n");
    printf("\nData Pegawai:\n");
    printf("NIP: %s\n", pegawai->nip);
    printf("Nama: %s\n", pegawai->nama);
    printf("Alamat: %s\n", pegawai->alamat);
    printf("No HP: %s\n", pegawai->noHp);
    printf("Jabatan: %s\n", pegawai->jabatan);
    printf("Golongan: %s\n", pegawai->golongan);
    printf("Gaji Pokok: Rp %d\n", pegawai->gajiPokok);
    printf("\n==================================================\n");
}

void ProsesHitungLembur(Pegawai *pegawai)
{
    float jamLemburPegawai, totalGaji;

    // Input jam lembur
    getValidHour ( &jamLemburPegawai );

    // Hitung total gaji dengan lembur
    totalGaji = pegawai->gajiPokok + HitungGajiLembur(pegawai, jamLemburPegawai);

    // Output hasil
    printf("\n============== Detail Gaji Pegawai =============\n");
    printf("\nNIP: %s\n", pegawai->nip);
    printf("Golongan: %s\n", pegawai->golongan);
    printf("Lembur: %.2f jam\n", jamLemburPegawai);
    printf("Total Gaji Bulan ini: Rp %.2f\n", totalGaji);
    printf("\n==================================================\n");
}

void getValidHour(float *jamLemburPegawai)
{
    char input[10];
    int valid;
    do {
        valid = 1;
        printf("\n---- Detail Data Lembur ----\n");
        printf("Masukkan Jam Lembur: ");
        scanf("%s", input);
        printf ("----------------------------\n");

        // Periksa apakah input valid (hanya angka)
        for (int i = 0; input[i] != '\0'; i++) 
        {
            if (!isdigit(input[i]) && input[i] != '.') 
            {
                valid = 0;
                break;
            }
        }

        if (valid) {
            *jamLemburPegawai = atof(input); // mengubah string menjadi float
            if (*jamLemburPegawai < 0) 
            {
                printf("Input tidak valid. Silakan coba lagi.\n");
                valid = 0;
            }
        } else 
        {
            printf("Input tidak valid. Silakan coba lagi.\n");
        }

        // Membersihkan buffer input
        while (getchar() != '\n');
    } while (!valid);
}
