//
// GPRS module
//
#include <stdio.h>
#include <string.h>
#include "../../uart/uart.h"
#include "gprs.h"


void sendDataToUdpPort(char *host, int port, char *data){

    while (!utx0_ready() && ukbhit0()) continue;
    uputs0("AT+CGATT=1\r\n");
    _delay_ms(3000);

    while (!utx0_ready() && ukbhit0()) continue;
    char strAPN[55];
    sprintf(strAPN, "AT+CGDCONT=1,\"IP\",\"%s\"\r\n\0", APN_ADDRESS);
    uputs0(strAPN);  //setting PDP parameter
    _delay_ms(3000);

    while (!utx0_ready() && ukbhit0()) continue;
    uputs0("AT+CGACT=1,1\r\n");  //Activate PDP, open Internet service
    _delay_ms(3000);

    while (!utx0_ready() && ukbhit0()) continue;
    uputs0("AT+CIFSR\r\n");
    _delay_ms(1000);

    while (!utx0_ready() && ukbhit0()) continue;
    char strHostAndPort[55];
    sprintf(strHostAndPort, "AT+CIPSTART=\"UDP\",\"%s\",%d\r\n\0", host, port);
    uputs0(strHostAndPort); //Establish TCP connection
    _delay_ms(3000);

    while (!utx0_ready() && ukbhit0()) continue;
    char strDataLength[20];
    int sizeData = strlen(data) + 5;
    sprintf(strDataLength, "AT+CIPSEND=%d\r\n\0", sizeData);
    uputs0(strDataLength);
    _delay_ms(1000);

    while (!utx0_ready() && ukbhit0()) continue;
    char strData[sizeData];
    sprintf(strData, "%s\r\n\0", data);
    uputs0(strData);
    _delay_ms(7000);

    while (!utx0_ready() && ukbhit0()) continue;
    uputs0("AT+CIPCLOSE\r\n");                              //Close TCP
    _delay_ms(2000);
}