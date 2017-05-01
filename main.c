
#include "components/modem/modem.h"
#include "components/system/easyavr.h"
#include "components/modem/gps/gps.h"


/**
 * MAIN LOOP
 *
 * @return
 */
int main() {

    initEasyAvr();
    initModem();

    enableGps();
    disableGpsReciver();//enableGpsReciver();

    while (1) {
        modemLoop();
    }

    return 0;
}


