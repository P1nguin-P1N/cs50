//#include<cs50.h>

#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#define _GNU_SOURCE       /* See feature_test_macros(7) */
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<crypt.h>
#include<math.h>

#define HASHLEN     13  // количество символов в обрабатываемом хеше DES
#define MAXLENPASS  30  // максимальная длина пародя

char salt[3]; // переменная для хранения SALT (соли)
char hash[HASHLEN+1] = "00";   // Здесь хранится переданный хеш
char result[MAXLENPASS+1] = ""; // сюда попадет результат, если будет найден
char *crpt = "1234567890123\0";
char ABC[128] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
int ABClen;
unsigned int lenpassmin = 1, lenpassmax = 6;

void findindic(FILE *fdic, unsigned long int count);
unsigned long int countline(FILE *file);
void findgenpass();
void progressbar(int percent);
void getabc(char *pass, unsigned int pl0, unsigned int position);


int main(int argc, char *argv[]){

    if (argc == 1){ // Проверка что есть аргументы
        printf("Введите \"--help\" для подсказки\n");
        return 0;
    }
    if(0 == strcmp(argv[1], "--help")){   // HELP
        printf("\n    Программа поиска пароля по хешу. Шифрование DES.\n\n");
        printf(" команда: ./crack <-h> [-abc <> -d -] \n\n");
        printf(" ПАРАМЕТРЫ:\n");
        printf("   -h    - Ваш хеш 13 знаков, который нужно вскрыть\n");
        printf("   -abc  - Укажите этот параметр, если хотите перебрать все комбинации\n\
\t   паролей из указаного алфавита:\n\
\t     a: [a-z]\n\
\t     A: [A-Z]\n\
\t     1: [0-9]\n\
\t     @: [!\"\':;-+=_~@#$%%^&*()}{[]><,.?/|\\]\n\
\t   Например: -abc a,A,1 создаст алфавит [a-z]+[A-Z]+[0-9]\n");
        printf("   -mabc - Ваш собственный алфавит. Укажите фаил с вашим алфавитом\n\
\t   Считывается только первая строка не более 128 знаков.\n");
        printf("   -dic  - Словарь возможных паролей\n");
        printf("   -l    - Задаёт длину пароля в формате 6 максимальная длина\n\
\t   или в виде двех значений минимальной и максимальной\n\
\t   длины 3,8 - пароль длиой от 3 до 8ми знаков.\n\
\t   Максимальная длина пароля 30. По умолчанию 6 знаков.\n\
\t   Минимальная длина пароля 1. По умолчанию 1 знак.\n\n");
        printf("Пример:\n");
        printf("./crack -h \"50Bpa7n/23iug\" -dic /usr/share/dict/american-english\n\n");
        return 0;
    }
    // Проверка наличия параметра -h
    _Bool h = 0;
    for(int i=1;i<argc-1;i++) {
        if(0 == strcmp(argv[i], "-h")) {
            h = 1;
            printf("Проверяю параметры...\n");
        }
    }
    if(!h) {
        printf("ERROR: обязательный параметр \"-h\" не задан.\n");
        return 1;
    }

    ABClen = strlen(ABC);
    int abc = 0;       // переменная которая хранит информацию о том был указал алфавит в параметрах или нет
    FILE *fdic;
    FILE *fabc;
    unsigned long int countdicline=0;

    for(int i=1; i < argc-1; i++) {
        if(0 == strcmp(argv[i], "-h")) {   // Проверка параметра -h и присвоение значения
            i++;
            if(strlen(argv[i]) == HASHLEN) {
                salt[0] = argv[i][0];
                salt[1] = argv[i][1];
                salt[2] = '\0';
                sprintf(hash, "%s", argv[i]);
            }
            else {
                printf("ERROR: ПАРАМЕТР \"-h\" - Вы указали хеш неправильной длины (13 знаков)\n\n");
                return 1;
            }
        }
        else if(0 == strcmp(argv[i], "-abc")) {  // проверка параметра -abc и формирование алфавита
            i++;
            if (abc != 0)
                printf("Вы указали два алфавита. Будет использован только последний\nуказанный в параметрах алфавит -abc\n");
            strcpy(ABC, "");

            for(unsigned int j=0; j<strlen(argv[i]); j+=2){

                switch (argv[i][j]) {

                case 'a':
                    strcat(ABC, "abcdefghijklmnopqrstuvwxyz");
                    break;
                case 'A':
                    strcat(ABC, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
                    break;
                case '1':
                    strcat(ABC, "0123456789");
                    break;
                case '@':
                    strcat(ABC, "!\"':;-+=_~@#$%^&*()}{[]><,.?/|\\");
                    break;

                default:
                    printf("ERROR: Вы указали неверные параметры для формирования алфавита -abc\n");
                    return 1;
                    break;
                }
            }
            abc++;
            ABClen = strlen(ABC);
            printf("Алфавит \"-abc\" сформирован...\n");
        }

        else if(0 == strcmp(argv[i], "-mabc")) { // определение алфавита пользователем -mabc
            i++;
            if (abc != 0)
                printf("Вы указали два алфавита. Будет использован только последний\nуказанный в параметрах алфавит -mabc\n");

            fabc = fopen(argv[i], "r");
            if (fabc == NULL) {
                printf("File does not exist.\n");
                printf("Не могу открыть фаил алфавита...\n");
                return 1;
            }

            strcpy(ABC, "");
            fgets(ABC, 128, fabc);

            fclose(fabc);
            ABClen = strlen(ABC) - 1;
            ABC[ABClen] = '\0';
        }

        else if(0 == strcmp(argv[i], "-dic")) {   //определение словая для перебора паролей
            i++;
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {
                printf("File does not exist.\n");
                printf("Не могу открыть фаил словаря...\n");
                return 1;
            }
            else {
                countdicline = countline(file);
                fclose(file);
            }
            fdic = fopen(argv[i], "r");
            if (fdic == NULL) {
                printf("File does not exist.\n");
                printf("Не могу открыть фаил словаря...\n");
                return 1;
            }
            else {
                printf("Словарь открыт \"%s\"\n", argv[i]);
                printf("В соваре %lu строк\n", countdicline);
            }
        }
        else if(0 == strcmp(argv[i], "-l")) {    // определение длины пароля для поиска
            i++;
            int max = 0, min = 0, c;
            if(strlen(argv[i]) >1) {
                for(unsigned int k=1, j=0; j<strlen(argv[i]); j++) {
                    if(isdigit(argv[i][j])) {
                        c = argv[i][j] - '0';
                        max = max * k + c;
                        k *= 10;
                    }
                    else if(argv[i][j] == ',') {
                        j++;
                        k = 1;
                        while(j<strlen(argv[i])) {
                            if(isdigit(argv[i][j])) {
                                c = argv[i][j] - '0';
                                min = min*k + c;
                                k *= 10;
                            }
                            else {
                                printf("ERROR: ПАРАМЕТР \"-l\" - задан не верно.\n\n");
                                return 1;
                            }
                            j++;
                        }
                    }
                    else {
                        printf("ERROR: ПАРАМЕТР \"-l\" - задан не верно.\n\n");
                        return 1;
                    }
                }
                // проверка кто из значений максимум. а кто минимум на всякий случай
                lenpassmax = (min < max) ? max : min;
                lenpassmin = (min < max) ? min : max;
            }
            else {
                if(isdigit(argv[i][0])) {
                    c = argv[i][0] - '0';
                    lenpassmax = c;
                }
            }
            // проверка что не равны нулю
            lenpassmax = (lenpassmax > 0) ? lenpassmax : 1;
            lenpassmin = (lenpassmin > 0) ? lenpassmin : 1;
            // проверка что не больше допустимого максимума
            if(lenpassmax > MAXLENPASS)
                printf("WARNING: Максимальная длина пароля установлена %d, больше нельзя.\n", MAXLENPASS);
            lenpassmax = (lenpassmax > MAXLENPASS) ? MAXLENPASS : lenpassmax;
            lenpassmin = (lenpassmin > MAXLENPASS) ? MAXLENPASS : lenpassmin;
        }
        else {
            printf("Please, check input variable and try again...\n\n");
            printf("Inpit \"--help\" for help.\n");
            return 1;
        }
    }

    printf("Алфавит %d знака: %s\n",ABClen, ABC);
    printf("Пароль [%i-%i] знаков\n", lenpassmin, lenpassmax);
    printf("SALT: %s\nHASH: %s\n", salt, hash);

    if(countdicline == 0)
        printf("Мы не используем словарь.\n");
    else {
        printf("Осуществляю поиск по словарю...\n");
        findindic(fdic, countdicline);
        fclose(fdic);
    }
    if(result[0] == '\0')
        findgenpass();
    if(result[0] == '\0') {
        printf("\n~~~~~~~~~~~~~~~~~==================================~~~~~~~~~~~~~~~~\n\n");
        printf("\tК сожалению пароль не найден\n");
        printf("\n~~~~~~~~~~~~~~~~~==================================~~~~~~~~~~~~~~~~\n\n");
        return 0;
    }
// Вывод результата!
    printf("\n~~~~~~~~~~~~~~~~~==================================~~~~~~~~~~~~~~~~\n\n");
    printf("PASSWORD: %s\t SALT: %s  HASH: %s\n", result, salt, crypt(result, salt));
    printf("\n~~~~~~~~~~~~~~~~~==================================~~~~~~~~~~~~~~~~\n");
    return 0;
// END OF MINE FUNCTION!!! ########################
}



void findgenpass() {                   // Функция составления всех возможных вариантов из заданного алфавита
    printf("\nОсуществляю поиск перебором всех возможных вариантов из алфавта...\n");
    char pass[lenpassmax+1];           // строка для работы с подбираеым паролем
    unsigned long int wordcount;       // количество всех возможных вариантов паролей
    for(unsigned int p=lenpassmin-1; p<lenpassmax; p++) { // цикл задающий текушую длину искомого пароля
       wordcount = pow(ABClen, p+1);   // вычисление сколько верояно паролей заданной длины с заданным алфавитом
       printf("Перебираю все слова длиной: %i таких слов %lu ...\n", p+1, wordcount);
       if(result[0] == '\0') {         // проверка, что результат ещё не найден
           pass[p+1] = '\0';
           getabc(pass, p, 0);         // уходим в рекурсивную функцию
       }
       else break;                     // если пароль был найден то выпуливаемся отсюдова!
    if(wordcount > 1000000)            // выдать сообщение при переходе на поиск более длинного пароля, чтоб пользователь не скучал
        printf("\nНе найдено.\n\n");
    }
}

void getabc(char *pass, unsigned int pl0, unsigned int position) { // Магия здесь - перебор паролей
    for(int i=0; i<ABClen; i++) {                                  // ходим по всему алфавиту
        pass[position] = ABC[i];                                   // присваеваем паролю в текущей позиции текущее значение из азбуки
        if(position < pl0 && (result[0] == '\0'))                  // проверяем если наша позиция на символе пароля ещё меньше чем длина нашего пароля и у нас не найден результат
            getabc(pass, pl0, (position+1));                       // прыгаем в эту же функцию снова передав по пути позицию на 1 больше чем здесь
        else {                                                     // иначе, если у нас позиция уже равна длине пароля
            crpt = crypt(pass, salt);                              // вычисляем хеш для текущего варианта пароля
            if(0 == strcmp(crpt, hash) && (result[0] == '\0')) {   // проверяем совпал ли найденый хеш с тем что мы ищем или нет
                strcpy(result, pass);                              // если совпал, то копируем в глобальную переменную result
                break;                                             // ну и эвакуируемся отсель
            }
        }
    }
}

void findindic(FILE *fdic, unsigned long int count) {
    int pl, prcn = 0;
    unsigned long int percent = count / 100;
    char pass[MAXLENPASS+1]; // массив для формирования и обработки паролей
    unsigned long int i = 0;
    printf("\n");
    while(fgets(pass, MAXLENPASS, fdic)) {
        pl = strlen(pass);
        i++;
        pass[pl-1] = '\0';
        crpt = crypt(pass, salt);
        if(!(i%percent)){
            printf("\b\b\b\b\b\b\b\b\b\b\b\b====>[%i%%]", ++prcn);
        }
        if(0 == strcmp(crpt, hash)) {
            strcpy(result, pass);
            break;
        }
    }
    result[0] = '\0';
}

unsigned long int countline(FILE *file) {
    unsigned long int i=0;
    char p[65];
    while(fgets(p, 64, file))
        i++;
    return i;
}
