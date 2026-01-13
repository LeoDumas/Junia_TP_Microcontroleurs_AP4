#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

/*
 * Dans Utilisation du Timer 2 (chenillard), nous définissons le PR2 ainsi que
 * le prescaler et le postscaler, afin qu'un délai puisse être présent entre
 * chaque cycle d'allumage de leds afin d'obtenir des cycles d'une durée de 1 seconde
 * comme indiqué.
 * 
 * Comme chaque exécution prend 0.5us, il est ainsi nécessaire de :
 * Calculer : (PR2 + 1) * Prescaler * Tinstr = (124 + 1) * 16 * 0.5us = 1000us = 1ms.
 * Ensuite, il faut utiliser un compteur logiciel de 125 itérations dans la fonction de délai pour
 * atteindre 125ms par LED (125 * 1ms).
 * Étant donné qu'il y a 8 LEDs au total, le cycle complet (8 * 125ms) durera 1 seconde
 * 
 * Une fonction est utilisé pour allumer chaque led une à une sous la forme d'une machine d'état.
*/


void init_timer2(void) {
    PR2 = 124; // Pour config PR2 sur 1ms 
                // 1ms = (PR2 + 1) * Tinstr * Prescaler
                // 1000us = 125 * 0.5us * 16
    
    T2CONbits.T2CKPS = 2; // Pour prescaler de 16
    T2CONbits.T2OUTPS = 0; // Pour postscale de 1:1
    T2CONbits.TMR2ON = 1;
}

void delai_par_led(void) {
    for (int i = 0; i < 125; i++) {
        while (PIR1bits.TMR2IF == 0);
        PIR1bits.TMR2IF = 0;
    }
}

void allumer_led_unique(int numero_led) {
    LATDbits.LATD0 = 0; LATDbits.LATD1 = 0; LATDbits.LATD2 = 0; LATDbits.LATD3 = 0;
    LATBbits.LATB0 = 0; LATBbits.LATB1 = 0; LATBbits.LATB2 = 0; LATBbits.LATB3 = 0;
    
    switch(numero_led) {
        // D1 à D4 sur PORTD
        case 0: LATDbits.LATD0 = 1; break; // D1
        case 1: LATDbits.LATD1 = 1; break; // D2
        case 2: LATDbits.LATD2 = 1; break; // D3
        case 3: LATDbits.LATD3 = 1; break; // D4
        
        // D5 à D8 sur PORTB
        case 4: LATBbits.LATB0 = 1; break; // D5
        case 5: LATBbits.LATB1 = 1; break; // D6
        case 6: LATBbits.LATB2 = 1; break; // D7
        case 7: LATBbits.LATB3 = 1; break; // D8
        default: break;
    }
}

void main(void) {
    /* Code d'initialisation */
    
    // Configuration des broches en sortie
    TRISDbits.TRISD0 = 0; TRISDbits.TRISD1 = 0; TRISDbits.TRISD2 = 0; TRISDbits.TRISD3 = 0;
    TRISBbits.TRISB0 = 0; TRISBbits.TRISB1 = 0; TRISBbits.TRISB2 = 0; TRISBbits.TRISB3 = 0;
    
    init_timer2();
    int position = 0;

    while(1) {
        allumer_led_unique(position);   
        delai_par_led();
        position++;
        if(position > 7) {
            position = 0;
        }
    }
}