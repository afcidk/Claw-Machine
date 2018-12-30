#include <xc.h>
#include "TT.h"

void TTConfig(void) {
    _TTPinConfig();
}

void TTturn(void) {
    LATDbits.LATD0 = 1;
    LATDbits.LATD1 = 0;
    
    int cnt = 0;
    while (++cnt < 10000);
    LATDbits.LATD0 = 1;
    LATDbits.LATD1 = 1;
}

void _TTPinConfig(void) {
      
    // enable as output
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    TRISDbits.RD3 = 0;
    
    // stop all TT motors
    LATDbits.LATD0 = 1;
    LATDbits.LATD1 = 1;
    LATDbits.LATD2 = 1;
    LATDbits.LATD3 = 1;
}