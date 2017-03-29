//
// Created by anton on 27.03.17.
//

#ifndef BLINK_SMS_H
#define BLINK_SMS_H

#define BUFF_SIZE 64

int isSmsCommand(char *message);
char* cleanSmsText(char *message);

#endif //BLINK_SMS_H
