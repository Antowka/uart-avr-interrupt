//
// Created by anton on 08.04.17.
//

#include <string.h>
#include "../../uart/uart.h"

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