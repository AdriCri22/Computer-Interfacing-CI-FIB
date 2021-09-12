# Entrega Previa

```c
#include <xc.h>
#include <math.h>
#define _XTAL_FREQ 8000000

#define t 5

// the interrupt will vector immediately to address 0008h for high priority, or 0018h for low priority
/*
   External interrupts
   RB0/INT0
   RB1/INT1
   RB2/INT2
*/

int num[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	      /*0,    1,    2,    3,    4,    5,    6,    7,    8,    9*/
int error[4] = {0x79, 0x50, 0x50, 0x86};
int count;
int activate = 1;

void configPIC(){
   //config PIC
   ANSELA = 0x00;  // All pins as digital
   ANSELB = 0x00;  // All pins as digital
   ANSELD = 0x00;  // All pins as digital

   TRISA = 0x00;   // All pins outputs
   TRISB = 0x07;   // pins 0:2 inputs
   TRISD = 0x00;   // All pins outputs
   
   PORTA = 0x01;   // Set first 7-Segment enable
   
   RCONbits.IPEN = 1; //Enable priority levels
   
   INTCONbits.GIEH = 1; // Enable high priority interrupt
   INTCONbits.GIEL = 1; // Enable low-priprity interrupt
   INTCONbits.INT0IE = 1; // Enable INT0 external interrupt
   
   INTCON2bits.INTEDG0 = 0; // INT0 falling edge
   INTCON2bits.INTEDG1 = 1; // INTO rising edge
   INTCON2bits.INTEDG2 = 1; // INTO rising edge
   
   INTCON3bits.INT1IP = 0; // INT1 low priority
   INTCON3bits.INT2IP = 0; // INT2 low priority
   INTCON3bits.INT1IE = 1; // Enable INT1 external interrupt
   INTCON3bits.INT2IE = 1; // Enable INT2 external interrupt
}

void interrupt high(void) {
   if (INTCONbits.INT0IF && INTCONbits.INT0IE) {
      INTCONbits.INT0IF = 0;
      if (activate) {
	 INTCON3bits.INT1IE = 0; // Disable INT1 external interrupt
	 INTCON3bits.INT2IE = 0; // Disable INT2 external interrupt
	 activate = 0;
      }
      else {
	 INTCON3bits.INT1IE = 1; // Enable INT1 external interrupt
	 INTCON3bits.INT2IE = 1; // Enable INT2 external interrupt
	 INTCON3bits.INT1IF = 0; 
	 INTCON3bits.INT2IF = 0; 
	 activate = 1;
      }
   }
}

void interrupt low_priority low(void) {
   if (INTCON3bits.INT1IF && INTCON3bits.INT1IE) {			// Incrementa contador
      INTCON3bits.INT1IF = 0;
      count++;
   }
   
   if (INTCON3bits.INT2IF && INTCON3bits.INT2IE) {			// Contador a 0
      INTCON3bits.INT2IF = 0;
      count = 0x00;
   }  
}

void main(void){
   configPIC();
   count == 0x00;
   while(1) {
      for (int i = 0; i < 4; i++) {
	 PORTA = pow(2, 3 - i);
	 if (count > 9) PORTD = error[i];
	 else if (PORTA == 0x01) PORTD = num[count];
	 else PORTD = num[0];
	 __delay_ms(t);
      }
   }
}
```

# Entrega Lab

```c
#include "config.h"
#include <xc.h>
#include <math.h>
#define _XTAL_FREQ 8000000

#define t 5
#define deb 100

// the interrupt will vector immediately to address 0008h for high priority, or 0018h for low priority
/*
   External interrupts
   RB0/INT0
   RB1/INT1
   RB2/INT2
*/

int num[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	      /*0,    1,    2,    3,    4,    5,    6,    7,    8,    9*/
int error[4] = {0x79, 0x50, 0x50, 0x86};
int count;
int activate = 1;

void configPIC(){
   //config PIC
   ANSELA = 0x00;  // All pins as digital
   ANSELB = 0x00;  // All pins as digital
   ANSELD = 0x00;  // All pins as digital

   TRISA = 0x00;   // All pins outputs
   TRISB = 0x07;   // pins 0:2 inputs
   TRISD = 0x00;   // All pins outputs
   
   PORTA = 0x01;   // Set first 7-Segment enable
   
   RCONbits.IPEN = 1; //Enable priority levels
   
   INTCONbits.GIEH = 1; // Enable high priority interrupt
   INTCONbits.GIEL = 1; // Enable low-priprity interrupt
   INTCONbits.INT0IE = 1; // Enable INT0 external interrupt
   
   INTCON2bits.INTEDG0 = 0; // INT0 falling edge
   INTCON2bits.INTEDG1 = 1; // INTO rising edge
   INTCON2bits.INTEDG2 = 1; // INTO rising edge
   
   INTCON3bits.INT1IP = 0; // INT1 low priority
   INTCON3bits.INT2IP = 0; // INT2 low priority
   INTCON3bits.INT1IE = 1; // Enable INT1 external interrupt
   INTCON3bits.INT2IE = 1; // Enable INT2 external interrupt
}

void interrupt high(void) {
   __delay_ms(deb);
   if (INTCONbits.INT0IF && INTCONbits.INT0IE) {
      INTCONbits.INT0IF = 0;
      if (activate) {
	 INTCON3bits.INT1IE = 0; // Disable INT1 external interrupt
	 INTCON3bits.INT2IE = 0; // Disable INT2 external interrupt
	 activate = 0;
      }
      else {
	 INTCON3bits.INT1IE = 1; // Enable INT1 external interrupt
	 INTCON3bits.INT2IE = 1; // Enable INT2 external interrupt
	 INTCON3bits.INT1IF = 0; 
	 INTCON3bits.INT2IF = 0; 
	 activate = 1;
      }
   }
}

void interrupt low_priority low(void) {
   __delay_ms(deb);
   if (INTCON3bits.INT1IF && INTCON3bits.INT1IE) {			// Incrementa contador
      INTCON3bits.INT1IF = 0;
      count++;
   }
   
   if (INTCON3bits.INT2IF && INTCON3bits.INT2IE) {			// Contador a 0
      INTCON3bits.INT2IF = 0;
      count = 0x00;
   }  
}

void main(void){
   configPIC();
   count == 0x00;
   while(1) {
      for (int i = 0; i < 4; i++) {
	 PORTA = pow(2, 3 - i);
	 if (count > 9) PORTD = error[i];
	 else if (PORTA == 0x01) PORTD = num[count];
	 else PORTD = num[0];
	 __delay_ms(t);
      }
   }
}
```
