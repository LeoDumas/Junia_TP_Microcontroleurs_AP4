#include <xc.h>
#include "configbits.h"

/*
 * Dans ce tp, nous nous sommes basés sur le TP précédent afin d'avoir tous les inits.
 * Comme indiqué dans "Liaison UART ? réception de données", nous avons configuré le flag d'interruption
 * Concernant la réception et l'echo du texte envoyé, nous définissons un buffer_size de 64
 * Comme discuté lors du cours, un buffer n'était pas nécessaire et seul un caractère devait être 
 * retourné comme indiqué de même dans l'énoncé. Nous avons mal lu la consigne.
 * 
 * Dans le __interrupt, nous vérifions pour chaque caractère écrit, si celui-ci est un EOL ou équivalent,
 * nous affichons sa valeur avec un "echo: " devant
 */


#define BUFFER_SIZE 64
volatile char rx_buffer[BUFFER_SIZE];
volatile int rx_index = 0;
volatile char message_ready = 0;

void init_serial(void) {
    RC6PPS = 0x14;
    RXPPS = 0x17;  

    LATCbits.LATC6 = 0;
    TRISCbits.TRISC6 = 0;  
    
    ANSELCbits.ANSC7 = 0;
    TRISCbits.TRISC7 = 1;
}

void init_baudrate(void) {
    TX1STAbits.SYNC = 0;
    TX1STAbits.BRGH = 0;
    BAUD1CONbits.BRG16 = 0;

    SP1BRGL = 12;
    SP1BRGH = 0;
}

void enable_eusart(void) {
    RC1STAbits.SPEN = 1; 
    TX1STAbits.TXEN = 1;
    RC1STAbits.CREN = 1;
}

void init_interrupts(void) {
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;  
}

void init_all(void){
    init_serial();
    init_baudrate();
    enable_eusart();
    init_interrupts();
}

void UART_Write(char data) {
    while (PIR1bits.TXIF == 0) {}
    TX1REG = data;
}

void display_text(char *str) {
    // Lecture jusqu'au dernier caractère
    while (*str != '\0') {
        UART_Write(*str);
        str++;
    }
}

void __interrupt() isr(void) {
    if (PIR1bits.RCIF) {
        char received_char = RC1REG;

        if (received_char == '\r' || received_char == '\n') {
            rx_buffer[rx_index] = '\0';
            message_ready = 1;
        } 
        else {
            if (rx_index < (BUFFER_SIZE - 1)) {
                rx_buffer[rx_index] = received_char;
                rx_index++;
                UART_Write(received_char);
            }
        }
    }
}

void main(void) {
    init_all();

    while (1) {
        if (message_ready) {
            display_text("\r\necho: ");
            display_text((char*)rx_buffer);
            display_text("\r\n");

            rx_index = 0;
            message_ready = 0;
        }
    }
}   