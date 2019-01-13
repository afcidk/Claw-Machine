#include <xc.h>
#include "TM1637.h"
#define _XTAL_FREQ 1000000
#define CLK PORTCbits.RC6
#define DIO PORTCbits.RC7

const char NUMBER_LIST[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};
// config global iteration variable
int i;

void I2CStart (void) {
    CLK = 1;
    DIO = 1;
    __delay_us(2);
    DIO = 0;
}

void I2Cask (void) {
    CLK = 0;
    __delay_us(5);
    while (DIO);
    CLK = 1;
    __delay_us(2);
    CLK = 0;
}

void I2CStop (void) {
    CLK = 0;
    __delay_us (2);
    DIO = 0;
    __delay_us (2);
    CLK = 1;
    __delay_us (2);
    DIO = 1;
}

void I2CWrByte (unsigned char oneByte) {
    // transfer 8 bit to the tm1637
    for (i = 0 ; i < 8 ; i++)
    {
        CLK = 0;
        if (oneByte & 0x01) { 
            DIO = 1;
        } else {
            DIO = 0;
        }
        __delay_us (3);
        oneByte = oneByte >> 1;
        CLK = 1;
        __delay_us (3);
    }
}

void Display(int number) {
    // declare program address auto plus one mode
    I2CStart();
    I2CWrByte(0x40);
    I2Cask();
    I2CStop();
    // set program start address
    I2CStart();
    I2CWrByte(0xC0);
    I2Cask();
    // transfer 4 number to tm1637
    for(i = 0 ; i < 4 ; i++) {
        I2CWrByte(NUMBER_LIST[number / 1000]); I2Cask();// Send data
        I2CWrByte(NUMBER_LIST[(number % 1000) / 100]); I2Cask();
        I2CWrByte(NUMBER_LIST[(number % 100) / 10]); I2Cask();
        I2CWrByte(NUMBER_LIST[(number % 10)]); I2Cask();
        I2Cask();
    }
    I2CStop();
    // transfer display signal and set maximum brightness
    I2CStart();
    I2CWrByte(0x89);
    I2Cask();
    I2CStop();
}

void I2CConfig() {
    TRISCbits.TRISC6 = TRISCbits.TRISC7 = 0;
    // set button interrupt
    TRISBbits.TRISB1 = 1;
    // set  INT1
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 1;
    INTCON2bits.INTEDG1 = 1;
    // set global interrupt
    RCONbits.IPEN = 1;
    //INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

