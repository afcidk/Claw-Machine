#include <xc.h>
#include "TT.h"

void TTConfig(void) {
    _TTPinConfig();
    _CollConfig();
}

void TTturn(void) {
    LATDbits.LATD0 = 1;
    LATDbits.LATD1 = 0;
    
    int cnt = 0;
    while (++cnt < 10000);
    LATDbits.LATD0 = 1;
    LATDbits.LATD1 = 1;
}

void _CollConfig() { // RB3 on change interrupt
    TRISBbits.RB5 = 1;
    
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
    INTCON2bits.RBIP = 1;
}

void _TTPinConfig(void) {
      
    // enable as output
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    TRISDbits.RD3 = 0;
    TRISDbits.RD4 = 0;
    TRISDbits.RD5 = 0;
    TRISDbits.RD6 = 0;
    TRISDbits.RD7 = 0;
    
    // stop all TT motors
    LATDbits.LATD0 = 1;
    LATDbits.LATD1 = 1;
    LATDbits.LATD2 = 1;
    LATDbits.LATD3 = 1;
    LATDbits.LATD4 = 1;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 1;
    LATDbits.LATD7 = 1;
}