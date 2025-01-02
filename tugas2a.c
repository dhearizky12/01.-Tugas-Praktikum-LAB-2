// Perhitungan Kupon Diskon
#include <stdio.h>

// Deklarasi fungsi
void InputMemberCustomer();
void InputAmountPaymentCustomer(char member[]);
void PrintInvoiceCustomer(char member[], float totalAmount, int kupon, float priceDiscount, float totalPaymentAfterDiscount);

int main()
{
    int choice;
    do
    {
        printf("\n==================================================");
        printf("\n| SELAMAT DATANG di SUPERMARKET BINUS KELOMPOK 4 |");
        printf("\n==================================================");
        printf("\n| Apakah customer memiliki member ? |");
        printf("\n| 1. Ya | ");
        printf("\n| 2. Tidak |");

        printf("\n\nPilihan anda : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            InputMemberCustomer();
            break;
        case 2:
            InputAmountPaymentCustomer("");
            break;

        default:
            printf("\nPilihan tidak valid. Silahkan coba lagi. \n");
        }
    } while (choice != 2);

    return 0;
}

void InputMemberCustomer()
{
    char member[10] = "";
    // Input member
    printf("Masukkan ID member :");
    scanf("%s", member);
    InputAmountPaymentCustomer(member);
}

void InputAmountPaymentCustomer(char member[])
{
    int kupon;
    float totalAmount, priceDiscount, totalPaymentAfterDiscount;
    double discountPrecentage = 0.05;

    while (1)
    {
        printf("\nMasukkan total pembelian: Rp ");
        scanf("%f", &totalAmount);
        if (totalAmount >= 1000)
        {
            break;
        }
        else
        {
            printf("\nMohon input total pembelian yang benar. \n");
        }
    }

    // Hitung kupon undian
    if (totalAmount >= 100000)
    {
        kupon = (int)(totalAmount / 100000);
        priceDiscount = totalAmount * discountPrecentage;
        totalPaymentAfterDiscount = totalAmount - priceDiscount;
    }
    else
    {
        kupon = 0;
        priceDiscount = 0;
        totalPaymentAfterDiscount = totalAmount;
    }

    PrintInvoiceCustomer(member, totalAmount, kupon, priceDiscount, totalPaymentAfterDiscount);
}

void PrintInvoiceCustomer(char member[], float totalAmount, int kupon, float priceDiscount, float totalPaymentAfterDiscount)
{
    // Output hasil
    printf("\n|================================================|");
    printf("\n| +++++++++++++++++ | INVOICE| ++++++++++++++++++|");
    printf("\n|================================================|");
    printf("\nMember : %s", member);
    printf("\nTotal Pembelian: Rp %.2f\n", totalAmount);
    printf("Jumlah Kupon: %d\n", kupon);
    printf("Diskon: Rp %.2f\n", priceDiscount);
    printf("Total yang harus dibayar: Rp %.2f\n", totalPaymentAfterDiscount);
}
