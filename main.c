#include "components/modem/modem.h"
#include "components/system/easyavr.h"


/**
 * MAIN LOOP
 *
 * @return
 */
int main() {

    initEasyAvr();
    initModem();

    while (1) {
        modemLoop();
    }

    return 0;
}


