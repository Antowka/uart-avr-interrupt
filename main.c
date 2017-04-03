#include "components/modem/modem.h"
#include "components/system/easyavr.h"
#include "components/modem/gprs/gprs.h"


/**
 * MAIN LOOP
 *
 * @return
 */
int main() {

    initEasyAvr();
    initModem();
    sendDataToAprs();

    while (1) {
        modemLoop();
    }

    return 0;
}


