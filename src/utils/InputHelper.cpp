#include "../common/InputHelper.h"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

// Works for Linux :
char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);            // get current terminal attributes
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);          // disable buffering and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);   // apply new settings
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);   // restore old settings
    return ch;
}
