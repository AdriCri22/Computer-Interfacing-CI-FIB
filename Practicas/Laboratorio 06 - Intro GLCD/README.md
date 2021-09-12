# Entrega Previa

```c
#include <xc.h>
#define _XTAL_FREQ 8000000  

#include <string.h>
#include "config.h"
#include "GLCD.h"

const char * s = "  %";
int count = 0;

void writeTxt(byte page, byte y) {
   int i = 2;
   while (i >= 0) {
      putchGLCD(page, y, s[i]);
      y--;
      i--;
   }
}

void writeCount(byte page, byte y) {
   int num = count;
   int i = 3;
   while (i > 0) {
      putchGLCD(page, y, num % 10 + '0');
      num /= 10;
      y--;
      i--;
   }
}

void main(void)
{
   ANSELD=0;                  //PORTD configurat com Digital
   ANSELB=0;  		   //PORTB configurat com Digital
   PORTD=0; 		   //Donem uns valors inicials als ports
   PORTB=0;  
   TRISD=0x00;		   //Configurem D i B de sortida
   TRISB=0x00;
   ANSELA = 0x00;
   TRISA = 0xFF;
   
   GLCDinit();		   //Inicialitzem la pantalla
   clearGLCD(0,7,0,127);      //Esborrem pantalla
   setStartLine(0);           //Definim linia d'inici
   
   writeCount(6, 3);
   
   int lastRA0, lastRA1;
   while (1) {
      if (PORTAbits.RA0 == 0) lastRA0 = 1;
      if (PORTAbits.RA1 == 0) lastRA1 = 1;
      
      if (PORTAbits.RA0 == 1 && lastRA0 == 1) {
	 lastRA0 = 0;
	 count++;
	 writeCount(6, 3);
      }    
      
      if (PORTAbits.RA1 == 1 && lastRA1 == 1) {
	 lastRA1 = 0;
	 writeTxt(6, 3);
      }
   }
}

```
