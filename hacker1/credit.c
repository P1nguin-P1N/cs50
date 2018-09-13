#include <stdio.h>
#include <cs50.h>

bool correct(long long number, int len);

int main(){
    long long number;
    char *card;
    card = "INVALID\0";
    do {
        printf("Number: ");
        number = get_long_long();
    } while(number < 4000000000000);

    int len = 0;
    /*вычисление корректности для карт VISA*/
    if (number >= 4000000000000 && number < 5000000000000) {
        len = 13;
        if (correct(number, len))
            card = "VISA\0";
        printf("\n%s\n", card);
        return 0;
    }

    if (number >= 4000000000000000 && number < 5000000000000000) {
        len = 16;
        if (correct(number, len))
            card = "VISA\0";
        printf("\n%s\n", card);
        return 0;
    }
    /*END of calculate VISA card is correct
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Calculate AMEX card is correct*/
    if ((number >= 340000000000000 && number < 350000000000000) ||
       (number >= 370000000000000 && number < 380000000000000)) {
        len = 15;
        if (correct(number, len))
            card = "AMEX\0";
        printf("\n%s\n", card);
        return 0;
    }
    /*END of calculate AMEX card is correct
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Calculate MASTERCARD card is correct*/
    if (number > 5100000000000000 && number < 5699999999999999) {
        len = 16;
        if (correct(number, len))
            card = "MASTERCARD\0";
        printf("\n%s\n", card);
        return 0;
    }
    /*END of calculate AMEX card is correct
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    printf("%lli\n%s\n", number, card);
    return 0;
}

bool correct(long long number, int len){

    int i = 0, sum1 = 0, sum2 = 0, tmp;
        for(i=0; i<len; i++){
            if(i%2){
                tmp = (number%10)*2;
                if (tmp > 9)
                    sum2 += (tmp%10) + (tmp/10);
                else
                    sum2 += tmp;
                    }
            else{
                sum1 += number%10;
            }
            number /= 10;
        }
    printf("sum1 = %d\tsum2 = %d\tsumm = %d\n", sum1, sum2, sum1+sum2);

    return true;
}
