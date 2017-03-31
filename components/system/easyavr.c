//
// Created by anton on 31.03.17.
//
#include <avr/io.h>
#include <util/delay.h>
#include "easyavr.h"
#include "mydefs.h"


void initEasyAvr(void) {
    PIN_AS_OUTPUT(DDRB, LED_PIN);
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
