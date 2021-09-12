# Entrega Previa

## Fase 1

```c
#include <xc.h>
#include "config.h"

// Configuración del PIC18F45k22
void configPic () {
   TRISCbits.RC2 = 0;		// Configuramos RC2 como output

   T2CONbits.T2OUTPS = 0b0000;	// Postscaler a 0
   T2CONbits.TMR2ON = 1;	// Timer 2 operativo
   T2CONbits.T2CKPS = 0b11;	// Prescaler a 4

   CCPTMRS0bits.C1TSEL = 0b00;	// CCP1 corresponde al puerto RC2 -> configuramos CCP1 al timer 2

   CCP1CONbits.DC1B = 0b00;	// Configuración de los 2 bits extra que nos ofrecen los fabricantes para más resolución
   CCP1CONbits.CCP1M = 0b1100;	// CCP1 configurado con el modo PWM

   // Subgrupo 13 A posición 44 -> duty_cycle = 20% (un 20% del tiempo está en flanco ascendente)
   // 1 ms / (0.5 us * 16 prescaler) = 125 tics cada ciclo ascendente
   PR2 = 124;	// Flanco ascendente    PR2 = PR2 + 1 = 125
   CCPR1L = 38;	// Flanco descendente
}

void main(void) {
   configPic();
   while (1);
 }
```

https://github.com/AdriCri22/Computer-Interfacing-CI-FIB/blob/main/Practicas/Laboratorio%2010%20-%20PWM/Previo/fase_1.png

## Fase 2

```c
#include <xc.h>
#include "config.h"

int subida = 1, duty_cycle = 0, count = 0;

int v[101];

void setPercentage() {
   for (int i = 0; i <= 100; i++) {
      v[i] = 1.25 * i;
   }
}

void interrupt highRSI(void) {
   if (PIE1bits.TMR2IE && PIR1bits.TMR2IF) {
      count++;
      if (count == 8) {	// Cada 8 ms
	 count = 0;
	 CCPR1L = v[duty_cycle];

	 if (subida) {
	    duty_cycle++;
	    if (duty_cycle == 100) subida = 0;
	 } else {
	    duty_cycle--;
	    if (duty_cycle == 0) subida = 1;
	 }
      }

      PIR1bits.TMR2IF = 0;
   }
}

// Configuración del PIC18F45k22
void configPic () {
   TRISCbits.RC2 = 0;		// Configuramos RC2 como output

   T2CONbits.T2OUTPS = 0b0000;	// Postscaler a 0
   T2CONbits.TMR2ON = 1;	// Timer 2 operativo
   T2CONbits.T2CKPS = 0b11;	// Prescaler a 16

   CCPTMRS0bits.C1TSEL = 0b00;	// CCP1 corresponde al puerto RC2 -> configuramos CCP1 al timer 2

   CCP1CONbits.DC1B = 0b00;	// Configuración de los 2 bits extra que nos ofrecen los fabricantes para más resolución
   CCP1CONbits.CCP1M = 0b1100;	// CCP1 configurado con el modo PWM

   INTCONbits.GIE = 1;	// Habilitamos las interrupciones globales
   PIE1bits.TMR2IE = 1;	// Habilitamos las interrupciones TMR2
   INTCONbits.PEIE = 1;

   // 1 ms / (0.5 us * 16 prescaler) = 125 tics
   PR2 = 124; //PR2 = 124 + 1 = 125
   CCPR1L = v[duty_cycle];
}

void main(void) {
   setPercentage();
   configPic();
   while (1) ;
 }
```



# Entrega Lab

```c

```
