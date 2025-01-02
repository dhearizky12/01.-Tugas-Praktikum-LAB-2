// Perhitungan Kupon Diskon
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXC 1024000
#define MIN_TOTAL_AMOUNT 1000
#define DISCOUNT_PERCENTAGE 0.05

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
    char inputTotal[MAXC];

    while (1)
    {
        fputs("\nMasukkan total pembelian (min Rp 1000): Rp ", stdout); /* prompt for input */

        fflush(stdin);

        if (fgets(inputTotal, MAXC, stdin) == NULL)
        {
            puts("(user canceled input)"); /* read entire line */
            return;                        /* handle manual EOF */
        }

        /* check char in array to see if it is valid, if so, break loop */
        int i = 0;
        int isValid = 1;
        while (inputTotal[i] != '\n')
        {
            if (isalpha(inputTotal[i]))
            {
                fputs("\nMohon input total pembelian yang benar (min Rp 1000).\n", stderr);
                isValid = 0;
                break;
            }
            i++;
        }

        if (isValid)
        {
            char *end;
            totalAmount = strtof(inputTotal, &end);

            if (totalAmount != 0 && totalAmount >= MIN_TOTAL_AMOUNT)
            {
                break;
            }
            else
            {
                fputs("\nMohon input total pembelian yang benar (min Rp 1000).\n", stderr);
            }
        }
    }

    // Hitung kupon undian
    if (totalAmount >= 100000)
    {
        kupon = (int)(totalAmount / 100000);
        priceDiscount = totalAmount * DISCOUNT_PERCENTAGE;
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
