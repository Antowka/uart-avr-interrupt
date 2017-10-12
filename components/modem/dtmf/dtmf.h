//
// Created by anton on 13.10.17.
//

#ifndef BLINK_DTMF_H
#define BLINK_DTMF_H

int isDtmf(char *message);
int isLastDtmf(char *message);
char * getDtmfCode();
void addSymbolToDtmfCode(const char *message);

#endif //BLINK_DTMF_H
