#include <stdio.h>

int main(){
    int H,i;
    printf("Input heigh of pyremind: ");
    scanf("%d", &H);
    char blocks1[H], blocks2[H];
    for(i = 0; i<H; i++){
        blocks1[i] = blocks2[i] = ' ';
    }
    for(i = 0; i<H; i++){
        blocks1[H-1-i] = blocks2[i] = '#';
        printf("%s %s\n", blocks1, blocks2);
    }

    return 0;
}
