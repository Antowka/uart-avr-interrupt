//
// Created by anton on 08.04.17.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../uart/uart.h"
#include "../gprs/gprs.h"
#include "gps.h"
#include "../../system/easyavr.h"

long gpsPockerCounter = COUNTER_NO_BLOCK_DELAY_GPS;

void enableGpsReciver(void) {
    uputs0("AT+GPS=1\r\n");
    delay_1ms(1000);
    uputs0("AT+GPSRD=1\r\n");
    delay_1ms(1000);
}

void disableGpsReciver(void) {
    uputs0("AT+GPSRD=0\r\n");
    delay_1ms(1000);
    uputs0("AT+GPS=0\r\n");
    delay_1ms(1000);
}

/**
 * Check incoming message for gps position
 *
 * @param message
 * @return
 */
int isGpsCommand(char *message) {
    return strstr(message, "GPSRD:") != NULL;
}

/**
 * Process new GPS position
 *
 * @param message
 */
void processNewGPSPosition(char *message) {

    if (gpsPockerCounter <= 0) {
        char str[strlen(message)+5];
        sprintf(str, "GPS::%s\r\n", message);
        sendDataToServer(str);
        gpsPockerCounter = COUNTER_NO_BLOCK_DELAY_GPS;
        blink();
        free(str);
    }
    gpsPockerCounter--;
}