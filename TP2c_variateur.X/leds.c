#include "leds.h"

void init_leds(void) {
    // Configuration des directions en sortie (0)
    LEDS_D1_DIR = 0; LEDS_D2_DIR = 0; LEDS_D3_DIR = 0; LEDS_D4_DIR = 0;
    LEDS_D5_DIR = 0; LEDS_D6_DIR = 0; LEDS_D7_DIR = 0; LEDS_D8_DIR = 0;
    
    // État initial éteint
    leds_off_all();
}

void leds_off_all(void) {
    LEDS_D1_STATE = 0; LEDS_D2_STATE = 0; LEDS_D3_STATE = 0; LEDS_D4_STATE = 0;
    LEDS_D5_STATE = 0; LEDS_D6_STATE = 0; LEDS_D7_STATE = 0; LEDS_D8_STATE = 0;
}

void allum_D1_D4(void){
    leds_off_all(); // On éteint tout d'abord
    LEDS_D1_STATE = 1; LEDS_D2_STATE = 1; LEDS_D3_STATE = 1; LEDS_D4_STATE = 1;
}

void allum_D5_D8(void){
    leds_off_all(); // On éteint tout d'abord
    LEDS_D5_STATE = 1; LEDS_D6_STATE = 1; LEDS_D7_STATE = 1; LEDS_D8_STATE = 1;
}

void allumer_led_unique(int numero_led) {
    leds_off_all(); // Reset
    
    switch(numero_led) {
        case 0: LEDS_D1_STATE = 1; break;
        case 1: LEDS_D2_STATE = 1; break;
        case 2: LEDS_D3_STATE = 1; break;
        case 3: LEDS_D4_STATE = 1; break;
        case 4: LEDS_D5_STATE = 1; break;
        case 5: LEDS_D6_STATE = 1; break;
        case 6: LEDS_D7_STATE = 1; break;
        case 7: LEDS_D8_STATE = 1; break;
        default: break;
    }
}
