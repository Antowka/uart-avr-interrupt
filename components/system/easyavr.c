//
// Created by anton on 31.03.17.
//
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "easyavr.h"
#include "mydefs.h"


void initEasyAvr(void) {

    PIN_PULL_UP(DDRB, PORTB, PINB3);
    PIN_PULL_UP(DDRB, PORTB, PINB4);
    PIN_PULL_UP(DDRB, PORTB, PINB5);

    PIN_PULL_UP(DDRD, PORTD, PIND2);
    PIN_PULL_UP(DDRD, PORTD, PIND4);
    PIN_PULL_UP(DDRD, PORTD, PIND5);
    PIN_PULL_UP(DDRD, PORTD, PIND6);
    PIN_PULL_UP(DDRD, PORTD, PIND7);

    PIN_PULL_UP(DDRC, PORTC, PINC0);
    PIN_PULL_UP(DDRC, PORTC, PINC1);
    PIN_PULL_UP(DDRC, PORTC, PINC2);
    PIN_PULL_UP(DDRC, PORTC, PINC3);

    PIN_AS_OUTPUT(DDRB, LED_PIN);

    PIN_AS_OUTPUT(DDRD, RELAY_PIN);
    PIN_ON(PORTD, RELAY_PIN);

    PIN_AS_OUTPUT(DDRB, PWR_KEY_PIN);
    PIN_OFF(PORTB, PWR_KEY_PIN);
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

void wdt_init(void) {
    MCUSR = 0;
    wdt_enable(WDTO_8S);
}