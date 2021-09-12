```c
#include <xc.h>
#define _XTAL_FREQ 8000000  

#include <string.h>
#include "config.h"
#include "GLCD.h"
#include "numbers.h"

#define base 3125 // 0.1 seconds of delay

// incSec bool that interprets if the program increases the seconds or decreases them
// started bool that interprets whether the program started the counter
int incSec, started = 0;

// Print the number n in position y (horizontal position)
void printNum(int n, int y) {
   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 32; j++) {
	 writeByte(i, y + j, num[n][j][i]);
      }
   }
}

void interrupt print(void) {
   int sec;
   if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
      
      if (incSec && !started) {sec = 0; started = 1;}		// Increasing counter
      else if (!incSec && !started) {sec = 9; started = 1;}	// Decreasing counter
      
      if (incSec) {
	 printNum(sec, 48);
	 TMR0 = 0xFFFF - 10*base;	// Set TMR0 to this value so it overflows in 1.0 second
	 INTCONbits.TMR0IF = 0;
	 sec++;
	 if (9 < sec) {
	    INTCONbits.TMR0IE = 0;	// Disable Timer0
	    INTCONbits.TMR0IF = 0;
	    started = 0;
	 }
      } else {
	 printNum(sec, 48);
	 TMR0 = 0xFFFF - 10*base;	// Set TMR0 to this value so it overflows in 1.0 second
	 INTCONbits.TMR0IF = 0;
	 sec--;
	 if (sec < 0) {
	    INTCONbits.TMR0IE = 0;	// Disable Timer0
	    INTCONbits.TMR0IF = 0;
	    started = 0;
	 }
      }
   }
}

// PIC18 configuration
void configPIC() {
   ANSELD=0;		// PORTD configured as Digital
   ANSELB=0;  		// PORTB configured as Digital
   PORTD=0; 		// Set port values
   PORTB=0;  
   TRISD=0x00;		// Configure B and D as Outputs
   TRISB=0x00;
   ANSELC = 0x00;
   TRISC = 0xFF;
   
   T0CONbits.TMR0ON = 1; 	// Enables Timer0
   T0CONbits.T08BIT = 0; 	// Configure Timer0 as a 16-bits timer
   T0CONbits.T0CS = 0;   	// Instruction cycle clock
   T0CONbits.PSA = 0;	 	// Prescaler is assigned
   T0CONbits.T0PS = 0b101;	// Timer0 prescaler select bits
   INTCONbits.GIE = 1;		// Enable Global interrupts
   
   GLCDinit();			// Initialize GLCD
   clearGLCD(0,7,0,127);      	// Clean screen
   setStartLine(0);           	// Define start line
}

void main(void) {
   configPIC();
   int lastRC0, lastRC1, lastRC2;
   while (1) {
      if (PORTCbits.RC0 == 0) lastRC0 = 1;
      if (PORTCbits.RC1 == 0) lastRC1 = 1;
      if (PORTCbits.RC2 == 1) lastRC2 = 1;
      
      if (PORTCbits.RC0 == 1 && lastRC0 == 1) { // Rising Edge
	 lastRC0 = 0;
	 clearGLCD(0, 7, 0, 127);
	 incSec = 1;
	 if (started) started = 0;	// Restart counter if this started and the button is pressed
	 INTCONbits.TMR0IE = 1;		// Enable Timer0	 
      }
      
      if (PORTCbits.RC1 == 1 && lastRC1 == 1) { // Rising Edge
	 lastRC1 = 0;
	 clearGLCD(0, 7, 0, 127);
	 incSec = 0;
	 if (started) started = 0;	// Restart counter if this started and the button is pressed
	 INTCONbits.TMR0IE = 1;		// Enable Timer0
      }
      
      if (PORTCbits.RC2 == 0 && lastRC2 == 1) { // Falling Edge
	 lastRC2 = 0;
	 INTCONbits.TMR0IE = 0;	// Disable Timer0 
	 int p = 0;
	 for (int k = 0; k < 3; k++) {
	    printNum(k, p);
	    p = p + 48;
	 }
      }
   }
}
```