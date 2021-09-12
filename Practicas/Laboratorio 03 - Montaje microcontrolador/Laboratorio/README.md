# Coche Fantástico

```c
#include <xc.h>
#include <math.h>
#include "config.h"

#define PINA0 PORTAbits.RA0
#define PINB0 PORTBbits.RB0
#define _XTAL_FREQ 8000000

void configPIC(){
   //config PIC
   ANSELD = 0x00;  // All pins as digital
   ANSELB = 0x00;  // All pins as digital
   ANSELC = 0x00;  // All pins as digital

   TRISD = 0x00;
   TRISB = 0xFF;
   TRISC = 0x00;

   PORTD = 0x00;
   PORTB = 0x00;
   PORTC = 0x00;
}

void main(void){
   configPIC();
   int suma = 0;
   while(1) {
      int j = 7;
      for (int i = 0; i < 8; i++) {
        PORTC = pow (2, i);
        PORTD = pow (2, j);
         j--;
        __delay_ms(150); 
      }

      j = 7;
      for (int i = 0; i < 8; i++) {
        PORTC = pow (2, j);
        PORTD = pow (2, i);
         j--;
        __delay_ms(150); 
      }
   }
}
'''

# Letras

```c
#include <xc.h>
#include "config.h"


#define PINA0 PORTAbits.RA0
#define PINB0 PORTBbits.RB0
#define _XTAL_FREQ 8000000

void configPIC(){
   //config PIC
   ANSELD = 0x00;  // All pins as digital
   ANSELB = 0x00;  // All pins as digital
   ANSELC = 0x00;  // All pins as digital

   TRISD = 0x00;
   TRISB = 0x00;
   TRISC = 0x00;
   
   PORTD = 0x00;
   PORTB = 0x00;
   PORTC = 0x00;
}

void main(void){
   configPIC();
   int suma = 0;
   while(1) {
      PORTB = 0x3C;
      PORTC = 0x7E;
      PORTD = 0x81;
      
      __delay_ms(1000);
      
      PORTB = 0xFF;
      PORTC = 0x81;
      PORTD = 0xFF;
      
      __delay_ms(1000);
   }
}
```
