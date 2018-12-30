
#include <xc.h>
#include "IR.h"

void IRConfig(void) {
    /*
     * Used ports: RB0
     * Used interrupts: INT0
     * ADCON1 is modified!
     */
    _IRPinConfig();
    _IRIntConfig();
}

void _IRPinConfig(void) {    
    PORTB = 0;
    
    // portb pull-up
    INTCON2bits.RBPU = 0;
    
    // set RB0 to digital input
    ADCON1 = 0b00000011; // ONLY RB0 is digital input!
    TRISBbits.RB0 = 1;
}

void _IRIntConfig(void) {
    // global interrupt
    INTCONbits.GIE = 1;
    RCONbits.IPEN = 1;
    
    // config bits for INT0(RB0)
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 1; // interrupt on rising edge
    INTCON2bits.RBIP = 1; // high priority interrupt on portB

}