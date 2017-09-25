//
// Created by anton on 24.09.17.
//
#include <stdio.h>
#include <stdlib.h>
#include "../../uart/uart.h"

/**
 * return free ram (FOR DEBUG)
 *
 * @return
 */
char* freeRam() {
    extern int __heap_start, *__brkval;
    int v;
    int t = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);

    static char buf[30];
    sprintf(buf, "FREE MEM: %d \r\n", t);

    return buf;
}

void freeMemPrintToUART() {
    char* freeMemory = freeRam();
    uputs0(freeMemory);
    free(freeMemory);
}
