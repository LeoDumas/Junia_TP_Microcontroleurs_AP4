#include <xc.h>

#define B_S1 PORTBbits.RB0

/*
 * Comme dit pendant le cours afin d'obtenir un pourcentage, il faut que ce 
 * pourcentage soit égale à : PWM4DCH/(PR2+1)
 * Donc 10% = 0.1 => 25/(249+1)  
 * 
 * Par la suite il faut connecter la sortie de la PWM4 à la led D0 comme défini dans config_pwm4
 * Dans le while, en fonction de l'état du boutton, la valeur du PWM4DCH sera mis à jours.
 * Celle de la PWM4DCL ne sera jamais jamais changé comme dit plus haut selon la formule
*/

void config_timer2(void){
T2CONbits.T2CKPS = 1;
PR2 = 249;
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

void allum_d1_100p(void){
    PWM4DCH = 250; // Pour un rapport  cyclique à 100% 
    PWM4DCL = 0;
    
    PWM4CONbits.PWM4EN = 1;
}

void allum_d1_10p(void){
    PWM4DCH = 25; // Pour un rapport  cyclique à 10% 
    PWM4DCL = 0;
    
    PWM4CONbits.PWM4EN = 1;
}

void init_rb0(void){
    // Button RB0
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSB0 = 0;    
}

void main(void) {
    init_rb0();
    config_timer2();
    config_pwm4();
    
    while(1){
        if(B_S1 == 0){
            allum_d1_100p();
        } else {
            allum_d1_10p(); 
        }
    }
}
