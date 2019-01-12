#include <xc.h>
#include "motor.h"

#define MOTOR_X_1 PORTDbits.RD0
#define MOTOR_X_2 PORTDbits.RD1
#define MOTOR_Y_1 PORTDbits.RD2
#define MOTOR_Y_2 PORTDbits.RD3
#define MOTOR_Z_1 PORTDbits.RD4
#define MOTOR_Z_2 PORTDbits.RD5

void MotorConfig() {
    TRISBbits.TRISB3 = 0;  //electric manget
    LATBbits.LATB3 = 0;
    // Tmr0Config();
    Int2Config();
}
/* no need for timer 0
void Tmr0Config() {
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCON2bits.TMR0IP = 1;
    
    TMR0 = 0xc2f6;
}
*/

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
/* no need for GrabStat
int isGrabbing() {
    return GrabStat;
}
*/
void Grab() {
    // start put down the magnet
    MOTOR_Z_1 = 1;
    MOTOR_Z_2 = 0;
    __delay_ms(1000);
    // open the magnet
    magnet_on();
    __delay_ms(1000);
    // close the magnet
    magnet_off();
    // start get back the magnet
    MOTOR_Z_1 = 0;
    MOTOR_Z_2 = 1;
    __delay_ms(1000);
    // back to the origin place
    while (!PORTBbits.RB5) {
        MOTOR_X_1 = 0;
        MOTOR_X_2 = 1;
    }
    while (!PORTBbits.RB6) {
        MOTOR_Y_1 = 0;
        MOTOR_Y_2 = 1;
    }
}
