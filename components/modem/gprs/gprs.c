//
// GPRS module
//

#include "../../uart/uart.h"

void initGPRS(void) {
    uputs0("AT+CREG?\r\n");                                 //Query network registration
    _delay_ms(2000);

    uputs0("AT+CGATT=1\r\n");
    _delay_ms(5000);

    uputs0("AT+CGDCONT=1,\"IP\",\"internet.mts.ru\"\r\n");  //setting PDP parameter
    _delay_ms(5000);

    uputs0("AT+CGACT=1,1\r\n");                             //Activate PDP, open Internet service
    _delay_ms(5000);
}

void sendDataToAprs(void){

    uputs0("AT+CIFSR\r\n");
    _delay_ms(1000);

    uputs0("AT+CIPSTART=\"TCP\",\"russia.aprs2.net\",14580\r\n"); //Establish TCP connection
    _delay_ms(10000);

    uputs0("AT+CIPSEND=22");
    uputs0("\r\n");
    _delay_ms(5000);
    uputs0("user UC6KFQ pass 22203");
    uputs0("\r\n");
    _delay_ms(6000);

    uputs0("AT+CIPSEND=93");
    uputs0("\r\n");
    _delay_ms(5000);
    uputs0("UC6KFQ>APRS,TCPIP*,qAC,T2GREECE:!5619.09N/04403.24Eyop.Anton (UC6KFQ/3) 145.500Mhz/433.500Mhz");
    uputs0("\r\n");
    _delay_ms(6000);

    uputs0("AT+CIPCLOSE\r\n");                              //Close TCP
    _delay_ms(2000);
}