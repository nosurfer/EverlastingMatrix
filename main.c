#include <stdio.h>
#ifdef _WIN32
#include <locale.h>
#endif

#include "game/game.h"

int main() {
    #ifdef _WIN32
    setlocale(LC_ALL, "ru_RU.UTF-8");
    #endif
    game();
    return 0;
}