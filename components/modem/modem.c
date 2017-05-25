//
// Functionality for modem
//
#include <util/delay.h>
#include <string.h>
#include "../system/mydefs.h"
#include "../uart/uart.h"
#include "sms/sms.h"
#include "gprs/gprs.h"
#include "modem.h"
#include "gps/gps.h"
#include "../system/easyavr.h"

/**
 * Counter for interrupt timer before send aprs
 */
long aprsCounter = COUNTER_NO_BLOCK_DELAY_APRS;
long pingCounter = PING_AT_COUNTER;

/**
 * Flag is changing by TIMER ISR
 */
int timerAprsCounterFlag = 0;
char buff[RX0_SIZE] = "";
char *buffLink = (char *) &buff;
int buffPointer = 0;

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
    delay_1ms(1000);
    uputs0("AT+CMGF=1\r\n");
    delay_1ms(1000);
    enableGps();
    delay_1ms(1000);
}

void enableModem(void) {
    PIN_ON(PORTB, 1);
    _delay_ms(1000);
    PIN_OFF(PORTB, 1);
}

void disableModem(void) {
    //PIN_ON(PORTD, 6);
    //_delay_ms(10000);
    //PIN_OFF(PORTD, 6);
    uputs0("AT+CPOF\r\n");
}

/**
 * Send AT command for check connection and sync speed
 */
void initModem(void) {

    enableModem();
    _delay_ms(10000);
    initTimerIrq();
    _delay_ms(15000);
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
        sendDataToServer(
                "UC6KFQ>APRS,TCPIP*,qAC,T2GREECE:!5619.09N/04403.24Eyop.Anton (UC6KFQ/3) 145.500Mhz/433.500Mhz");
        aprsCounter = COUNTER_NO_BLOCK_DELAY_APRS;
    }
    aprsCounter--;
}

/**
 * process incoming sms
 */
void smsProcessor() {

    char *sms = cleanSmsText(buffLink);

    if (strstr(sms, "ping")) {
        STOP_TIMER1;
        sendSms("+79875359969", "pong");
        _delay_ms(3000);
        START_TIMER1;
    }
}

/**
 * CLean buffer
 */
void cleanBuff(void) {
    memset(buffLink, 0, sizeof buffLink);
    buffPointer = 0;
}


/**
 * call from main loop
 */
void modemLoop(void) {

    STOP_TIMER1;
    _delay_ms(300);

    if (timerAprsCounterFlag == 1) {
        timerAprsCounterFlag = 0;
        pingModem();
        sendAprs();
        enableGpsReciver();
    }
    START_TIMER1;

    if (!ukbhit0()) {
        return;
    }

    do {
        buffLink[buffPointer] = ugetchar0();
        buffPointer++;
    } while (ukbhit0());


    if (buffPointer > 0) {
        if (isSmsCommand(buffLink)) {
            STOP_TIMER1;
            disableGpsReciver();
            smsProcessor();
            cleanBuffer();
            START_TIMER1;
        } else if (strstr(buffLink, "RING") != NULL) {
            STOP_TIMER1;
            disableGpsReciver();
            uputs0("ATA\r\n");
            cleanBuffer();
            START_TIMER1;
        } else if (strstr(buffLink, "GPSRD:") != NULL) {
            STOP_TIMER1;
            disableGpsReciver();
            processNewGPSPosition(buffLink);
            cleanBuffer();
            START_TIMER1;
        } else if (strstr(buffLink, "OK") != NULL) {
            blink();
            pingCounter = PING_AT_COUNTER;
            cleanBuffer();
        }
        cleanBuff();
    }
}


/**
 * Handler for timer's interrupts
 */
ISR(TIMER1_OVF_vect) {
    timerAprsCounterFlag = 1;
}