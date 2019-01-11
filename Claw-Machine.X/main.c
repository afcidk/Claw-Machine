// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include "modules/IR.h"
#include "modules/TT.h"
#include "modules/TM1637.h"
#include "modules/XYSensor.h"
#include "modules/motor.h"

/* occupied pin or interrupt
 * RA0, RA1 -> analog input 
 * RB0 -> infrared
 * RB1 -> micro switch for coin detection
 * RB2 -> grab button
 * RB5 -> electromagnet
 * RB3 -> motor collision detect 1
 * RD0~RD7 -> motor
 * RC6, RC7 -> CLK/DIO for seven-segment display
 * timer0 -> motor delay timer
 */

#define MOTOR_X_1 PORTDbits.RD0
#define MOTOR_X_2 PORTDbits.RD1
#define MOTOR_Y_1 PORTDbits.RD2
#define MOTOR_Y_2 PORTDbits.RD3
#define MOTOR_Z_1 PORTDbits.RD4 // now useless
#define MOTOR_Z_2 PORTDbits.RD5 // now useless

int NUMBER; // for seven-segment display
int SENSOR_X, SENSOR_Y;
int SENSITIVE = 200;

void main(void) {
    IRConfig();
    TTConfig();
    I2CConfig();
    ADCConfig();
    MotorConfig();
    
    // start first adc conversion
    ADCON0bits.GO = 1;
    while (1) {
        // if grab pressed -> grab()
    }
    return;
}


void __interrupt(high_priority) HI_ISR(void) {
    if (INTCON3bits.INT2IF && !isGrabbing()) { // Grab button
        INTCON3bits.INT2IF = 0;
        Nop();
        Grab();
    }
    else if (INTCONbits.RBIF) { // collision interrupt
        INTCONbits.RBIF = 0;
        if (PORTBbits.RB5) {
            Nop();
            Nop();
            Nop();
        }
    }
    else if (INTCONbits.TMR0IF) { // grab motor timer
        INTCONbits.TMR0IF = 0;
        TMR0 = 0xc2f6;
    }
    else if (INTCONbits.INT0IF) { // infra-red control
        INTCONbits.INT0IF = 0;
        Nop();
    }
    else if (INTCON3bits.INT1IF) { // micro switch for seven-segment display
        INTCON3bits.INT1IF = 0;
        NUMBER += 5; //need to divide by 2 (bug???)
        Display(NUMBER);
    } else if (PIR1bits.ADIF && !isGrabbing()) { // XY Sensor ADC convertor interrupt
        PIR1bits.ADIF = 0;
        if (ADCON0bits.CHS) {
            SENSOR_X = (ADRESH * 256) | (ADRESL);
        } else {
            SENSOR_Y = (ADRESH * 256) | (ADRESL);
        }
        ADCON0bits.CHS =  1 - ADCON0bits.CHS;
        ADCON0bits.GO = 1;
         //turn on motor when result is large enough
        if (SENSOR_Y > 1024 - SENSITIVE) {
            MOTOR_Y_1 = 1;
            MOTOR_Y_2 = 0;
        }
        else if (SENSOR_Y < SENSITIVE) {
            MOTOR_Y_1 = 0;
            MOTOR_Y_2 = 1;
        }
        else {
            MOTOR_Y_1 = MOTOR_Y_2 = 0;
        }
        
        if (SENSOR_X > 1024 - SENSITIVE) {
            MOTOR_X_1 = 1;
            MOTOR_X_2 = 0;
        }
        else if (SENSOR_X < SENSITIVE) {
            MOTOR_X_1 = 0;
            MOTOR_X_2 = 1;
        }
        else {
            MOTOR_X_1 = MOTOR_X_2 = 0;
        }
    }

}

void __interrupt(low_priority) LO_ISR(void) {
    
}
