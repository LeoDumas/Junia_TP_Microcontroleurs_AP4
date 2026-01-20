#ifndef LEDS_H
#define LEDS_H

#include <xc.h>

// LEDs D1 à D4 (PORT D)
#define LEDS_D1_DIR TRISDbits.TRISD0
#define LEDS_D1_STATE LATDbits.LATD0
#define LEDS_D2_DIR TRISDbits.TRISD1
#define LEDS_D2_STATE LATDbits.LATD1
#define LEDS_D3_DIR TRISDbits.TRISD2
#define LEDS_D3_STATE LATDbits.LATD2
#define LEDS_D4_DIR TRISDbits.TRISD3
#define LEDS_D4_STATE LATDbits.LATD3

// LEDs D5 à D8 (PORT B)
#define LEDS_D5_DIR TRISBbits.TRISB0
#define LEDS_D5_STATE LATBbits.LATB0
#define LEDS_D6_DIR TRISBbits.TRISB1
#define LEDS_D6_STATE LATBbits.LATB1
#define LEDS_D7_DIR TRISBbits.TRISB2
#define LEDS_D7_STATE LATBbits.LATB2
#define LEDS_D8_DIR TRISBbits.TRISB3
#define LEDS_D8_STATE LATBbits.LATB3

void init_leds(void);
void allumer_led_unique(int numero_led);
void allum_D1_D4(void);
void allum_D5_D8(void);
void leds_off_all(void);

#endif // LEDS_H
