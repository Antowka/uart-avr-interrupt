//
// Created by anton on 31.03.17.
//

#ifndef BLINK_MODEM_H
#define BLINK_MODEM_H

#define START_TIMER1 TIMSK1 |= (1 << TOIE1);
#define STOP_TIMER1  TIMSK1 &= ~(1 << TOIE1)

void initModem(void);
void modemLoop(void);

#endif //BLINK_MODEM_H
