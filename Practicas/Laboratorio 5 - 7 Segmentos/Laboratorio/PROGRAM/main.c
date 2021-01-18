/* Main.c file generated by New Project wizard
 *
 * Created:   mi. oct. 21 2020
 * Processor: PIC18F45K22
 * Compiler:  MPLAB XC8
 */

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