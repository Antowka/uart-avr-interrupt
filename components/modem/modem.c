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
#include "../system/easyavr.h"
#include "dtmf/dtmf.h"

/**
 * Counter for interrupt timer before send aprs
 */
long pingCounter = PING_AT_COUNTER;

/**
 * Flag is changing by TIMER ISR
 */
int timerFinishFlag = 0;


void initTimerIrq(void) {
    TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10); //CPU_SPEED/1024
    TIMSK1 |= (1 << TOIE1);
    TCNT1 = 0;
}

/**
 * Send default config
 */
void sendConfig(void) {

    uputs0("AT\r");
    delay_1ms(800);
    uputs0("ATE0\r");
    delay_1ms(800);
    uputs0("AT+CMGD=1,4\r");
    delay_1ms(800);
    uputs0("AT+CMGF=1\r");
    delay_1ms(800);
    uputs0("AT+CNMI=1,2,0,0,0\r");
    delay_1ms(800);
    uputs0("AT+DDET=1\r");
    delay_1ms(800);
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

    //enableModem();
    _delay_ms(3000);
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
        uputs0("AT\r\n");
    }

    pingCounter--;
}

/**
 * process incoming sms
 */
void smsProcessor(char *message) {

    char *smsCommand = cleanSmsText(message);

    if (strstr(smsCommand, "ping")) {
        sendSms("+79875359969", "pong");
        _delay_ms(1000);
    }
}

/**
 * call from main loop
 */
void modemLoop(void) {

    char buff[RX0_SIZE] = "";
    char *buffLink = (char *) &buff;
    int buffPointer = 0;

    if (timerFinishFlag) {
        STOP_TIMER1;
        timerFinishFlag = 0;
        pingModem();
        START_TIMER1;
    }

    _delay_ms(800);

    if (!ukbhit0()) {
        return;
    }


    do {
        buffLink[buffPointer++] = ugetchar0();
    } while (ukbhit0());

    if (buffPointer) {

        if (isSmsCommand(buffLink)) {
            STOP_TIMER1;
            smsProcessor(buffLink);
            cleanBuffer();
            START_TIMER1;
        } else if (isDtmf(buffLink)) {

            if (isLastDtmf(buffLink)) {
                uputs0(getDtmfCode());
            } else {
                addSymbolToDtmfCode(buffLink);
            }

        } else if (strstr(buffLink, "RING") != NULL) {
            STOP_TIMER1;
            uputs0("ATA\r\n");
            _delay_ms(1000);
            uputs0("AT+VTS=\"1,4,#,A,6,7,0\"\r\n");
            cleanBuffer();
            START_TIMER1;
        } else if (strstr(buffLink, "OK") != NULL) {
            STOP_TIMER1;
            blink();
            pingCounter = PING_AT_COUNTER;
            cleanBuffer();
            START_TIMER1;
        }
    }
}


/**
 * Handler for timer's interrupts
 */
ISR(TIMER1_OVF_vect) {
    timerFinishFlag = 1;
}