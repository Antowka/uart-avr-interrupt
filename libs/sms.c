//
// Created by anton on 27.03.17.
//

#include <string.h>
#include "sms.h"

static char sms_buff[BUFF_SIZE];
static char *buffPointer = (char *) &sms_buff;

/**
 * Check incoming message for sms command
 *
 * @param message
 * @return
 */
int isSmsCommand(char *message) {
    return strstr(message, "SMS:") != NULL;
}

/**
 * Read clean sms text
 *
 * @param message
 * @return
 */
char* cleanSmsText(char *message) {
    sms_buff[0] = '\0';
    memcpy(buffPointer, &message[(strstr(message, "SMS:")-message+4)], BUFF_SIZE-1);
    buffPointer[BUFF_SIZE-1] = '\0';
    return sms_buff;
}


