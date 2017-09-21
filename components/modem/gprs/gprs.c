//
// GPRS module
//
#include <stdio.h>
#include <string.h>
#include "../../uart/uart.h"


void sendDataToServer(char *message){

    uputs0("AT+CGATT=1\r\n");
    _delay_ms(3000);

    uputs0("AT+CGDCONT=1,\"IP\",\"internet.mts.ru\"\r\n");  //setting PDP parameter
    _delay_ms(3000);

    uputs0("AT+CGACT=1,1\r\n");                             //Activate PDP, open Internet service
    _delay_ms(3000);

    uputs0("AT+CIFSR\r\n");
    _delay_ms(1000);

    uputs0("AT+CIPSTART=\"UDP\",\"antowka.ru\",7778\r\n"); //Establish TCP connection
    _delay_ms(3000);
    char str[16];
    sprintf(str, "AT+CIPSEND=%d\r\n", strlen(message));
    uputs0(str);
    _delay_ms(1000);

    char msg[127];
    sprintf(msg, "%s\r\n", message);
    uputs0(msg);
    _delay_ms(1000);

    uputs0("AT+CIPCLOSE\r\n");                              //Close TCP
    _delay_ms(2000);
}