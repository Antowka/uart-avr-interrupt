#include <string.h>

//
// Created by anton on 13.10.17.
// *[1-9]# - format
//
static char dtmfCode[2]; //TODO: вынести 3-ку в константы
int indexLastSymbolInCode = 0;

int isDtmf(char *message) {

    int isDtmf = strstr(message, "DTMF:") != NULL;

    if (isDtmf && strstr(message, "DTMF: *") != NULL) {
        memset(dtmfCode, 0, strlen(dtmfCode)); //clean array
    }

    return isDtmf;
}

int isLastDtmf(char *message) {
    return strstr(message, "+DTMF: #") != NULL;
}

char * getDtmfCode() {
    return dtmfCode;
}

void addSymbolToDtmfCode(const char *message) {

    if (message[9] == '*') {
        return;
    }

    dtmfCode[indexLastSymbolInCode] = message[9];

    indexLastSymbolInCode++;
    if (indexLastSymbolInCode >= 2) {
        indexLastSymbolInCode = 0;
    }
}


