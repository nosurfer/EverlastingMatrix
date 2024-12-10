#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <unistd.h>
#endif

void asciiArt(char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Ошибка: Не могу открыть файл %s\n", file_name);
        return;
    }
    char buf[2048];
    size_t bytesRead = fread(buf, 1, sizeof(buf) - 1, file);
    if (bytesRead == 0) {
        printf("Ошибка чтения файла или файл пустой.\n");
        fclose(file);
        return;
    }
    buf[bytesRead] = '\0';
    fputs(buf, stdout);
    printf("\n");
    fclose(file);
}

void clearScreen() {
    #ifdef _WIN32
        COORD coordScreen = {0, 0};
        DWORD dwCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        GetConsoleScreenBufferInfo(hConsole, &csbi);
        DWORD dwSize = csbi.dwSize.X * csbi.dwSize.Y;

        FillConsoleOutputCharacter(hConsole, ' ', dwSize, coordScreen, &dwCharsWritten);
        FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwSize, coordScreen, &dwCharsWritten);

        SetConsoleCursorPosition(hConsole, coordScreen);
    #elif __linux__ || __APPLE__
        printf("\033[H\033[J");
    #endif
}

int countLines(FILE *file) {
    char buf[2048];
    int counter = 0;
    for (;;) {
        size_t res = fread(buf, 1, sizeof(buf), file);
        if (ferror(file))
            return -1;

        int i;
        for(i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(file))
            break;
    }
    return counter;
}

long prevLine(FILE *file) {
    long pos = ftell(file);
    if (pos == 0) {
        return -1;
    }
    while (pos > 0) {
        fseek(file, --pos, SEEK_SET);
        if (fgetc(file) == '\n' && pos > 0) {
            break;
        }
    }
    fseek(file, pos == 0 ? 0 : pos + 1, SEEK_SET);
    return pos;
}