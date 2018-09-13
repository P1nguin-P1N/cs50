/*
 * caesar.c
 * Программа шифрует или дешифрует строку. она смещает буквы на заданный ключ,
 * который необходимо передать как аргумент при запуске программы,
 * например ./caesar 13
 * : Шифруемая строка латиницей
 *  Created on: 05.09.2018
 *      Author: pin
 */
#include<stdio.h>
//#include"cs50.h"
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define SIZELN    1000

int main(int argc, char *argv[]){
    if (argc != 2){
    	printf("Введите один целочисленный аргумент\n");
        return 1;
    }
    int key;
    key = atoi(argv[1]);
    key %= 26;
    char string[SIZELN], cryptostring[SIZELN];
    printf(": ");
    while(fgets(string, SIZELN, stdin) == NULL);
    int size = strlen(string);

    for(int i=0; i<size;i++){
    	if(islower(string[i])) // медод определения, что у меня буква и она маленькая
    			cryptostring[i] = ((string[i] - 'a') + key) % 26 + 'a'; // вычисляю смещение буквы на ключ
    	else if(string[i] >= 'A' && string[i] <= 'Z') // это мой способ определить, что буква и она большая
    		cryptostring[i] = ((string[i] - 'A') + key) % 26 + 'A';
    	else cryptostring[i] = string[i];
    }
    cryptostring[size] = '\0';

    printf("\n%s\n", cryptostring);

    return 0;
}

