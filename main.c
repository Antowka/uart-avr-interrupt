#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "easyavr.h"
#include "libs/uart.h"
#include "libs/sms.h"
#include "libs/debug.h"

#define LED_PIN 5

char buff[256] = "";
char *buffLink = (char*) &buff;
int buffPointer = 0;

/**
 * Send AT command for check connection and sync speed
 */
void initModem(void) {
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
    _delay_ms(1000);
    PIN_OFF(PORTB, LED_PIN);
    _delay_ms(1000);
}

void pingModem(void) {
    uputs0("AT\r\n");
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
 * process incoming sms
 */
void smsProcessor() {
    char* sms = cleanSmsText(buffLink);
    blink();

    if (strstr(sms, "ping")) {
        uputs0("AT+CMGF=1\r\n");
        _delay_ms(1000);
        uputs0("AT+CSCS=UCS2\r\n");
        _delay_ms(1000);
        uputs0("AT+CMGS=+79875359969\r\n");
        _delay_ms(1000);
        uputs0("pong\032");
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

    PIN_AS_OUTPUT(DDRB, LED_PIN);
    initUART();
    initModem();

    while (1) {

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

            if(isSmsCommand(buffLink)) {
                smsProcessor();
            } else {
                messageProcessor();
            }
        }
        //pingModem();
    }

    return 0;
}


