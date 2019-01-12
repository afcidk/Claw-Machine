#include <xc.h>
#include "motor.h"
int GrabStat = 0;

void MotorConfig() {
    TRISBbits.TRISB3 = 0;  //electric manget
    LATBbits.LATB3 = 0;
    Tmr0Config();
    Int2Config();
}

void Tmr0Config() {
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCON2bits.TMR0IP = 1;
    
    TMR0 = 0xc2f6;
}

void Int2Config() { // grab button
    TRISBbits.TRISB2 = 1;
    INTCON3bits.INT2IE = 1;
    INTCON3bits.INT2IF = 0;
    INTCON3bits.INT2IP = 1;
    INTCON2bits.INTEDG2 = 1; // on rising edge
}

void magnet_on() {
    LATBbits.LATB3 = 1;
}

void magnet_off() {
    LATBbits.LATB3 = 0;
}

int isGrabbing() {
    return GrabStat;
}
void Grab() {
    GrabStat = 1;
    T0CON = 0b10000011;
    
    magnet_on();
    // let magnetic go down
    
    
    // let magnetic go up
    
    
    // go to destination, turn off magnet
    magnet_off();
    
    
    // go to initial place
    
    // restart adc conversion
    T0CONbits.TMR0ON = 0;
    ADCON0bits.GO = 1;
    
    GrabStat = 0;
}
