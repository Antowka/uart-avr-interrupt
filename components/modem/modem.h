//
// Created by anton on 31.03.17.
//

#ifndef BLINK_MODEM_H
#define BLINK_MODEM_H

#define START_TIMER1 TIMSK1 |= (1 << TOIE1);
#define STOP_TIMER1  TIMSK1 &= ~(1 << TOIE1)
#define CLEAR_TIMER1 TCNT1 = 0

void initModem(void);
void modemLoop(void);
void sendAprs(void);

#endif //BLINK_MODEM_H
