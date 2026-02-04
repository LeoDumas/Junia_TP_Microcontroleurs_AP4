#include <xc.h>
#include "configbits.h"
#include "lcd.h"
#include "spi.h"

/*
 * On suivant les instrucitons présente dans le TP, nous avons réalisé l'affichage du hello world.
 * La boucle infi permet de garder l'affichage sans clignotement.
 */

void main(void) {
    SPI_InitializePins();
    LCD_InitializePins();
    SPI_Initialize();
    LCD_Initialize();
    LCD_Clear();
    LCD_GoTo(0,0);
    const char* value = "Hello world !";
    LCD_WriteString(value);
    
    while(1){}
    
    return;
}
