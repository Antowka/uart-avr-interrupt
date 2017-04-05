//
// Created by anton on 31.03.17.
//

#ifndef BLINK_MODEM_H
#define BLINK_MODEM_H

void initModem(void);
void modemLoop(void);
void enableTimerIrq(void);
void disableTimerIrq(void);

#endif //BLINK_MODEM_H
