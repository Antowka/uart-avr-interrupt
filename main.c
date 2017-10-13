#include <avr/io.h>
#include <avr/wdt.h>
#include "components/modem/modem.h"
#include "components/system/easyavr.h"


/**
 * MAIN LOOP
 *
 * @return
 */
int main() {

    initEasyAvr();
    wdt_enable(WDTO_8S);
    initModem();

    while (1) {
        modemLoop();
        wdt_reset();
    }
}


