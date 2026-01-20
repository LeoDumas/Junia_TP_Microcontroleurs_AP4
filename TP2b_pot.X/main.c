#include <xc.h>
#include "leds.h"

/*
 * En nous basant sur le code donnée dans le cours, nous l'avons adapté afin d'obtenir la valeur
 * du potentiomettre avec AN0/RA0
 * Le shift présent dans la loop while est de 5 car 2^5 est égale = 32
 * ce qui signifie que nous avons 256 valeur possible car ADRESH retourné par read_adc est de 0 bits
 * 2^8 = 256, une division par 32 permet d'obtenir la led à allumer en utilisant l'une des fonctions
 * LEDs créée dans le TP1. La valeur reste un entier signifiant que le bonne LED sera choisit
 */

void config_adc(void){
    // Mettre le potentiomettre en mode entrée
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSA0 = 1;
    
    ADCON0bits.CHS = 0; // Pour utiliser AN0 donné dans l'énnoncés
    // ADCON1bits.ADPREF = 0;
    // ADCON1bits.ADNREF = 0;
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
    init_leds();
    config_adc();
    
    while(1) {
        allumer_led_unique((int)read_adc() >> 5); // Shift de 5 bits car 2^5 = 32
    }
    return;
}
