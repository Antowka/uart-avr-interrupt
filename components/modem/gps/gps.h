//
// Created by anton on 08.04.17.
//

#ifndef BLINK_GPS_H
#define BLINK_GPS_H

#define COUNTER_NO_BLOCK_DELAY_GPS 30
void enableGps(void);
void disableGps(void);
void enableGpsReciver(void);
void disableGpsReciver(void);
void processNewGPSPosition(char *message);

#endif //BLINK_GPS_H
