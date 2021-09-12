# Entregas Previas

## Programa 1

```c
#include <xc.h>
#define _XTAL_FREQ 8000000

#define PINA0 PORTAbits.RA0
#define tP1 300

void configPIC() {
  //config PIC
  ANSELA = 0x00;  // All pins as digital
  ANSELD = 0x00;  // All pins as digital

  TRISA = 0x00;
  TRISD = 0x00;
}

void main(void) {
  configPIC();
  while(1) {
    PORTA = 0x01;
    for (int i = 0; i < 10; i++) {
      if (i == 0) PORTD = 0x3F;
      else if (i == 1) PORTD = 0x06;
      else if (i == 2) PORTD = 0x5B;
      else if (i == 3) PORTD = 0x4F;
      else if (i == 4) PORTD = 0x66;
      else if (i == 5) PORTD = 0x6D;
      else if (i == 6) PORTD = 0x7D;
      else if (i == 7) PORTD = 0x07;
      else if (i == 8) PORTD = 0x7F;
      else if (i == 9) PORTD = 0x6F;
      __delay_ms(tP1);
    }      
  }
}
```

## Programa 2

```c
#include <xc.h>
#define _XTAL_FREQ 8000000

#define PINA0 PORTAbits.RA0
#define PINA1 PORTAbits.RA1
#define PINA2 PORTAbits.RA2
#define PINA3 PORTAbits.RA3
#define PIND7 PORTDbits.RD7
#define t 5

void configPIC() {
   //config PIC
   ANSELA = 0x00;  // All pins as digital
   ANSELD = 0x00;  // All pins as digital

   TRISA = 0x00;
   TRISD = 0x00;
   
   PORTA = 0x00;
   PORTD = 0x00;
}

void displayValue(int display, int value, int decimal) {
   if (display == 0) PINA0 = 1;
   else if (display == 1) PINA1 = 1;
   else if (display == 2) PINA2 = 1;
   else if (display == 3) PINA3 = 1;
   
   if (value == 0) PORTD = 0x3F;
   else if (value == 1) PORTD = 0x06;
   else if (value == 2) PORTD = 0x5B;
   else if (value == 3) PORTD = 0x4F;
   else if (value == 4) PORTD = 0x66;
   else if (value == 5) PORTD = 0x6D;
   else if (value == 6) PORTD = 0x7D;
   else if (value == 7) PORTD = 0x07;
   else if (value == 8) PORTD = 0x7F;
   else if (value == 9) PORTD = 0x6F;
      
   if (decimal == 1) PIND7 = 1;
   __delay_ms(t);
}

void func(int val, int dec) {
   for (int i = 0; i < 4; i++) {
      if (i == dec) displayValue(i, val % 10, 1);
      else displayValue(i, val % 10, 0);
      PORTA = 0x0;
      val = val / 10;
   }
}

void main(void) {
   configPIC();
   while (1) {
      func(267, 2);
   }
}
```

## Programa 3

```c
#include <xc.h>
#define _XTAL_FREQ 8000000

#define PINA0 PORTAbits.RA0
#define PINA1 PORTAbits.RA1
#define PINA2 PORTAbits.RA2
#define PINA3 PORTAbits.RA3
#define PIND7 PORTDbits.RD7
#define PINC1 PORTCbits.RC1
#define tP1 300
#define tP2 5


void configPIC() {
   //config PIC
   ANSELA = 0x00;  // All pins as digital
   ANSELC = 0x00;  // All pins as digital
   ANSELD = 0x00;  // All pins as digital

   TRISA = 0x00;   // Output
   TRISC = 0x02;   // Input
   TRISD = 0x00;   // Output
}

void displayValue(int display, int value, int decimal) {
   if (display == 0) PINA0 = 1;
   else if (display == 1) PINA1 = 1;
   else if (display == 2) PINA2 = 1;
   else if (display == 3) PINA3 = 1;
   
   if (value == 0) PORTD = 0x3F;
   else if (value == 1) PORTD = 0x06;
   else if (value == 2) PORTD = 0x5B;
   else if (value == 3) PORTD = 0x4F;
   else if (value == 4) PORTD = 0x66;
   else if (value == 5) PORTD = 0x6D;
   else if (value == 6) PORTD = 0x7D;
   else if (value == 7) PORTD = 0x07;
   else if (value == 8) PORTD = 0x7F;
   else if (value == 9) PORTD = 0x6F;
      
   if (decimal == 1) PIND7 = 1;
   __delay_ms(tP2);
}

void func(int val, int dec) {
   for (int i = 0; i < 4; i++) {
      if (i == dec) displayValue(i, val % 10, 1);
      else displayValue(i, val % 10, 0);
      PORTA = 0x00;
      val = val / 10;
   }
}

void programa2() {
   PORTA = 0x00;
   func(267, 2);
}

void programa1() {
   PORTA = 0x01;
   for (int i = 0; i < 10; i++) {
      if (i == 0) PORTD = 0x3F;
      else if (i == 1) PORTD = 0x06;
      else if (i == 2) PORTD = 0x5B;
      else if (i == 3) PORTD = 0x4F;
      else if (i == 4) PORTD = 0x66;
      else if (i == 5) PORTD = 0x6D;
      else if (i == 6) PORTD = 0x7D;
      else if (i == 7) PORTD = 0x07;
      else if (i == 8) PORTD = 0x7F;
      else if (i == 9) PORTD = 0x6F;
      __delay_ms(tP1);
   }
}

void main(void) {
   configPIC();
   while(1) {
      if (PINC1 == 1) programa1();
      else programa2();
   }
}
```

# Entregas Lab

## Programa 3

```c
#include <xc.h>
#include "config.h"

#define _XTAL_FREQ 8000000

#define PINA0 PORTAbits.RA0
#define PINA1 PORTAbits.RA1
#define PINA2 PORTAbits.RA2
#define PINA3 PORTAbits.RA3
#define PIND7 PORTDbits.RD7
#define PINC1 PORTCbits.RC1
#define tP1 150
#define tP2 5


void configPIC(){
   //config PIC
   ANSELA = 0x00;  // All pins as digital
   ANSELC = 0x00;  // All pins as digital
   ANSELD = 0x00;  // All pins as digital

   TRISA = 0x00;   // Output
   TRISC = 0x02;   // Input
   TRISD = 0x00;   // Output
}

void displayValue(int display, int value, int decimal) {
   if (display == 0) PINA0 = 1;
   else if (display == 1) PINA1 = 1;
   else if (display == 2) PINA2 = 1;
   else if (display == 3) PINA3 = 1;
   
   if (value == 0) PORTD = 0x3F;
   else if (value == 1) PORTD = 0x06;
   else if (value == 2) PORTD = 0x5B;
   else if (value == 3) PORTD = 0x4F;
   else if (value == 4) PORTD = 0x66;
   else if (value == 5) PORTD = 0x6D;
   else if (value == 6) PORTD = 0x7D;
   else if (value == 7) PORTD = 0x07;
   else if (value == 8) PORTD = 0x7F;
   else if (value == 9) PORTD = 0x6F;
      
   if (decimal == 1) PIND7 = 1;
   __delay_ms(tP2);
}

void func(int val, int dec) {
   for (int i = 0; i < 4; i++) {
      if (i == dec) displayValue(i, val % 10, 1);
      else displayValue(i, val % 10, 0);
      PORTA = 0x00;
      val = val / 10;
   }
}

void programa2() {
   PORTA = 0x00;
   func(267, 2);
}

void programa1() {
   PORTA = 0x02;
   for (int i = 0; i < 10 && PINC1 == 1; i++) {
      if (i == 0) PORTD = 0x3F;
      else if (i == 1) PORTD = 0x06;
      else if (i == 2) PORTD = 0x5B;
      else if (i == 3) PORTD = 0x4F;
      else if (i == 4) PORTD = 0x66;
      else if (i == 5) PORTD = 0x6D;
      else if (i == 6) PORTD = 0x7D;
      else if (i == 7) PORTD = 0x07;
      else if (i == 8) PORTD = 0x7F;
      else if (i == 9) PORTD = 0x6F;
      __delay_ms(tP1);
   }
}

void main(void){
   configPIC();
   while(1) {
      if (PINC1 == 1) programa1();
      else programa2();
   }
}
```

## Lectura string

```c
#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 8000000
#define n 9
#define t1 100
#define t2 15

void configPIC(){
   //config PIC
   ANSELA = 0x00;  // All pins as digital
   ANSELC = 0x00;  // All pins as digital
   ANSELD = 0x00;  // All pins as digital

   TRISA = 0x00;   // Output
   TRISC = 0x02;   // Input
   TRISD = 0x00;   // Output
}

void print_char(char c, int seg) {
   PORTA = seg;
   int numbers[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; // Numbers
   int capital_letters[26] ={0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x30, // Capital letters
      0x1E, 0x76, 0x38, 0x76, 0x54, 0x3F, 0x73, 0x67, 0x50, 0x6D,0x78, 0x3E, 
      0x3E, 0x3E, 0x76, 0x6E, 0x5B};
   int lower_letters[26] = {0x77, 0x7C, 0x58, 0x5E, 0x79, 0x71, 0x3D, 0x74, 0x30, // Lower case letters
      0x1E, 0x76, 0x30, 0x76, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D, 0x78, 
      0x1C, 0x3E, 0x3E, 0x76, 0x6E, 0x5B};

   if ('0' <= c && c <= '9') PORTD = numbers[c - '0'];
   else if ('A' <= c && c <= 'Z') PORTD = capital_letters[c - 'A'];
   else if ('a' <= c && c <= 'z') PORTD = lower_letters[c - 'a'];
   else PORTD = 0x00;
   __delay_ms(t2);
}



void main(void) {
   configPIC();
   // Write your code here
   while (1) {
      PORTA = 0x01;
      char txt[n] = {'A','d', 'r', 'i', 'a', 'n', ' ', '1', '9'};
      for (int i = 0; i < n; i++) {
         if (i == 0) print_char(txt[i], 0x01);
         else if (i == 1) {
            print_char(txt[i-1], 0x02);
            print_char(txt[i], 0x01);
         } else if (i == 2) {
            print_char(txt[i-2], 0x04);
            print_char(txt[i-1], 0x02);
            print_char(txt[i], 0x01);
         } else {
            print_char(txt[i-3], 0x08);
            print_char(txt[i-2], 0x04);
            print_char(txt[i-1], 0x02);
            print_char(txt[i], 0x01);
         }
         __delay_ms(t1);
      }
   }
 }
```
