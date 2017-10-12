//
// Created by anton on 27.03.17.
//

#include <string.h>
#include <stdio.h>
#include "sms.h"
#include "../../uart/uart.h"

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

/**
 * Send SMS
 *
 * @param phone
 * @param text
 */
void sendSms(char *phone, char *text) {

    char phoneCommand[25];
    char smsText[64];

    uputs0("AT+CSCS=\"GSM\"\r");

    _delay_ms(1000);
    uputs0("AT+CMGF=1\r");

    _delay_ms(1000);
    sprintf(phoneCommand, "AT+CMGS=\"%s\"\r", phone);
    uputs0(phoneCommand);

    _delay_ms(1000);
    sprintf(smsText, "%s", text);
    uputs0(text);

    _delay_ms(800);
    uputs0("\x1A");
}

