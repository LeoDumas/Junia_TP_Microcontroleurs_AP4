#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

/*
 * Ici nous utilisons l'interruption afin de faire le chenilard
 * GIE, PEIE et TMR2IE activent le processus d'interruption vers l'ISR selon la figure 7-1 INTERRUPT LOGIC
 * L'ISR incrémente step_counter et remet TMR2IF à 0.
 * Un seuil de 125 interruptions génère un délai de 125ms par LED.
 * Le cycle total pour 8 LEDs atteint ainsi exactement 1 seconde (8 * 125ms).
 * Cette méthode libère le main(), qui se limite à une boucle while vide.
 * L'exécution est plus précise car le temps n'est pas lié à la vitesse du code principal car il force l'arret.
*/

unsigned int step_counter = 0; 
int position = 0;

void init_timer2(void) {
    PR2 = 124;            // 1ms
    T2CONbits.T2CKPS = 2; // Prescaler de 1:16
    T2CONbits.T2OUTPS = 0; // Postscaler de 1:1
    T2CONbits.TMR2ON = 1;
}

void init_interrupt(void){
    INTCONbits.PEIE = 1; // Nécessaire pour interrupt CPU
    INTCONbits.GIE = 1; // Nécessaire pour interrup CPU
    PIE1bits.TMR2IE = 1; // Indiquer que les interruptions sont activé
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

void __interrupt() isr(void){
    if(PIE1bits.TMR2IE && PIR1bits.TMR2IF){
        PIR1bits.TMR2IF = 0;
        
        step_counter++;
        
        if(step_counter >= 125) {
            step_counter = 0;
            
            position++;
            if(position > 7) {
                position = 0;
            }
            allumer_led_unique(position);
        }
    }
}

void main(void) {
    /* Code d'initialisation */
    TRISDbits.TRISD0 = 0; TRISDbits.TRISD1 = 0; TRISDbits.TRISD2 = 0; TRISDbits.TRISD3 = 0;
    TRISBbits.TRISB0 = 0; TRISBbits.TRISB1 = 0; TRISBbits.TRISB2 = 0; TRISBbits.TRISB3 = 0;
    
    allumer_led_unique(0);
    init_timer2();
    init_interrupt();

    while(1) {}
}