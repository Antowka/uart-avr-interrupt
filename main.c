#include "components/modem/modem.h"
#include "components/system/easyavr.h"
#include "components/modem/gprs/gprs.h"
#include "components/modem/gps/gps.h"


/**
 * MAIN LOOP
 *
 * @return
 */
int main() {

    initEasyAvr();
    initModem();
    sendDataToAprs();
    enableGpsReciver();

    while (1) {
        modemLoop();
    }

    return 0;
}


