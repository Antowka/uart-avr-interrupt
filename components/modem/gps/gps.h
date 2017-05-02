//
// Created by anton on 08.04.17.
//

#ifndef BLINK_GPS_H
#define BLINK_GPS_H

#define COUNTER_DELAY_ENABLE_RECEIVE 10
void enableGps(void);
void disableGps(void);
void enableGpsReciver(void);
void disableGpsReciver(void);
void processNewGPSPosition(char *message);

#endif //BLINK_GPS_H
