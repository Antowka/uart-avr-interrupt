//
// Created by anton on 08.04.17.
//

#ifndef BLINK_GPS_H
#define BLINK_GPS_H

#define COUNTER_NO_BLOCK_DELAY_GPS 30

void enableGpsReciver(void);
void disableGpsReciver(void);
int isGpsCommand(char *message);
void processNewGPSPosition(char *message);

#endif //BLINK_GPS_H
