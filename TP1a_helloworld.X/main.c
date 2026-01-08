#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

/*
Commentaire :
 * Cette partie du TP fonctionne comme demandé.
 * L'utilisation de la fonction delai_approx fonctionne en utilisant une boucle faisait
 * 85 000 itérations afin d'avoir un délai approximatif de 1 seconde.
 * Ce nombre a été trouvé de manière empirique pour avoir l'équivalent d'une seconde.
 * 
 * Pour les fonctions allum_*, comme discuté en cours, chaque LED a été allumée de manière sélective et non une range
 * afin de ne pas déclencher une bombe nucléaire.
 
 */
void delai_approx(void){
    unsigned long i = 0;
    for(i=0; i < 85000; i++){}
}

void allum_D1_D4(void){
    // Éteindre D5_D8
    TRISBbits.TRISB0 = 0; LATBbits.LATB0 = 0;
    TRISBbits.TRISB1 = 0; LATBbits.LATB1 = 0;
    TRISBbits.TRISB2 = 0; LATBbits.LATB2 = 0;
    TRISBbits.TRISB3 = 0; LATBbits.LATB3 = 0;
    
    // Allumer D1_D4
    TRISDbits.TRISD0 = 0; LATDbits.LATD0 = 1;
    TRISDbits.TRISD1 = 0; LATDbits.LATD1 = 1;
    TRISDbits.TRISD2 = 0; LATDbits.LATD2 = 1;
    TRISDbits.TRISD3 = 0; LATDbits.LATD3 = 1;
}

void allum_D5_D8(void){
    // Éteindre D1_D4
    TRISDbits.TRISD0 = 0; LATDbits.LATD0 = 0;
    TRISDbits.TRISD1 = 0; LATDbits.LATD1 = 0;
    TRISDbits.TRISD2 = 0; LATDbits.LATD2 = 0;
    TRISDbits.TRISD3 = 0; LATDbits.LATD3 = 0;
    
    // Allumer D5_D8
    TRISBbits.TRISB0 = 0; LATBbits.LATB0 = 1;
    TRISBbits.TRISB1 = 0; LATBbits.LATB1 = 1;
    TRISBbits.TRISB2 = 0; LATBbits.LATB2 = 1;
    TRISBbits.TRISB3 = 0; LATBbits.LATB3 = 1;
}


void main(void) {
    /* Code d'initialisation */
    
    while(1){
        allum_D1_D4();
        delai_approx();
        allum_D5_D8();
        delai_approx();
    }
}