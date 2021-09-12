# Entrega Previa

```c
#include <xc.h>
#define _XTAL_FREQ 8000000  

#include <string.h>
#include "config.h"
#include "GLCD.h"

#define INIT 0
#define RUNNING 1
#define STOPPED 2
#define time 10010400
#define timeH 0x0C	// 3125
#define timeL 0x35

long counter = time;
int status = INIT;
int v[7];

void print_timer() {
   int j = 0;
   for (int i = 0; i < 10; i++) {
      if (i == 1) putchGLCD(6, 10 - i, '.');
      else if (i == 4 || i == 7) putchGLCD(6, 10 - i, ':');
      else {
	 putchGLCD(6, 10 - i, v[j] + '0');
	 j++;
      }
   }
}

void tic() {
   // Tenths of a second to hours, minutes and seconds
   v[0] = counter % 10;
   int k = counter / 10;
   for (int i = 1; i < 7; i++) {
      v[i] = k % 10;
      i++;
      v[i] = k / 10 % 6;
      k = k / 60;
   }
   print_timer();
   
   counter--;
   
}

void interrupt highRSI(void) {
   if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
      tic();
      TMR0H = 0x00 - timeH;	// 0x0000 - 3125
      TMR0L = 0x00 - timeL;
      INTCONbits.TMR0IF = 0;
      if (counter <= 0) {		// Counter ends
	 status = STOPPED;
	 tic();
	 INTCONbits.TMR0IE = 0;	// Disable Timer0
	 INTCONbits.TMR0IF = 0;
      }
   }
} 

void configPIC() {
   ANSELD = 0;		// PORTD configured as Digital
   ANSELB = 0;          // PORTB configured as Digital
   ANSELC = 0;
   PORTD = 0; 		// Set port values
   PORTB = 0;
   PORTC = 0;
   TRISD = 0x00;	// Configure B and D as Outputs
   TRISB = 0x00;
   TRISC = 0xFF;	// Configure C as Input
   
   T0CONbits.TMR0ON = 1; 	// Enables Timer0
   T0CONbits.T08BIT = 0; 	// Configure Timer0 as a 16-bits timer
   T0CONbits.T0CS = 0;   	// Instruction cycle clock
   T0CONbits.PSA = 0;	 	// Prescaler is assigned
   T0CONbits.T0PS = 0b101;	// Timer0 prescaler select bits
   INTCONbits.GIE = 1;		// Enable Global interrupts
   
   GLCDinit();		  	//Inicialitzem la pantalla
   clearGLCD(0,7,0,127);      	//Esborrem pantalla
   setStartLine(0);           	//Definim linia d'inici
}

void main(void) {
   configPIC();
   tic();
   int lastRC0, running = 0;
   while (1) {
      if (PORTCbits.RC0 == 0) lastRC0 = 1;
      if (PORTCbits.RC0 == 1 && lastRC0 == 1) { // Rising Edge
	 lastRC0 = 0;
	 if (status == INIT) {		// INIT -> RUNNING
	    status = RUNNING;
	    INTCONbits.TMR0IE = 1;	// Enable Timer0
	    INTCONbits.TMR0IF = 1;
	 } else if (status == RUNNING) { // RUNNING -> STOPPED
	    status = STOPPED;
	    INTCONbits.TMR0IE = 0;	// Disable Timer0
	    INTCONbits.TMR0IF = 0;
	 } else {			// STOPPED -> INIT
	    status = INIT;
	    counter = time;
	    tic();
	 }
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

#define CRON 0
#define TEMP 1
#define CLOCK 2
#define SELECT 2
#define time 10010400
#define timeH 0x0C	// 3125
#define timeL 0x35

long counter = 0;
int clockTime = 666, timerDec = 0;
int v[7];
int status = 0;
int posSel = 0;

void print_timer() {
   int j = 0;
   for (int i = 0; i < 10; i++) {
      if (i == 1) putchGLCD(6, 10 - i, '.');
      else if (i == 4 || i == 7) putchGLCD(6, 10 - i, ':');
      else {
	 putchGLCD(6, 10 - i, v[j] + '0');
	 j++;
      }
   }
}

void cronometro() {
   // Tenths of a second to hours, minutes and seconds
   v[0] = counter % 10;
   int k = counter / 10;
   for (int i = 1; i < 7; i++) {
      v[i] = k % 10;
      i++;
      v[i] = k / 10 % 6;
      k = k / 60;
   }
   print_timer();
   
   counter++;
}

void temporizador() {
   // Tenths of a second to hours, minutes and seconds
   v[0] = counter % 10;
   int k = counter / 10;
   for (int i = 1; i < 7; i++) {
      v[i] = k % 10;
      i++;
      v[i] = k / 10 % 6;
      k = k / 60;
   }
   print_timer();
   
   counter--;
   if (counter < 1) {		// Counter ends
      print_timer();
      INTCONbits.TMR0IE = 0;	// Disable Timer0
      INTCONbits.TMR0IF = 0;
   }
}

void clock() {
   putchGLCD(6, 10, (clockTime % 10) + '0');
   putchGLCD(6, 9, (clockTime / 10 % 6) + '0');
   putchGLCD(6, 8, ':');
   putchGLCD(6, 7, (clockTime / 60 % 10) + '0');
   putchGLCD(6, 6, (clockTime / 600 % 6) + '0');
}

void interrupt highRSI(void) {
   if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
      if (status == CRON) cronometro();
      else if (status == TEMP) temporizador();
      else if (status == CLOCK) {
	 clock();
	 if (timerDec == 600) {
	    clockTime++;
	    timerDec == 0;
	 } 
	 timerDec++;
      }
      TMR0H = 0x00 - timeH;	// 0x0000 - 3125
      TMR0L = 0x00 - timeL;
      INTCONbits.TMR0IF = 0;
      
   }
} 

void print_clock() {
   int j = 0;
   for (int i = 0; i < 5; i++) {
      if (i == 2) putchGLCD(6, 10 - i, ':');
      else {
	 putchGLCD(6, 10 - i, v[j] + '0');
	 j++;
      }
   }
}

void configPIC() {
   ANSELD = 0;		// PORTD configured as Digital
   ANSELB = 0;          // PORTB configured as Digital
   ANSELA = 0;
   PORTD = 0; 		// Set port values
   PORTB = 0;
   PORTA = 0;
   TRISD = 0x00;	// Configure B and D as Outputs
   TRISB = 0x00;
   TRISA = 0xFF;	// Configure C as Input
   
   T0CONbits.TMR0ON = 1; 	// Enables Timer0
   T0CONbits.T08BIT = 0; 	// Configure Timer0 as a 16-bits timer
   T0CONbits.T0CS = 0;   	// Instruction cycle clock
   T0CONbits.PSA = 0;	 	// Prescaler is assigned
   T0CONbits.T0PS = 0b101;	// Timer0 prescaler select bits
   INTCONbits.GIE = 1;		// Enable Global interrupts
   
   GLCDinit();		  	//Inicialitzem la pantalla
   clearGLCD(0,7,0,127);      	//Esborrem pantalla
   setStartLine(0);           	//Definim linia d'inici
}

void main(void) {
   configPIC();
   int actSel = 0;
   int lastRA0, lastRA1, lastRA2, lastRA3;
   while (1) {
      if (PORTAbits.RA0 == 0) lastRA0 = 1;
      if (PORTAbits.RA1 == 0) lastRA1 = 1;
      if (PORTAbits.RA2 == 0) lastRA2 = 1;
      if (PORTAbits.RA3 == 0) lastRA3 = 1;
      if (PORTAbits.RA0 == 1 && lastRA0 == 1 && !actSel) { // Cron mode
	 lastRA0 = 0;
	 status = CRON;
	 counter = 0;
	 INTCONbits.TMR0IE = 1;	// Enable Timer0
	 INTCONbits.TMR0IF = 1;	
      } else if (PORTAbits.RA1 == 1 && lastRA1 == 1 && !actSel) { // Temp mode
	 status = TEMP;
	 lastRA1 = 0;
	 counter = time;
	 INTCONbits.TMR0IE = 1;	// Enable Timer0
	 INTCONbits.TMR0IF = 1;	
      } else if (PORTAbits.RA2 == 1 && lastRA2 == 1 && !actSel) { // Clock mode
	 lastRA2 = 0;
	 status = CLOCK;
	 clearGLCD(0,7,0,127);      	//Esborrem pantalla
	 INTCONbits.TMR0IE = 1;	// Enable Timer0
	 INTCONbits.TMR0IF = 1;
      } else if (PORTAbits.RA3 == 1 && lastRA3 == 1 && !actSel) { // Select mode
	 lastRA3 = 0;
	 if (status != SELECT) status = SELECT;
	 actSel = 1;
	 print_clock();
      } else if (PORTAbits.RA3 == 1 && lastRA3 == 1 && actSel) { // Select mode position++
	 lastRA3 = 0;
	 if (status != SELECT) status = SELECT;
	 posSel++;
	 if (posSel == 4) actSel = 0;
	 print_clock();
      } else if (PORTAbits.RA0 == 1 && lastRA0== 1 && actSel) { // Select Up
	 lastRA0 = 0;
	 if (posSel == 0 && v[posSel] >= 0 && v[posSel] < 9) v[posSel]++; // Que no sobrepase tiempos
	 if (posSel == 2 && v[posSel] >= 0 && v[posSel] < 9) v[posSel]++;
	 if (posSel == 1 && v[posSel] >= 0 && v[posSel] < 5) v[posSel]++;
	 if (posSel == 3 && v[posSel] >= 0 && v[posSel] < 5) v[posSel]++;
	 print_clock();
      } else if (PORTAbits.RA1 == 1 && lastRA1== 1 && actSel) { // Select Down
	 lastRA1 = 0;
	 if (posSel == 0 && v[posSel] > 0 && v[posSel] < 10) v[posSel]--; // Que no sobrepase tiempos
	 if (posSel == 2 && v[posSel] > 0 && v[posSel] < 10) v[posSel]--;
	 if (posSel == 1 && v[posSel] > 0 && v[posSel] < 6) v[posSel]--;
	 if (posSel == 3 && v[posSel] > 0 && v[posSel] < 6) v[posSel]--;
	 print_clock();
      }
   }
}
```
