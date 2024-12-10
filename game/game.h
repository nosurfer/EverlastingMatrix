#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../functions/additionalFunctions.h"

typedef struct {
    char *author;
    char *text;
} Dialog;

void startMenu() {
    clearScreen();
    printf("Everlasting matrix\n");
    printf("Start [1]\nExit  [0]\n > ");
}

void startGame() {
    char afile_name[20], file_name[20];
    int counter = 1;
    int line_number;
    int choice;

    while (1) {
        sprintf(afile_name, "arts/%d.art", counter);
        sprintf(file_name, "levels/%d.lvl", counter);

        if (access(file_name, F_OK) != 0) {
            clearScreen();
            printf("У каждой истории есть начало и конец. У каждой истории есть своя канва, синопсис, содержание, ключевые моменты, прологи и эпилоги. И нет такой книги, в которой при каждом новом прочтении не открывались бы вещи, на которые раньше не обращал внимания. Но у каждой книги есть своя последняя страница, перевернув которую, мы ставим книгу на полку. Чтобы завтра открыть новую.\nСпасибо за игру!\n");
            break;
        }

        FILE *file = fopen(file_name, "r");
        if (file == NULL) {
            printf("Ошибка: Не могу открыть файл %s\n", file_name);
            return;
        }

        line_number = countLines(file);
        rewind(file);

        char line[1024];
        size_t i = 1;


        while (i <= line_number) {
            rewind(file);
            size_t current_line = 1;
            while (current_line <= i && fgets(line, sizeof(line), file)) {
                current_line++;
            }

            Dialog *dialog = malloc(sizeof(Dialog));
            if (dialog == NULL) {
                printf("Ошибка: Нехватка памяти!\n");
                fclose(file);
                return;
            }

            line[strcspn(line, "\r\n")] = '\0';

            char *buffer = strdup(line);
            const char delimiter[2] = "@";
            dialog->author = strdup(strtok(buffer, delimiter));
            dialog->text = strdup(strtok(NULL, delimiter));
            free(buffer);

            clearScreen();
            asciiArt(afile_name);
            printf("%i/%i\n", i, line_number);
            printf("%s:\n", dialog->author);

            for (size_t i = 0; i < strlen(dialog->text); i++) {
                printf("%c", dialog->text[i]);
                usleep(20000);
            }
            sleep(1); 
            printf("\n");
            
            free(dialog->author);
            free(dialog->text);
            free(dialog);

            while (1) {
                printf("Дальше [1]\n");
                printf("Назад  [0]\n");
                printf(" > ");
                scanf("%i", &choice);
                if (choice == 1) {
                    i++;
                    break;
                } else if (i > 1) {
                    i--;
                    break;
                } else {
                    printf("Действие невозможно.\n");
                }
            }
        }

        fclose(file);
        counter++;
    }
}


void game() {
    startMenu();
    int choice;
    scanf("%i", &choice);
    if (choice == 1) {
        startGame();
    } else {
        printf("До свидания! Жаль, что ты уже уходишь!\n");
    }
}