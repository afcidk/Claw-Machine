/* 
 * File:   XYSendor.c
 * Author: f26401004
 *
 * Created on January 3, 2019, 8:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "XYSensor.h"

void ADCConfig(void) {
    // open RA0/RA1 as input
    TRISAbits.RA0 = TRISAbits.RA1 = 1;
    // config ref V
    ADCON1bits.VCFG1 = ADCON1bits.VCFG0 = 0;
    // config analog port
    ADCON1bits.PCFG = 13;
    ADCON2bits.ADFM = 1;
    ADCON2bits.ADCS = 0;
    ADCON2bits.ACQT = 2;
    // reset ASRES
    ADRESH = ADRESL = 0;
    // config interrupt
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    // open ADC convertor
    ADCON0bits.ADON = 1;   
}