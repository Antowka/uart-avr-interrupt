#include "../uart/uart.h"


void printString(char* message) {
    uputs0("\r\nSTART\r\n");
    uputs0(message);
    uputs0("\r\nEND\r\n");
}
