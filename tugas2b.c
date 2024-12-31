// perhitungan gaji pegawai

#include <stdio.h>
#include <string.h>

typedef struct {
    char nip[20];
    char nama[50];
    char alamat[100];
    char noHp[15];
    char jabatan[30];
    char golongan[3];
    int gajiPokok;
    int lemburPerJam;
} Pegawai;

int main() {
    Pegawai pegawai;
    int jamLembur, totalGaji;

    // Input data pegawai
    printf("Masukkan NIP: ");
    scanf("%s", pegawai.nip);
    printf("Masukkan Nama: ");
    scanf(" %[^\n]s", pegawai.nama);  // Untuk membaca string dengan spasi
    printf("Masukkan Alamat: ");
    scanf(" %[^\n]s", pegawai.alamat);
    printf("Masukkan No HP: ");
    scanf("%s", pegawai.noHp);
    printf("Masukkan Jabatan: ");
    scanf(" %[^\n]s", pegawai.jabatan);
    printf("Masukkan Golongan (D1/D2/D3): ");
    scanf("%s", pegawai.golongan);

    // Tentukan gaji pokok dan lembur per jam
    if (strcmp(pegawai.golongan, "D1") == 0) {
        pegawai.gajiPokok = 2500000;
        pegawai.lemburPerJam = 10000;
    } else if (strcmp(pegawai.golongan, "D2") == 0) {
        pegawai.gajiPokok = 2000000;
        pegawai.lemburPerJam = 5000;
    } else if (strcmp(pegawai.golongan, "D3") == 0) {
        pegawai.gajiPokok = 1500000;
        pegawai.lemburPerJam = 2500;
    } else {
        printf("Golongan tidak valid.\n");
        return 1;
    }

    // Output data pegawai
    printf("\nData Pegawai:\n");
    printf("NIP: %s\n", pegawai.nip);
    printf("Nama: %s\n", pegawai.nama);
    printf("Alamat: %s\n", pegawai.alamat);
    printf("No HP: %s\n", pegawai.noHp);
    printf("Jabatan: %s\n", pegawai.jabatan);
    printf("Golongan: %s\n", pegawai.golongan);
    printf("Gaji Pokok: Rp %d\n", pegawai.gajiPokok);

    // Input jam lembur
    printf("\nMasukkan jumlah jam lembur: ");
    scanf("%d", &jamLembur);

    // Hitung total gaji
    totalGaji = pegawai.gajiPokok + (jamLembur * pegawai.lemburPerJam);

    // Output hasil
    printf("\nNIP: %s\n", pegawai.nip);
    printf("Golongan: %s\n", pegawai.golongan);
    printf("Lembur: %d jam\n", jamLembur);
    printf("Total Gaji Bulan ini: Rp %d\n", totalGaji);

    return 0;
}
