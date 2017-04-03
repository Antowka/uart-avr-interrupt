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


long counter = COUNTER_NO_BLOCK_DELAY;
long aprsCounter = COUNTER_NO_BLOCK_DELAY_APRS;
char buff[RX0_SIZE] = "";
char *buffLink = (char *) &buff;
int buffPointer = 0;

/**
 * Send AT command for check connection and sync speed
 */
void initModem(void) {

    initUART();

    uputs0("AT\r\n");
    delay_1ms(500);

    uputs0("AT+GPS=1\r\n");
    delay_1ms(500);

    uputs0("AT+CMGF=1\r\n");
    delay_1ms(500);

    initGPRS();
}

/**
 * Send AT command for check connect modem
 */
void pingModem(void) {

    if (counter <= 0) {

        for (int i = 2; i > 0; i--) {
            uputs0("AT\r\n");
            _delay_ms(300);
        }

        blink();
        counter = COUNTER_NO_BLOCK_DELAY;
    }
    counter--;
}

/**
 * Send APRS DATA
 */
void sendAprs(void) {

    if (aprsCounter <= 0) {

        sendDataToAprs();
        blink();
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

    pingModem();
    sendAprs();

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