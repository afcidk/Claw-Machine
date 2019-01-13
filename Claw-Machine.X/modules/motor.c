#include <xc.h>
#include "motor.h"
#include "XYSensor.h"

#define _XTAL_FREQ 1000000

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

void Int2Config() { // grab button
    TRISBbits.TRISB2 = 1;
    INTCON3bits.INT2IE = 0;
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

void Grab(int stat) {
    // start put down the magnet
    MOTOR_Z_1 = 1;
    MOTOR_Z_2 = 0;
    __delay_ms(5200);
    MOTOR_Z_1 = 0;
    MOTOR_Z_2 = 0;
    // open the magnet
    magnet_on();
    // start get back the magnet
    MOTOR_Z_1 = 0;
    MOTOR_Z_2 = 1;
    __delay_ms(6000);
        // close the magnet
    if (stat == 0)    magnet_off();
    MOTOR_Z_1 = 0;
    MOTOR_Z_2 = 0;
    
    // back to the origin place
    while (PORTBbits.RB5) {
        MOTOR_Y_1 = 0;
        MOTOR_Y_2 = 1;
    }    
    MOTOR_Y_1 = 0;
    MOTOR_Y_2 = 0;
    
    
    while (PORTBbits.RB6) {
        MOTOR_X_1 = 1;
        MOTOR_X_2 = 0;
        
    }
    MOTOR_X_1 = 0;
    MOTOR_X_2 = 0;
    if (stat == 1) magnet_off();
    __delay_ms(1000);
}
