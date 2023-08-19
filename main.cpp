/****
 * 
 * CREATED BY : JEET SHAH
 * DESC : FILE EXPLORER
 * 
 * *****/


#include "libList.h"
#include "normalmode.h"
#include "keyPressTriggers.h"
#include "commandmode.h"

void die(const char *s) {
  perror(s);
  exit(1);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}


int main(int argc, char **argv){

    // to detect when the window get's resized resetWindowSize is triggered
    signal(SIGWINCH,resetWindowSize);

    enableRawMode();
    switchToNormalMode();
    disableRawMode();
    return 0;

}