#include <xc.h>
#include "leds.h"

/*
 * En se basant sur les deux dernières partie du TP2, seul de l'adaptation était nécessaire
 * Dans un premier temps, la PR2 était à mettre à 255 afin de varier de 0 à 1
 * permettant d'aller de 0% à 100%
 * Comme le potentiomettre va de 0 à 255, la valeur est assigné à PWM4DCH permettant
 * de répondre à [0;1] = [0;255]/(254+1) 
 */

void config_timer2(void){
T2CONbits.T2CKPS = 1;
PR2 = 254;
T2CONbits.TMR2ON = 1;
}

void config_pwm4(void) {
    PWM4DCH = 0;
    PWM4DCL = 0;
    
    PWM4CONbits.PWM4POL = 0;
    PWM4CONbits.PWM4EN = 1;
    
    // Connecter PWM4 à D0
    TRISDbits.TRISD0 = 0;
    RD0PPS = 0x0F; // -> Pour pwm4
}

void varier_d1(int pot_value){
    PWM4DCH = pot_value; 
    PWM4DCL = 0;
    
    PWM4CONbits.PWM4EN = 1;
}

void config_adc(void){
    // Mettre le potentiomettre en mode entrée
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSA0 = 1;
    
    ADCON0bits.CHS = 0; // Pour utiliser AN0 donné dans l'énnoncés
    ADCON1bits.ADCS = 0b010;
    ADCON1bits.ADFM = 0;
    
    ADCON0bits.ADON = 1; // ADC : en marche
}

char read_adc(void){
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO){}
    return ADRESH;
}


void main(void) {
    config_timer2();
    config_pwm4();
    init_leds();
    config_adc();
    
    while(1){
        varier_d1((int)read_adc());
    }
}
