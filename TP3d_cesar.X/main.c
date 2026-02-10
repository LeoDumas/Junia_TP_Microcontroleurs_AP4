#include <xc.h>
#include <ctype.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "configbits.h"
#include "lcd.h"
#include "spi.h"
#include "timers.h"


/*
 * Dans ce TP de chiffrement, déchiffrement avec le code césar, nous avons utilisé ce qui a été fait dans les TPs 
 * Précédent, c'est-à-dire l'utilisation du potentiomètre, le bouton, le LCD et l'UART.
 * Pour l'étape du chiffrement et déchiffrement du code César, nous avons utilisé un alphabet spécial.
 * Nous utilisons donc la librairie "ctype" nous permettant de vérifier si un char est un digit ou non,
 * Nous permettant de récupérer le bon indice dans l'alphabet personnalisé (avec un +10 dans un alphanumérique).
 * On peut ensuite ajouter l'offset et obtenir la valeur suite à l'opération césar.
 */


#define B_S1 PORTBbits.RB0
#define BUFFER_SIZE 64
#define ALPHABET "0123456789abcdefghijklmnopqrstuvwxyz"

// LCD
char globalPotValue;
int globalEncryptingValue = -1;
int isEncrypting = 1;

// UART
char rx_buffer[BUFFER_SIZE];
int rx_index = 0;
char message_ready = 0;

char to_lowercase(char text){
    return tolower(text);
}

void init_rb0(void){
    // Button RB0
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSB0 = 0;    
}

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

void init_lcd(void){
    SPI_InitializePins();
    LCD_InitializePins();
    SPI_Initialize();
    LCD_Initialize();
    LCD_Clear();
    LCD_GoTo(0,0);
}

void change_lcd_value(char* text){
    LCD_WriteString(text);
    LCD_Clear();
}

void print_pot_value_lcd(char potValue){
    char buffer[20];
    if(potValue != globalPotValue){
        globalPotValue = potValue;
        LCD_GoTo(0,0);
        sprintf(buffer, "Offset : %-3u", potValue);
        LCD_WriteString(buffer);
    }
}

void print_current_mode_to_lcd(){
    if (isEncrypting != globalEncryptingValue) {
        globalEncryptingValue = isEncrypting;
        LCD_GoTo(1,0);
        LCD_WriteString((isEncrypting == 1) ? "chiffrement  " : "dechiffrement" );
    }
}

// UART part

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
                rx_buffer[rx_index] = to_lowercase(received_char);
                rx_index++;
                UART_Write(received_char);
            }
        }
    }
}

void crypto_on_message(char* message, int offset){
    int alphabet_len = strlen(ALPHABET);
    
    for(int i = 0; message[i] != '\0'; i++){
        int current_pos = -1;

        if(isdigit(message[i])){
            current_pos = message[i] - '0'; 
            
        }else if(islower(message[i])){
            current_pos = message[i] - 'a' + 10; // +10 car 'a' est à la 10eme position
        }
        if(current_pos != -1){
            int new_pos = (isEncrypting)
            ? (current_pos + offset) % alphabet_len
            : (current_pos - offset + alphabet_len) % alphabet_len;
            
            if(new_pos < 0){
                new_pos += alphabet_len;
            }
            
            message[i] = ALPHABET[new_pos];
        }
    }
}

void init_all(void){
    config_adc();
    init_lcd();
    init_rb0();
    init_serial();
    init_baudrate();
    enable_eusart();
    init_interrupts();
    LCD_Clear();
}

void main(void) {
    init_all();

    while(1){
        if (B_S1 == 0) {
            while(B_S1 == 0);
            isEncrypting = !isEncrypting;
        }
        
        if (message_ready) {
            char *text = (char *)rx_buffer;
            char val = (char)((int)(read_adc() / 7.5) + 1);

            display_text(isEncrypting ? "\r\nReponse (Chiffree): "
                                      : "\r\nReponse (Dechiffree): ");

            crypto_on_message(text, val);
            display_text(text);
            display_text("\r\n");

            rx_index = 0;
            message_ready = 0;
        }

        char val = (char)((int)(read_adc() / 7.5)+1);
        print_pot_value_lcd(val);
        print_current_mode_to_lcd();
    }
}
