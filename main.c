#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "easyavr.h"
#include "libs/uart.h"
#include "libs/sms.h"

#define LED_PIN 5
long counter = COUNTER_NO_BLOCK_DELAY;
char buff[RX0_SIZE] = "";
char *buffLink = (char *) &buff;
int buffPointer = 0;

/**
 * Send AT command for check connection and sync speed
 */
void initModem(void) {

    PIN_AS_OUTPUT(DDRB, LED_PIN);

    uputs0("AT\r\n");
    delay_1ms(500);

    uputs0("AT+GPS=1\r\n");
    delay_1ms(500);

    uputs0("AT+CMGF=1\r\n");
    delay_1ms(500);
}

/**
 * Blink for debug
 *
 */
void blink(void) {
    PIN_ON(PORTB, LED_PIN);
    _delay_ms(100);
    PIN_OFF(PORTB, LED_PIN);
    _delay_ms(100);
}

/**
 * Send AT command for check connect modem
 */
void pingModem(void) {

    if (counter <= 0) {

        for (int i = 2; i > 0; i--) {
            uputs0("AT\r\n");
            _delay_ms(300);
        }

        blink();
        counter = COUNTER_NO_BLOCK_DELAY;
    }
    counter--;
}


/**
 * Wait incoming call
 */
void messageProcessor() {

    //check on incoming RING
    if (strstr(buffLink, "RING") != NULL) {
        uputs0("ATA\r\n");
        return;
    }

    //check response on AT command
    if (strstr(buffLink, "OK") == NULL || strstr(buffLink, "AT") == NULL) {
        return;
    }
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
    sprintf(phoneCommand, "AT+CMGS=%s\r\n", phone);
    sprintf(smsText, "%s\032", text);

    uputs0("AT+CMGF=1\r\n");
    _delay_ms(1000);
    uputs0("AT+CSCS=UCS2\r\n");
    _delay_ms(1000);
    uputs0(phoneCommand);
    _delay_ms(1000);
    uputs0(smsText);
}

/**
 * process incoming sms
 */
void smsProcessor() {
    char *sms = cleanSmsText(buffLink);

    if (strstr(sms, "ping")) {
        sendSms("+79875359969", "pong");
    }

    return;
}

void cleanBuff(void) {
    memset(buffLink, 0, sizeof buffLink);
    buffPointer = 0;
}

/**
 * MAIN LOOP
 *
 * @return
 */
int main() {

    initUART();
    initModem();

    while (1) {

        pingModem();

        if (!ukbhit0()) {
            continue;
        }

        delay_1ms(300);

        cleanBuff();

        do {
            buffLink[buffPointer] = ugetchar0();
            buffPointer++;
        } while (ukbhit0());

        cleanBuffer();

        if (buffPointer > 1) {

            if (isSmsCommand(buffLink)) {
                smsProcessor();
            } else {
                messageProcessor();
            }
        }
    }

    return 0;
}


