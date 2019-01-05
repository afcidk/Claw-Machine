#include <xc.h>

// CONFIG1H
#pragma config OSC = INTIO67      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1        // Watchdog Timer Postscale Select bits (1:1)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
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




#define _XTAL_FREQ      4000000

// TONES  ==========================================

#define  C4     261
#define  D4     293  
#define  E4     329  
#define  F4     349 
#define  F4H    370
#define  G4     392  
#define  A4     440
#define  B4L    466
#define  B4     493
#define  C5     523  
#define  D5     587 
#define  E5     659 
#define  F5     698 
#define  G5     784 
#define  A5     880
#define  B5L    932
#define  B5     987
#define  C6     1046
// Define a special note, 'R', to represent a rest
#define  R     0
// ==================================================
#define TMR2PRESCALE 16
#define TMR0PRESCALE 256

int led_cycle = 0;
int led2_cycle = 0;
int tmr3_cycle = 1;
int tmr1_cycle = 1;
int MELODY_LENGTH;
int MELODY_PTR;
int MELODY_LENGTH2;
int MELODY_PTR2;

int current_tone = 0; // current playing note
int current_tone_duration = 0; // current note duration
int isRunning = 0;
int *wm_1;
char *wb_1;
int *wm_2;
char *wb_2;

const int melody4[] = {
    C4,C4,G5,G5,A5,A5,G5,F4,F4,E4,E4,D4,D4,C4
};
const char beats4[] = {
    8,8,8,8,8,8,4,8,8,8,8,8,8,4
};

const int melody3[] = {
    D5,D5,E5,C5,C5,E5, B4,R,E5,R,A4,R,E5,R, D5,D5,E5,C5,C5,E5, B4,R,E5,R,A4,R,    
};

const char beats3[] = {
    4,8,8,4,8,8, 8,8,8,8,8,8,8,8, 4,8,8,4,8,8, 8,8,8,8,4,4
};

const int melody2[] = {C5,B4L, A4,F5,F5,C5,A4, B4L,F5,F5,F5,G5, F5,C5,C5,C5,B4L,B4L,B4L,A4,B4L,B4L,C5,C5,
                    C5,C5,C5,C5,B4L, A4,F5,F5,C5,A4, B4L,G5,G5,A5,B5L, C6,F5,F5,D5,C5,F5,E5,F5,F5, F5,R,C5,A5,A5,G5,
                    R,D5,F5,D5,A5,D5,F5,D5, R,D5,F5,D5,A5,G5,F5,G5, R,F5,G5,F5,G5,F5,G5,F5,G5,F5,G5,F5,G5,A5,
                    B5L,C6,A5,G5,G5,A5,F5,D5, R,D5,F5,D5,A5,D5,F5,D5, R,D5,F5,D5,A5,G5,F5,G5, 
                    G5,A5, B5L,B5L,R,D5,A5,G5,A5, A5,A5,G5,A5,B5L,A5,B5L, A5,G5,F5,E5,C5,C5,A5,G5,A5,
                    A5,C6,B5L,A5,G5,F5, G5,A5,B5L,C6,A5,G5, F5,F5,F5,F5,C5,G5,F5,C5,C5, A5,A5,A5,A5,A5,F5,B5L,A5,F5,C6,
                    C6,C6,B5L,A5,F5,F5,C5,F5, G5,G5,A5,A5,G5,B5L,C5,A5,G5,A5, A5,A5,G5,A5,B5L,A5,B5L, A5,G5,F5,E5,C5,C5,A5,G5,A5,
                    A5,C6,B5L,A5,G5,F5, G5,A5,B5L,C6,A5,G5, F5,F5,F5,F5,C5,G5,F5,C5,C5,
                    A5,A5,A5,A5,A5,F5,B5L,A5,F5,C6, C6,C6,B5L,A5,F5,F5,C5,F5, G5,G5,A5,A5,G5,G5,B5L,A5,C5,F5, F5,
                    R
                    };
const char beats2[] = {16,16, 4,2,8,16,16, 4,2,8,16,16, 8,16,8,16,16,8,16,16,8,16,16,16,
                2,4,8,16,16, 4,2,8,16,16, 4,2,8,16,16, 8,16,6,16,6,16,6,16,16, 6,8,16,16,16,2,
                4,16,8,16,8,16,6,8, 4,16,8,16,8,16,6,8, 16,16,16,16,16,16,16,16,8,16,8,16,16,16,
                8,8,8,8,4,8,16,16, 4,16,8,16,8,16,6,8, 4,16,8,16,8,16,6,8,
                2,2, 2,8,16,16,8,16,16, 4,8,16,6,4,16,16, 8,16,8,8,4,16,8,16,16,
                4,4,8,16,8,6, 8,16,8,8,16,2, 8,16,16,8,16,8,8,4,16, 16,16,16,16,8,16,8,8,6,8,
                4,8,16,16,4,8,16,16, 16,16,8,8,16,4,16,8,16,16, 4,8,16,6,4,16,16, 8,16,8,8,4,16,8,16,16,
                4,4,8,16,8,6, 8,16,8,8,16,2, 8,16,16,8,16,8,8,4,16,
                16,16,16,16,8,16,8,8,6,8, 4,8,16,16,4,8,16,16, 16,16,16,8,8,16,8,8,8,8, 1,
                1
                };

const int melody[] = {R,R, F4, F4, F4, G4, 
                       F4,A4,A4,G4,F4, F4,G4,R, G4,A4, B4L,C5,
                       D4,D4,B4L,B4L,B4L,B4L,B4L,B4L,B4L,B4L,B4L,B4L,B4L,B4L,
                       D4,D4,A4,A4,A4,A4,A4,A4,A4,A4,A4,A4,A4,A4,
                       C5,R,G4,F4,E4, R,C5,D5,C5,A4, R,A4,C5,A5,C5,A5,C5,A5, R,A4,C5,A5,C5,C5,A4,G4,
                       G4,A4, B4L,B4L,R,D4,F4,G4,F4, F4,F4,G4,F4,F4,F4,B4L, F4,E4,D4,C4,C4,C4,F4,
                       F4,F4,F4,A4,D4,D4, D4,A4,G4,G4,A4,C4, C4,C4,C4,C4,C4,C4,C4,C4,C4, F4,F4,F4,F4,F4,F4,C4,C4,C4,C5,
                       F4,F4,D4,D4,C4,C4,C4,F4, C4,C4,C4,C4,C4,C4,C4,A4,G4,F4, F4,F4,E4,F4,F4,A4,B4L, F4,E4,C4,E4,C4,C4,C4,G4,F4,
                       F4,F4,F4,F4,D4,D4, D4,A4,G4,G4,A4,E4, C4,C4,C4,C4,C4,C4,C4,C4,C4, 
                       C4,C4,C4,C4,C4,F4,C4,C4,C4,C5, F4,F4,D4,D4,C4,C4,F4, C4,C4,C4,C4,C4,C4,C4,A4,C4,F4, F4,
                       F4
};
const char beats[] = {8,16, 1, 1, 1, 1,
                       4,2,8,16,16, 4,2,4, 2,2, 2,2,
                       8,8,16,16,16,16,16,16,16,16,16,16,16,16,
                       8,8,16,16,16,16,16,16,16,16,16,16,16,16,
                       2,8,16,16,4, 2,4,8,16,16, 4,16,8,16,8,16,6,8, 4,16,8,16,8,16,6,8,
                       2,2, 2,8,16,16,8,16,16, 4,8,16,6,4,16,16, 8,16,8,8,4,16,8,16,16,
                       4,4,8,16,8,6, 8,16,8,8,16,2, 8,16,16,8,16,8,8,4,16, 16,16,16,16,8,16,8,8,6,8,
                       4,8,16,16,4,8,16,16, 16,16,8,8,16,4,16,8,16,16, 4,8,16,6,4,16,16, 8,16,8,8,4,16,8,16,16,
                       4,4,8,16,8,6, 8,16,8,8,16,2, 8,16,16,8,16,8,8,4,16, 
                       16,16,16,16,8,16,8,8,6,8, 4,8,16,16,4,8,16,16, 16,16,16,8,8,16,8,8,8,8, 1,
                       1
                        };

//void delay_ms(unsigned int milliseconds) {
//   while(milliseconds > 0) {
//      milliseconds--; // 1 * 4 / (4 *  10^-7) = 10^-7 (s)
//       __delay_us(990);
//   }
//}

void SetupClock() {
    OSCCONbits.IRCF = 0b110; // set internal oscillator to 4 Mhz
    OSCCONbits.SCS = 0b00; // use oscillator defined by FOSC
}

int CalcSpeed(int beat, int pos) {
    int tmp;
    if (beat == 1) tmp = 48;
    else if (beat == 2) tmp = 24;
    else if (beat == 4) tmp = 12;
    else if (beat == 6) tmp = 9;
    else if (beat == 8) tmp = 6;
    else if (beat == 16) tmp = 3;
    else {
        tmp = 1;
    }
    tmp *= 10;
    
    if (pos) tmr1_cycle = tmp;
    else tmr3_cycle = tmp;
    Nop();
    return 0;
}

void SetupTimer() {
    T0CON = 0b11010111; // 256 prescalar
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;

    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    led_cycle = 0;
}

// set PR2 to the desired frequency 
/* 
 * PWM Period = [PR2 + 1] * 4 * Tosc * TMR2 Prescaler
 * => [PR2 + 1] = PWM Period / 4 / Tosc / TMR2 Prescaler
 * => [PR2 + 1] = Fosc * PWM Period / 4 / TMR2 Prescaler
 * => [PR2 + 1] = Fosc / PWM frequency / 4 / TMR2 Prescaler
 * => PR2 = (Fosc / PWM frequency / 4 / TMR2 Prescaler) - 1
*/
// PWM frequency is buzzer frequency
void PWM_Init(long desired_frequency) {
    PR2 = (_XTAL_FREQ / (desired_frequency * 4 * TMR2PRESCALE)) - 1;
    current_tone = desired_frequency;
}

int PWM_Max_Duty() {
    return (_XTAL_FREQ / (current_tone * TMR2PRESCALE));
}

void PWM_Duty(unsigned int duty) {
    if (duty < 1024) {
        duty = ((float) duty / 1023) * PWM_Max_Duty();
        CCP1X = duty & 2;
        CCP1Y = duty & 1;
        CCPR1L = duty >> 2;
    }
}

void PWM_Mode_On() {
    CCP1M3 = 1;
    CCP1M2 = 1;
}

void PWM_Start() {
    PWM_Mode_On();

    T2CKPS0 = 1;
    T2CKPS1 = 1;

    TMR2ON = 1;
}

void PWM_Stop() {
    CCP1M3 = 0;
    CCP1M2 = 0;
}

void playTone() {
    PWM_Init(current_tone);
    PWM_Duty(512); // set duty cycle to 50%
}

void Setup(int song) {
     
    TRISCbits.TRISC1 = 0;
    CCP2CON = 0x02;
    PIR2bits.CCP2IF = 0;
    TMR3 = 0;
    T3CON = 0xC0;
    CCPR2 = 1000;
    T3CONbits.TMR3ON = 1;
    
    if (song == 0) {
        wm_1 = melody;
        wb_1 = beats;
        wm_2 = melody2;
        wb_2 = beats2;
        MELODY_LENGTH = sizeof (melody) / sizeof (melody[0]);
        MELODY_LENGTH2 = sizeof (melody2) / sizeof (melody2[0]);
    }
    else if (song == 1){
        wm_1 = melody3;
        wb_1 = beats3;
        wm_2 = melody3;
        wb_2 = beats3;
        MELODY_LENGTH = sizeof (melody3) / sizeof (melody3[0]);
        MELODY_LENGTH2 = sizeof (melody3) / sizeof (melody3[0]);
    }
    else {
        wm_1 = melody4;
        wb_1 = beats4;
        wm_2 = melody4;
        wb_2 = beats4;
        MELODY_LENGTH = sizeof (melody4) / sizeof (melody4[0]);
        MELODY_LENGTH2 = sizeof (melody4) / sizeof (melody4[0]);
    }
    
    MELODY_PTR = 0;
    MELODY_PTR2 = 0;

    CalcSpeed(16, 0);
    CalcSpeed(16, 1);
    SetupClock();
    SetupTimer();
    // enable RC2 to output
    TRISCbits.TRISC2 = 0; 
    LATC = 0;

    PWM_Init(500); //exact value doesn't matter at all, just not 0
    PWM_Duty(0);
    PWM_Start();
}

int main(int argc, char** argv) {
    // input to trigger music
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;   

    while (1) {
        if (PORTDbits.RD0 == 0 && PORTDbits.RD1 == 0) {
            isRunning = 0;
            TRISCbits.TRISC0 = 1;
            TRISCbits.TRISC1 = 1;
            INTCONbits.TMR0IE = 1;
            PWM_Duty(0);
        }
        if (!isRunning && PORTDbits.RD0 == 1 && PORTDbits.RD1 == 0) {
            isRunning = 1;
            Setup(0);
        }
        else if (!isRunning && PORTDbits.RD1 == 1 && PORTDbits.RD0 == 0) {
            isRunning = 1;
            Setup(1);
        }
        else if (!isRunning && PORTDbits.RD0 == 1 && PORTDbits.RD1 == 1) {
            isRunning = 1;
            Setup(2);
        }
        if (PIR2bits.CCP2IF) {
            PIR2bits.CCP2IF = 0;
            TMR3 = 0;
        }
    }
    return 0;
}

void __interrupt(high_priority) HI_ISR(void) {
    if (INTCONbits.TMR0IF) { 
        INTCONbits.TMR0IF = 0;
        TMR0 = 0xF1;
        
        if (MELODY_PTR >= MELODY_LENGTH && MELODY_PTR2 >= MELODY_LENGTH2) {
            MELODY_PTR = 0;
            MELODY_PTR2 = 0;
            led_cycle = 0;
            led2_cycle = 0;
            
            isRunning = 0;
            TRISCbits.TRISC0 = 1;
            TRISCbits.TRISC1 = 1;
            INTCONbits.TMR0IE = 1;
            PWM_Duty(0);
        }
        
        ++led_cycle;
        ++led2_cycle;
        if (led_cycle >= tmr1_cycle && MELODY_PTR < MELODY_LENGTH) {
            ++MELODY_PTR;           
            
            current_tone = wm_1[MELODY_PTR];
            int beat = wb_1[MELODY_PTR];
            if (beat == 1) tmr1_cycle = 48;
            else if (beat == 2) tmr1_cycle = 24;
            else if (beat == 4) tmr1_cycle = 12;
            else if (beat == 6) tmr1_cycle = 9;
            else if (beat == 8) tmr1_cycle = 6;
            else if (beat == 16) tmr1_cycle = 3;
            tmr1_cycle *= 10;
            if (wm_1[MELODY_PTR] == R) { //close the buzzer
                TRISCbits.TRISC0 = 1;
            }
            else {
                TRISCbits.TRISC0 = 0;
            }
            
            playTone();
            
            led_cycle = 0;
        }
        
        if (led2_cycle >= tmr3_cycle && MELODY_PTR2 < MELODY_LENGTH2) {
            ++MELODY_PTR2;
            
//            ++CCPR2; // 500 means 1000Hz
//            if (CCPR2 > 2000) CCPR2 = 0;
            CCPR2 = 1000000/wm_2[MELODY_PTR2];
            if (wm_2[MELODY_PTR2] == R) { //close the buzzer
                TRISCbits.TRISC1 = 1;
                CCPR2 = 0;
            }
            else {
                TRISCbits.TRISC1 = 0;
                CCPR2 = 1000000/wm_2[MELODY_PTR2];
            }
            
            int beat = wb_2[MELODY_PTR2];
            if (beat == 1) tmr3_cycle = 48;
            else if (beat == 2) tmr3_cycle = 24;
            else if (beat == 4) tmr3_cycle = 12;
            else if (beat == 6) tmr3_cycle = 9;
            else if (beat == 8) tmr3_cycle = 6;
            else if (beat == 16) tmr3_cycle = 3;
            tmr3_cycle *= 10;
            
            led2_cycle = 0;
        }

    }
}