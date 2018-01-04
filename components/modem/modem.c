//
// Functionality for modem
//
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
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

void onOffModem(void) {
    PIN_ON(PORTB, PWR_KEY_PIN);
    custom_delay_ms(2500);
    PIN_OFF(PORTB, PWR_KEY_PIN);
}

void enableRelay(void) {
    PIN_OFF(PORTD, RELAY_PIN);
}

void disableRelay(void) {
    PIN_ON(PORTD, RELAY_PIN);
}

int isEnabledRelay(void) {
    return !CHECK_PIN(PIND, RELAY_PIN);
}

int checkDtmfCode(char *code) {
    return strcmp(getDtmfCode(), code) == 0;
}

/**
 * Send default config
 */
void sendConfig(void) {

    onOffModem();
    custom_delay_ms(5000);
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

/**
 * Send AT command for check connection and sync speed
 */
void initModem(void) {

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
    PIN_ON(PORTB, LED_PIN);

    if (strstr(smsCommand, "ON")) {

        enableRelay();
        sendSms(PHONE_NUMBER, isEnabledRelay() ? "OK" : "FAIL");

    } else if (strstr(smsCommand, "OFF")) {

        disableRelay();
        sendSms(PHONE_NUMBER, isEnabledRelay() ? "FAIL" : "OK");

    } else if (strstr(smsCommand, "STATUS")) {

        sendSms(PHONE_NUMBER, isEnabledRelay() ? "ON" : "OFF");
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

    custom_delay_ms(800);

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
                if (checkDtmfCode("01")) {  //ON

                    enableRelay();
                    custom_delay_ms(2000);
                    uputs0("AT+VTS=\"2,2,2\"\r\n");

                } else if (checkDtmfCode("02")) { //OFF

                    disableRelay();
                    custom_delay_ms(2000);
                    uputs0("AT+VTS=\"2,2,2\"\r\n");

                } else if (checkDtmfCode("03")) { //STATUS

                    if (isEnabledRelay()) {
                        custom_delay_ms(2000);
                        uputs0("AT+VTS=\"2\"\r\n");
                    } else {
                        custom_delay_ms(2000);
                        uputs0("AT+VTS=\"2,2\"\r\n");
                    }
                }

            } else {
                addSymbolToDtmfCode(buffLink);
            }

        } else if (strstr(buffLink, "RING") != NULL) {
            STOP_TIMER1;
            uputs0("ATA\r\n");
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