#include "timers.h"

void init_timer2(void) {
    PR2 = 124;                 // 1ms
    T2CONbits.T2CKPS = 2;      // Prescaler 1:16
    T2CONbits.T2OUTPS = 0;     // Postscaler 1:1
    T2CONbits.TMR2ON = 1;
}

void init_interrupt(void){
    INTCONbits.PEIE = 1; 
    INTCONbits.GIE = 1; 
    PIE1bits.TMR2IE = 1; 
}

void delai_par_led_polling(void) {
    for (int i = 0; i < 125; i++) {
        while (PIR1bits.TMR2IF == 0);
        PIR1bits.TMR2IF = 0;
    }
}