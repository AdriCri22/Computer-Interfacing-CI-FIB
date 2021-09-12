# Entregas Previas

## Cifras

```c
#include <xc.h>
#define _XTAL_FREQ 8000000  

#include <string.h>
#include "config.h"
#include "GLCD.h"


void writeNumber(byte page, byte y, int n) {
   if (n == 0) putchGLCD(page, y, '0');
   else {
      while (n > 0) {
	 putchGLCD(page, y, n % 10 + '0');
	 y--;
	 n /= 10;
      }
   }
}

void configPIC() {
   ANSELD = 0;		// PORTD configured as Digital
   ANSELB = 0;          // PORTB configured as Digital
   PORTD = 0; 		// Set port values
   PORTB = 0;
   TRISD = 0x00;	// Configure B and D as Outputs
   TRISB = 0x00;
   
   GLCDinit();		  	// Initialize screen
   clearGLCD(0, 7, 0, 127);      	// Clean screen
   setStartLine(0);           	// Set start line
   
   ADCON2bits.ADFM = 1;		// Conversion Result Format select bists: Left justified
   ADCON2bits.ACQT = 0b100;	// TAD = 64 / FOSC = 8 x 10^-6
   ADCON2bits.ADCS = 0b110;	// FOSC / 64
   ADCON1bits.PVCFG = 0b00;	// A/D Vref+ = VDD
   ADCON1bits.NVCFG = 0b00;	// A/D Vref- = VSS
   TRISAbits.RA0 = 1;		// Set pin A0 as input
   ANSELA = 1;			// Set pin A0 as analog
   ADCON0bits.CHS = 0b00000;	// Analog chanel select bits
   ADCON0bits.ADON = 1;		// ADC Enabled
}

void main(void) {
   configPIC();
   int prev = 0;
   while (1) {
      ADCON0bits.GO = 1;
      while (ADCON0bits.GO);		// Wait until obtain the va
      int value = ADRESH & 0x03;	// Ignore bits 10:15
      value = value << 8;		// Move 8 bits to the left (shift left)
      value = value | ADRESL;		// Concantenate ADRESH and ADRESL
      // Refresh the screen if the value has change
      if (value != prev) {		
	 clearGLCD(6, 6, 8, 64);      	// Clean screen
	 writeNumber(6, 8, value);	// Print value
      }
      prev = value;
   }
}
```

## Barra

```c
#include <xc.h>
#define _XTAL_FREQ 8000000  

#include <string.h>
#include "config.h"
#include "GLCD.h"

// Print the voltatge ratio
void printBar(int n) {
   int p = (double) 50 / 1023 * n;	// Set ratio of the bar to 50
   // the line -1 and the line 51 represents the bounds
   for (int i = -1; i <= 50 + 1; i++) {	// 0 to 50 represents the voltatge ratio
      SetDot(i + 7, 9); SetDot(i + 7, 17);	// This are the bounds 
      if ((i <= p && p != 0) || i == 51 || i == -1) {	// print the upper and lower bounds
	 for (int j = 0; j < 7; j++) {	// Print the ratio
	    SetDot(50 - i + 7, j + 10);
	 } 
      }
   }
}

// Configuration of the PIC and of the GLCD
void configPIC() {
   ANSELD = 0;		// PORTD configured as Digital
   ANSELB = 0;          // PORTB configured as Digital
   PORTD = 0; 		// Set port values
   PORTB = 0;
   TRISD = 0x00;	// Configure B and D as Outputs
   TRISB = 0x00;
   
   GLCDinit();		  	// Initialize screen
   clearGLCD(0, 7, 0, 127);     // Clean screen
   setStartLine(0);           	// Set start line
   
   ADCON2bits.ADFM = 1;		// Conversion Result Format select bists: Left justified
   ADCON2bits.ACQT = 0b100;	// TAD = 64 / FOSC = 8 x 10^-6
   ADCON2bits.ADCS = 0b110;	// FOSC / 64
   ADCON1bits.PVCFG = 0b00;	// A/D Vref+ = VDD
   ADCON1bits.NVCFG = 0b00;	// A/D Vref- = VSS
   TRISAbits.RA0 = 1;		// Set pin A0 as input
   ANSELA = 1;			// Set pin A0 as analog
   ADCON0bits.CHS = 0b00000;	// Analog chanel select bits
   ADCON0bits.ADON = 1;		// ADC Enabled
}

void main(void) {
   configPIC();
   int prev = 0;
   while (1) {
      ADCON0bits.GO = 1;
      while (ADCON0bits.GO);		// Wait until obtain the va
      int value = ADRESH & 0x03;	// Ignore bits 10:15
      value = value << 8;		// Move 8 bits to the left (shift left)
      value = value | ADRESL;		// Concantenate ADRESH and ADRESL
      // Refresh the screen if the value has change
      if (value != prev) {		
	 clearGLCD(0, 7, 0, 20);      	// Clean screen
	 printBar(value);		// Print value
      }
      prev = value;
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
#define timeH 0x0C	// 3125
#define timeL 0x35
#define a (float) 0.951229424500714

int an0, t = 0, prev = 0, count = 0;

void printDot(int an0) {
   int value = an0 / 32;
   SetDot(value, t);
}

void printDot2(float fltr) {
   int value = (int) fltr / 32;
   SetDot(32 + value, t);
}

void interrupt highRSI(void) {
   if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {      
      count++;
      if (count == 100) {
	 count = 0;
	 ADCON0bits.GO = 1;
	 while (ADCON0bits.GO);	// Wait until obtain the va
	 an0 = ADRESH & 0x03;	// Ignore bits 10:15
	 an0 = an0 << 8;	// Move 8 bits to the left (shift left)
	 an0 = an0 | ADRESL;	// Concantenate ADRESH and ADRESL
	 t++;
	 if (t == 128) {
	    clearGLCD(0, 7, 0, 127);     // Clean screen
	    t = 0;
	 }
	 float fltr = a * prev + (1 - a) * an0;
	 prev = fltr;
	 printDot(an0);
	 printDot2(fltr);
	 
	 TMR0H = 0x00 - timeH;	// 0x00000 - 3125
	 TMR0L = 0x00 - timeL;
	 INTCONbits.TMR0IF = 0;
      }
   }
}


// Configuration of the PIC and of the GLCD
void configPIC() {
   ANSELD = 0;		// PORTD configured as Digital
   ANSELB = 0;          // PORTB configured as Digital
   PORTD = 0; 		// Set port values
   PORTB = 0;
   TRISD = 0x00;	// Configure B and D as Outputs
   TRISB = 0x00;
   
   GLCDinit();		  	// Initialize screen
   clearGLCD(0, 7, 0, 127);     // Clean screen
   setStartLine(0);           	// Set start line
   
   ADCON2bits.ADFM = 1;		// Conversion Result Format select bists: Left justified
   ADCON2bits.ACQT = 0b100;	// TAD = 64 / FOSC = 8 x 10^-6
   ADCON2bits.ADCS = 0b110;	// FOSC / 64
   ADCON1bits.PVCFG = 0b00;	// A/D Vref+ = VDD
   ADCON1bits.NVCFG = 0b00;	// A/D Vref- = VSS
   TRISAbits.RA0 = 1;		// Set pin A0 as input
   ANSELA = 1;			// Set pin A0 as analog
   ADCON0bits.CHS = 0b00000;	// Analog chanel select bits
   ADCON0bits.ADON = 1;		// ADC Enabled
   
   T0CONbits.TMR0ON = 1; 	// Enables Timer0
   T0CONbits.T08BIT = 0; 	// Configure Timer0 as a 16-bits timer
   T0CONbits.T0CS = 0;   	// Instruction cycle clock
   T0CONbits.PSA = 0;	 	// Prescaler is assigned
   T0CONbits.T0PS = 0b101;	// Timer0 prescaler select bits
   INTCONbits.GIE = 1;		// Enable Global interrupts
   INTCONbits.TMR0IE = 1;	// Enable Timer0
   INTCONbits.TMR0IF = 1;
}

void main(void) {
   configPIC();
   while (1);
}
```
