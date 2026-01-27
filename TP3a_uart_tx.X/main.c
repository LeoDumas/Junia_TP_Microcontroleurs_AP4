#include <xc.h>
#include "configbits.h"

/*
 * On suivant toutes les étapes à suivre dans "I.b Configuration logicielle (Microcontrôleur)",
 * nous avons créer les diverses fonctions permettant leurs initialisations.
 * UART_Write permet d'écrire un caractère dans TX1REG, UART_Read permet de renvoyer RC1REG
 * contenant la valeur complète des données envoyés à TX1REG.
 * display_text, permet alors d'envoyer chaque caractère du string un à un dans TX1REG.
 * executant à la fin UART_Read, permettant d'afficher dans ce qui est contenu dans notre terminal
 */

void init_serial(void) {
    RC6PPS = 0x14; 
    RXPPS = 0x17;  

    // RC6 as output
    LATCbits.LATC6 = 0;
    TRISCbits.TRISC6 = 0;  
    
    // RC7 as input
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

void init_all(void){
    init_serial();
    init_baudrate();
    enable_eusart();
}

void UART_Write(char data) {
    while (PIR1bits.TXIF == 0) {}
    TX1REG = data;
}

char UART_Read(void) {
    while (PIR1bits.RCIF == 0) {}
    return RC1REG;
}

void display_text(char *str) {
    // Lecture jusqu'au dernier caractère
    while (*str != '\0') {
        UART_Write(*str);
        str++;
    }
    UART_Read();
}

void main(void) {
    init_all();
    
    while (1) {
        display_text("Hello, World!");
    }
}