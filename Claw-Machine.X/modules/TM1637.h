#ifndef TM1637_H
#define	TM1637_H

#include <xc.h> 

void I2CConfig(void);
void I2CStart (void);
void I2Cask (void);
void I2CStop (void);
void I2CWrByte (unsigned char oneByte);

void Display(int number);

#endif

