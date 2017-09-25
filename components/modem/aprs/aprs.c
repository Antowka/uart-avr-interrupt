//
// Created by anton on 24.09.17.
//

#include <stdio.h>
#include <stdlib.h>
#include "../gprs/gprs.h"

void sendAprsPosition(char* latitude, char* longitude, char* message, char* icon) {

    char data[150];
    sprintf(
            data,
            "UC6KFQ>APRS,TCPIP*,qAC,T2GREECE:!%s/0%s%s%s",
            latitude,
            longitude,
            icon,
            message
    );
    sendDataToUdpPort("antowka.ru", 7778, data);
    free(data);
}