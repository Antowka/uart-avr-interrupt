//
// Functionality for modem
//
#include <util/delay.h>
#include <string.h>
#include "../system/mydefs.h"
#include "../uart/uart.h"
#include "../system/easyavr.h"
#include "sms/sms.h"
#include "gprs/gprs.h"


long aprsCounter = COUNTER_NO_BLOCK_DELAY_APRS;
char buff[RX0_SIZE] = "";
char *buffLink = (char *) &buff;
int buffPointer = 0;

void initTimerIrq(void) {
    cli(); //Disable all Interrupts
    TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10); //CPU_SPEED/1024
    TIMSK1 |= (1 << TOIE1);
    TCNT1 = 0;
    sei(); //Enable all Interrupts
}

void enableTimerIrq(void) {
    TIMSK1 |= (1 << TOIE1);
}

void disableTimerIrq(void) {
    TIMSK1 |= (0 << TOIE1);
}

/**
 * Send AT command for check connection and sync speed
 */
void initModem(void) {

    initTimerIrq();

    initUART();

    uputs0("AT\r\n");
    delay_1ms(500);

    uputs0("AT+GPS=1\r\n");
    delay_1ms(1000);

    uputs0("AT+CMGF=1\r\n");
    delay_1ms(1000);

    initGPRS();
}

/**
 * Send AT command for check connect modem
 */
void pingModem(void) {
    uputs0("AT\r\n");
    _delay_ms(500);
}

/**
 * Send APRS DATA
 */
void sendAprs(void) {

    if (aprsCounter <= 0) {
        sendDataToAprs();
        aprsCounter = COUNTER_NO_BLOCK_DELAY_APRS;
    }
    aprsCounter--;
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
    char *sms = cleanSmsText(buffLink);

    if (strstr(sms, "ping")) {
        sendSms("+79875359969", "pong");
    }

    return;
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
        } else {
            messageProcessor();
        }
    }
}

/**
 * Handler for timer's interrupts
 */
ISR(TIMER1_OVF_vect) {
    pingModem();
    sendAprs();
    blink();
}