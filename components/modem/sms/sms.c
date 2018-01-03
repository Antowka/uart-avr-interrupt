//
// Created by anton on 27.03.17.
//

#include <string.h>
#include <stdio.h>
#include "sms.h"
#include "../../uart/uart.h"
#include "../../system/easyavr.h"

static char sms_buff[BUFF_SIZE];
static char *buffPointer = (char *) &sms_buff;

/**
 * Check incoming message for sms command
 *
 * @param message
 * @return
 */
int isSmsCommand(char *message) {
    return strstr(message, "CMD:") != NULL;
}

/**
 * Read clean sms text
 *
 * @param message
 * @return
 */
char* cleanSmsText(char *message) {
    sms_buff[0] = '\0';
    memcpy(buffPointer, &message[(strstr(message, "CMD:")-message+4)], BUFF_SIZE-1);
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
    char smsText[sizeof(text)+1];

    uputs0("AT+CSCS=\"GSM\"\r\n");

    custom_delay_ms(1000);
    uputs0("AT+CMGF=1\r");

    custom_delay_ms(1000);
    sprintf(phoneCommand, "AT+CMGS=\"%s\"\r\n", phone);
    uputs0(phoneCommand);

    custom_delay_ms(1000);
    sprintf(smsText, "%s%c", text, '\032');
    uputs0(smsText);
}

