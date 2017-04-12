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
 * Send AT command for check connection and sync speed
 */
void initModem(void) {

    initTimerIrq();
    initUART();

    uputs0("AT\r\n");
    delay_1ms(500);

    uputs0("AT+CMGF=1\r\n");
    delay_1ms(1000);
}

/**
 * Send AT command for check connect modem
 */
void pingModem(void) {
    uputs0("AT\r\n");
    _delay_ms(1000);
}

/**
 * Send APRS DATA
 */
void sendAprs(void) {

    if (aprsCounter <= 0) {
        sendDataToServer("APRS::UC6KFQ>APRS,TCPIP*,qAC,T2GREECE:!5619.09N/04403.24Eyop.Anton (UC6KFQ/3) 145.500Mhz/433.500Mhz");
        aprsCounter = COUNTER_NO_BLOCK_DELAY_APRS;
    }
    aprsCounter--;
}

/**
 * Wait incoming call
 */
void messageProcessor() {
    //check response on AT command
    if (strstr(buffLink, "OK") == NULL || strstr(buffLink, "AT") == NULL) {
        return;
    }
}

/**
 * Check is calling
 *
 * @return
 */
int isRing() {
    return strstr(buffLink, "RING") != NULL;
}


/**
 * process incoming sms
 */
void smsProcessor() {

    char *sms = cleanSmsText(buffLink);

    if (strstr(sms, "ping")) {
        STOP_TIMER1;
        sendSms("+79875359969", "pong");
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

    if(timerAprsCounterFlag) {
        pingModem();
        sendAprs();
        timerAprsCounterFlag = 0;
    }

    if (!ukbhit0()) {
        return;
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
        } else if (isRing()) {
            uputs0("ATA\r\n");
        } else if (isGpsCommand(buffLink)) {
            processNewGPSPosition(buffLink);
        } else {
            messageProcessor();
        }
    }
}

/**
 * Handler for timer's interrupts
 */
ISR(TIMER1_OVF_vect) {
    timerAprsCounterFlag = 1;
}