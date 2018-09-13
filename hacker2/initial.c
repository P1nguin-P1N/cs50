//#include<cs50.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>

int main(){
    printf("Input you full name: ");
    char name[50];
    fgets(name, 50, stdin);
    char initial[9];
    int lenght = strlen(name);
    int j=0;
    if(isalpha(name[0]))
        initial[j++] = toupper(name[0]);

    for(int i=1; (i<lenght && j<8); i++){
        if(isalpha(name[i]) && name[i-1] == ' ')
            initial[j++] = toupper(name[i]);
    }
    initial[j] = '\0';
    printf("%s\n", initial);
    return 0;
}