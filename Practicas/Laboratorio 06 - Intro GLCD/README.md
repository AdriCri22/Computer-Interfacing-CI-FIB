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

# Entrega Lab

```c
#include <xc.h>
#define _XTAL_FREQ 8000000  

#include <string.h>
#include "config.h"
#include "GLCD.h"

void writeFunc1(int k) { //y = kx
   int x = 0;
   for (int y = 0; y < 63; y++) {
      SetDot(63 - y, x);
      if (y == k * x) x++;
   }
}

void writeFunc2(int k) { //y = kx^2
   int x = 0;
   for (int y = 0; y < 63; y++) {
      SetDot(63 - y, x);
      if (y == k * (x * x)) x++;
   }
}

void writeFunc3(int k) { //y = kx^3
   int x = 0;
   for (int y = 0; y < 63; y++) {
      SetDot(63 - y, x);
      if (y == k * (x * x * x)) x++;
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
   
   writeFunc1(1);
   
   int lastRA0, func = 0;
   while (1) {
      if (PORTAbits.RA0 == 0) lastRA0 = 1;
      
      if (PORTAbits.RA0 == 1 && lastRA0 == 1) {
	 lastRA0 = 0;
	 func++;
	 if (func == 3) func = 0;
	 clearGLCD(0, 7, 0, 127);
	 if (func == 0) writeFunc1(1);
	 else if (func == 1) writeFunc2(1);
	 else writeFunc3(1);  
      }
   }
}

```

```h
/* La pantalla té dos xips de memòria. Un controla la meitat esquerra i
l'altre la meitat dreta. Cada meitat es divideix en 8 files i 64 columnes.
Cada divisio d'aquestes és un byte i el bit de menor pes és el que està més amunt */

typedef unsigned char byte;


/* Envia una ordre al xip determinat per CS. L'ordre és un codi dels definits abans
OR el valor que es vol enviar */
void sendGLCDCommand(byte val, byte CS);

/* Estableix la línia que es mostra a la part de dalt de la pantalla (de 0 a 63)
Serveix per a fer scroll sense moure els continguts de tota la memòria */
void setStartLine(byte z);

/* Estableix la pàgina actual (de 0 a 7) */
void setXAddress(byte x);

/* Estableix la columna actual (de 0 a 127) */
void setYAddress(byte y);

/* Inicialitzacio GLCD */
void GLCDinit(void);

/* Escriu un byte a la pantalla a la pagina p (de 0 a 7), columna y (de 0 a 127) */
void writeByte(byte p, byte y, byte data);

/* Escriu el caràcter c a la pagina p (de 0 a 7), character column y (de 0 a 20)
Cada columna de text té una amplada de 6 píxels.
Compte els caracters estan definits amb un font 7x5 (7 alt, 5 amplada) */
void putchGLCD(byte p, byte y, char c);


/* Esborra la pantalla 
entre les files ri, re i les columnes ci, ce*/
void clearGLCD(byte ri, byte re, byte ci, byte ce);

// llegeix un byte de la pantalla de la pagina p (de 0 a 7), columna y (de 0 a 127)
byte readByte(byte p, byte y);

// Encen el pixel a la posicio (x,y), (x [0:63]) (y [0:127])
void SetDot(byte x, byte y);

// Apaga el pixel a la posicio (x,y), (x [0:63]) (y [0:127])
void ClearDot(byte x, byte y);

// page p [0:7], character column y [0:20], value [-32768:32767]
void writeNum (byte p, byte y, int value);
```
