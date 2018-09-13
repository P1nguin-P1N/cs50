/*
 * vigenere.c
 *
 *  Created on: 06.09.2018
 *      Author: pin
 */

#include<stdio.h>
//#include"cs50.h"
//#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define SIZELN    1000

int main(int argc, char *argv[]){
    if (argc != 2){
    	printf("Введите одно слово-ключ, как аргумент\n");
        return 1;
    }
    int len = strlen(argv[1]);
    int key[len];
    for(int i=0; i<len; i++){
    	if(islower(argv[1][i]))
    		key[i] = argv[1][i] - 'a';
    	else if (isupper(argv[1][i]))
    		key[i] = argv[1][i] - 'A';
    }

    char string[SIZELN], cryptostring[SIZELN];
    printf(": ");
    fgets(string, SIZELN, stdin);
    int size = strlen(string);

    for(int j=0,i=0; i<size;i++){
    	if(islower(string[i])){ // медод определения, что у меня буква и она маленькая
    		cryptostring[i] = ((string[i] - 'a') + key[j%len]) % 26 + 'a'; // вычисляю смещение буквы на ключ
    		j++;
    	}
    	else if(string[i] >= 'A' && string[i] <= 'Z'){ // это мой способ определить, что буква и она большая
    		cryptostring[i] = ((string[i] - 'A') + key[j%len]) % 26 + 'A';
    		j++;
    	}
    	else cryptostring[i] = string[i];
    }
    cryptostring[size] = '\0';
    printf("%s", cryptostring);

    return 0;
}

