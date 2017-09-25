//
// Created by anton on 08.04.17.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../uart/uart.h"
#include "gps.h"


long delayEnableReceiveCounter = COUNTER_DELAY_ENABLE_RECEIVE;
int isEnableReceive = 0;

void enableGps(void) {
    uputs0("AT+GPS=1\r\n");
}

void enableGpsReceiver(void) {
    if (delayEnableReceiveCounter < 0 && isEnableReceive == 0) {
        uputs0("AT+GPSRD=1\r\n");
        isEnableReceive = 1;
    }
    delayEnableReceiveCounter--;
}

void disableGpsReceiver(void) {
    uputs0("AT+GPSRD=0\r\n");
    isEnableReceive = 0;
    delayEnableReceiveCounter = COUNTER_DELAY_ENABLE_RECEIVE;
}

/**
 * Process new GPS position
 *
 * @param message
 */
void processNewGPSPosition(char *message) {

    char str[strlen(message)+5];
    sprintf(str, "GPS::%s\r\n", message);
    //sendDataToServer(str);
    free(str);
}