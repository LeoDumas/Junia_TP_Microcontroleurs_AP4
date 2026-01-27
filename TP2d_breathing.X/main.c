#include <xc.h>
#include "configbits.h"
#include "leds.h"
#include "timers.h"

/*
 * Comme Timer 2 est configuré pour 1ms avec PR2=124 et un Prescaler de 1:16,
 * notre PWM Max correspond à PWM4DCH = 124.
 * Soit pour avec 1 secondes, il faut passer de 0 à 124
 * Soit, 1000 ms / 125 pas = 8 ms par pas.
 * 
 * Dans notre interrupt, nous avons un toggle afin d'inverser la sens de notre LED.
 */
#define WAIT_TICKS 8 
#define PWM_MAX 124

int step_counter = 0;
int breath_value = 0;
int isInhaling = 1;

void config_pwm4(void) {
    PWM4DCH = 0;
    PWM4DCL = 0;
    
    PWM4CONbits.PWM4POL = 0;
    PWM4CONbits.PWM4EN = 1;
    
    // Connecter PWM4 à D0
    TRISDbits.TRISD0 = 0;
    RD0PPS = 0x0F; // -> Pour pwm4
}

void varier_d1(int b_value){
    PWM4DCH = b_value; 
    PWM4DCL = 0;
    
    PWM4CONbits.PWM4EN = 1;
}

void __interrupt() isr(void) {
    if (PIE1bits.TMR2IE && PIR1bits.TMR2IF) {
        
        PIR1bits.TMR2IF = 0;
        
        step_counter++;

        if (step_counter >= WAIT_TICKS) {
            step_counter = 0;

            varier_d1(breath_value);

            if (isInhaling) {
                breath_value++;
                if (breath_value >= PWM_MAX) {
                    breath_value = PWM_MAX;
                    isInhaling = 0;
                }
            } else {
                breath_value--;
                if (breath_value <= 0) {
                    breath_value = 0;
                    isInhaling = 1;
                }
            }
        }
    }
}

void main(void) {
    init_leds();
    init_timer2();      
    init_interrupt();   
    config_pwm4();
    
    while(1) {}
}
