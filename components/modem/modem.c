//
// Functionality for modem
//
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "../system/mydefs.h"
#include "../uart/uart.h"
#include "sms/sms.h"
#include "modem.h"
#include "gps/gps.h"
#include "../system/easyavr.h"
#include "aprs/aprs.h"

/**
 * Counter for interrupt timer before send aprs
 */
int aprsCounter = COUNTER_NO_BLOCK_DELAY_APRS;
long pingCounter = PING_AT_COUNTER;

/**
 * Flag is changing by TIMER ISR
 */
int timerAprsCounterFlag = 0;


void initTimerIrq(void) {
    TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10); //CPU_SPEED/1024
    TIMSK1 |= (1 << TOIE1);
    TCNT1 = 0;
}

/**
 * Send default config
 */
void sendConfig(void) {

    uputs0("ATE0\r\n");
    delay_1ms(800);
    uputs0("AT+CMGF=1\r\n");
    delay_1ms(800);
    enableGps();
    delay_1ms(800);
    disableGpsReceiver();
}

void enableModem(void) {
    PIN_ON(PORTB, 1);
    _delay_ms(1000);
    PIN_OFF(PORTB, 1);
}

/**
 * Send AT command for check connection and sync speed
 */
void initModem(void) {

    enableModem();
    _delay_ms(10000);
    initTimerIrq();
    initUART();
    sendConfig();
}

/**
 * Send AT command for check connect modem
 */
void pingModem(void) {

    if (pingCounter <= 0) {
        sendConfig();
        pingCounter = PING_AT_COUNTER;
    } else if (pingCounter < (PING_AT_COUNTER / 2)) {
        _delay_ms(500);
        uputs0("AT\r\n");
    }

    pingCounter--;
}

/**
 * Send APRS DATA
 */
void sendAprs(void) {
    if (aprsCounter <= 0) {
        sendAprsPosition("5619.08N", "4403.27E", "op.Anton", "y");
        aprsCounter = COUNTER_NO_BLOCK_DELAY_APRS;
    }
    aprsCounter--;
}

/**
 * process incoming sms
 */
void smsProcessor(char *message) {

    char *smsCommand = cleanSmsText(message);

    if (strstr(smsCommand, "ping")) {
        STOP_TIMER1;
        sendSms("+79875359969", "pong");
        _delay_ms(3000);
        START_TIMER1;
    }
}

/**
 * call from main loop
 */
void modemLoop(void) {

    char buff[RX0_SIZE] = "";
    char *buffLink = (char *) &buff;
    int buffPointer = 0;

    STOP_TIMER1;
    _delay_ms(500);
    if (timerAprsCounterFlag) {
        timerAprsCounterFlag = 0;
        pingModem();
        //sendAprs();
        //enableGpsReceiver();
    }
    _delay_ms(500);
    START_TIMER1;

    if (!ukbhit0()) {
        return;
    }


    do {
        buffLink[buffPointer] = ugetchar0();
        buffPointer++;
    } while (ukbhit0());

    if (buffPointer) {

        if (isSmsCommand(buffLink)) {
            STOP_TIMER1;
            disableGpsReceiver();
            smsProcessor(buffLink);
            cleanBuffer();
            START_TIMER1;
        } else if (strstr(buffLink, "RING") != NULL) {
            STOP_TIMER1;
            disableGpsReceiver();
            uputs0("ATA\r\n");
            cleanBuffer();
            START_TIMER1;
        } else if (strstr(buffLink, "GPGGA")  != NULL) {
            STOP_TIMER1;
            disableGpsReceiver();
            processNewGPSPosition(buffLink);
            cleanBuffer();
            START_TIMER1;
        } else if (strstr(buffLink, "OK") != NULL) {
            blink();
            pingCounter = PING_AT_COUNTER;
            cleanBuffer();
        }
    }
}


/**
 * Handler for timer's interrupts
 */
ISR(TIMER1_OVF_vect) {
    timerAprsCounterFlag = 1;
}