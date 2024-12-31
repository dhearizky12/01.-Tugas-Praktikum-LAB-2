//Perhitungan Kupon Diskon
#include <stdio.h>

int main() {
    float totalPembelian, diskon, totalBayar;
    int kupon;

    // Input total pembelian
    printf("Masukkan total pembelian: Rp ");
    scanf("%f", &totalPembelian);

    // Hitung kupon undian
    if (totalPembelian >= 100000) {
        kupon = (int)(totalPembelian / 100000);
        diskon = totalPembelian * 0.05;
        totalBayar = totalPembelian - diskon;
    } else {
        kupon = 0;
        diskon = 0;
        totalBayar = totalPembelian;
    }

    // Output hasil
    printf("\nTotal Pembelian: Rp %.2f\n", totalPembelian);
    printf("Jumlah Kupon: %d\n", kupon);
    printf("Diskon: Rp %.2f\n", diskon);
    printf("Total yang harus dibayar: Rp %.2f\n", totalBayar);

    return 0;
}
