//
// Created by anton on 31.03.17.
//
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "easyavr.h"
#include "mydefs.h"


void initEasyAvr(void) {
    PIN_AS_OUTPUT(DDRB, LED_PIN);

    PIN_AS_OUTPUT(DDRB, 0);
    PIN_ON(PORTB, 0);
}

/**
 * Blink for debug
 *
 */
void blink(void) {
    PIN_ON(PORTB, LED_PIN);
    custom_delay_ms(100);
    PIN_OFF(PORTB, LED_PIN);
    custom_delay_ms(100);
}

void custom_delay_ms(int delayTimeout) {
    while(delayTimeout--) {
        _delay_ms(1);
        wdt_reset();
    }
}