#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "easyavr.h"
#include "libs/uart.h"

#define LED_PIN 5

char buff[64] = "";
int buffPointer = 0;

/**
 * Send AT command for check connection and sync speed
 */
void initModem(void) {
    uputs0("AT\r\n");
    delay_1ms(1000);
}

/**
 * Send command for enable GPS
 */
void enableGps(void) {
    uputs0("AT+GPS=1\r\n");
    delay_1ms(300);
}


/**
 * Blink for debug
 *
 */
void blink(void) {
    PIN_ON(PORTB, LED_PIN);
    _delay_ms(300);
    PIN_OFF(PORTB, LED_PIN);
    _delay_ms(300);
}

/**
 * Wait incoming call
 */
void messageProcessor() {

    //check on incoming RING
    if (strstr(buff, "RING") != NULL) {
        uputs0("ATA");
        uputchar0('\r');
        uputchar0('\n');
        return;
    }

    //check response on AT command
    if (strstr(buff, "OK") == NULL || strstr(buff, "AT") == NULL) {
        return;
    }
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
    enableGps();

    while (1) {

        if (!ukbhit0()) {
            continue;
        }

        delay_1ms(1000);

        do {
            buff[buffPointer] = ugetchar0();
            buffPointer++;
        } while (ukbhit0());

        if (buffPointer > 1) {
            messageProcessor();
            buff[0] = '\0';
            buffPointer = 0;
        }

        initModem();
        blink();
    }

    return 0;
}


