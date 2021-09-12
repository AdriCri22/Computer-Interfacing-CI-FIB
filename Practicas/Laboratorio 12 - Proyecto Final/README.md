# main.c

```c
#include <xc.h>
#define _XTAL_FREQ 8000000  

#include <string.h>
#include "config.h"
#include "GLCD.h"
#include "Splash.h"
#include "game.h"

#define delayH 0x20	// 0.0256 seconds
#define delayL 0x00

void interrupt highRSI(void) {
   // Interruption to move the player
   if (PIE1bits.TMR1IE && PIR1bits.TMR1IF) {
      active = 0;
      clearPlayer();
      if (moveLeft == 1) xPlayerPos--;
      else xPlayerPos++;
      printPlayer();
      moveLeft = 0; moveRight = 0;
      PIE1bits.TMR1IE = 0;
      PIR1bits.TMR1IF = 0;
   }
   
   // Interruption to move the pic
   if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
      if (existPic == 0) {	// If there is no Pic to catch
	 xPicPos = random_number(0, 120); // Generate a pic randomlly
	 yPicPos = 10;
	 printPic();
	 existPic = 1;
      } 
      
      else { // If exist a Pic to catch
	 clearPic();
	 yPicPos++;
	 // The Pic advances one position vertically 
	 printPic();
	 
	 // The pic touch the ground => decrease the live
	 if (yPicPos == 55) {
	    clearPic();
	    lives--;
	    print_lives();
	    existPic = 0;
	    if (lives == 0 && !gameOverPrinted) game_over();
	 } 
	 // The Player catch the pic => increase points
	 else if (yPicPos < 55 && yPicPos + 6 >= 47 && xPicPos + 4 >= xPlayerPos && xPicPos <= xPlayerPos + 7) {
	    points += sumPoints;
	    print_points();
	    printPlayer();
	    clearPic();
	    existPic = 0;
	    send_points();
	 }
      }
          
      TMR0 = 0x0000 - difficulty;
      INTCONbits.TMR0IF = 0;
   }
   
   // Interruption to hget de difficulty
   if (PIE1bits.ADIE && PIR1bits.ADIF) {
      int value = ADRESH & 0x03;	// Ignore bits 10:15
      value = value << 8;		// Move 8 bits to the left (shift left)
      value = value | ADRESL;		// Concantenate ADRESH and ADRESL
      difficulty = 1400 - value;	// Set difficulty 
      
      PIR1bits.ADIF = 0;
      ADCON0bits.GO = 1;
   }
   
   // Interruption to check if we receive an x to increment the live
   if (PIE1bits.RC1IE && PIR1bits.RC1IF) {
      if (RCREG1 == 'x') {
	 lives++;
	 print_lives();
      }
      
      PIR1bits.RC1IF = 0;
   }
}

// Configuration of the PIC and of the GLCD
void configGLCD() {
   ANSELD = 0;		// PORTD configured as Digital
   ANSELB = 0;          // PORTB configured as Digital
   PORTD = 0; 		// Set port values
   PORTB = 0;
   TRISD = 0x00;	// Configure B and D as Outputs
   TRISB = 0x00;
   
   GLCDinit();		  	// Initialize screen
   clearGLCD(0, 7, 0, 127);     // Clean screen
   setStartLine(0);           	// Set start line
}

// Interruption configuration
void configInterruptions() {
   RCONbits.IPEN = 0;		// Disable priority levels
   INTCONbits.GIE = 1;		// Enable Global interrupts
   INTCONbits.PEIE = 1;
   
   // TMR0 configuration
   T0CONbits.TMR0ON = 1; 	// Enables Timer0
   T0CONbits.T08BIT = 0; 	// Configure Timer0 as a 16-bits timer
   T0CONbits.T0CS = 0;   	// Instruction cycle clock
   T0CONbits.PSA = 0;	 	// Prescaler is assigned
   T0CONbits.T0PS = 0b110;	// prescaler = 128
   
   // TMR2 condiguration
   T1CONbits.TMR1CS = 0b01;	// clock source = FOSC
   T1CONbits.T1CKPS = 0b11;	// prescaler = 8
   T1CONbits.T1RD16 = 1;	// 16 bits timer
   T1CONbits.TMR1ON = 1;	// Timer 1 ON
   IPR1bits.TMR1IP = 1;		// High priority interruption
   PIE3bits.TMR1GIE = 0;	// Disable gate interrupt
}

// Transmition configuration
void configTransmition() {
   ANSELC = 0;		// PORTC configured as Digital
   PORTC = 0;		// Set port value
   TRISC = 0xFF;	// Configure C as Input
   
   TRISCbits.RC7 = 1;	// Configure RX1 pin for input
   TRISCbits.RC6 = 0;	// Configure TX1 pin for output
   
   TXSTA1bits.TX9 = 0;		// 8 bits transmission
   TXSTA1bits.TXEN = 1;		// Enable transmition
   TXSTA1bits.SYNC = 0;		// Asynchronous mode
   TXSTA1bits.BRGH = 1;		// Asynchronous mode: High speed
   PIE1bits.TXIE = 0; 		// Disable transmit interrupt
   
   RCSTA1bits.SPEN = 1;		// Enable USART port
   RCSTA1bits.RX9 = 0;		// 8-bits reception
   RCSTA1bits.CREN = 1;		// Enables receiver
   
   BAUDCON1bits.BRG16 = 1;	// 8-bit Baud Rate
   // SYNC = 0; BRG16 = 1; BRGH = 1; -> 16-bit/Asynchronous -> FSOC / [4 * (n + 1)]
   // FOSC = 8MHz; Desired boud rate = 115200; multiplier = 4
   // n = ((8MHz / 115200) / 4) - 1 = 16.361 -> n = 16 (Half Round Up)
   // Bound rate = 8MHz / (4 * (16 + 1)) = 117647.0588 -> error = (117647.0588 - 115200) / 115200 = 0.021242 = 2.1242%
   SPBRG1 = 16;
}

// Analog to Digital configuration
void ADConfig() {
   ADCON2bits.ADFM = 1;		// Conversion Result Format select bists: Left justified
   ADCON2bits.ACQT = 0b100;	// TAD = 64 / FOSC = 8 x 10^-6
   ADCON2bits.ADCS = 0b101;	// FOSC / 64
   ADCON1bits.PVCFG = 0b00;	// A/D Vref+ = VDD
   ADCON1bits.NVCFG = 0b00;	// A/D Vref- = VSS
   ADCON0bits.CHS = 0b00010;	// AN2
   ADCON0bits.ADON = 1;		// ADC Enabled 
   PIE1bits.ADIE = 1;		// Enable ADC
   ADCON0bits.GO = 1;
}

// Configure the Pic
void configPIC() {
   configGLCD();
   configInterruptions();
   configTransmition();
   ADConfig();   
   
   TRISA = 0xFF;
   ANSELA = 0x04;
}

void main(void) {
   configPIC();
   write_name();
   print_intro();
   
   start_game();
   
   while(1) {                
      if (PORTAbits.RA0 == 1 && xPlayerPos > 0 && !active) {
	 active = 1;
	 moveLeft = 1;
	 TMR1H = 0x00 - delayH;
	 TMR1L = 0x00 - delayL;
	 PIE1bits.TMR1IE = 1;
      }
      
      if (PORTAbits.RA1 == 1 && xPlayerPos + 9 < 127 && !active) {
	 active = 1;
	 moveRight = 1;
	 TMR1H = 0x00 - delayH;
	 TMR1L = 0x00 - delayL;
	 PIE1bits.TMR1IE = 1;
      }
   }
}
```

# game.h
```c
// Game parameters
#define pointsPage 0
#define pointsColumn 1
#define numPointsColumn 11
#define livesPage 0
#define livesColumn 13
#define numLivesColumn 22
#define yPlayerPos 6

#define iniPoints 0
#define iniLives 3
#define iniPlayerPos 60
#define sumPoints 1

int xPlayerPos = iniPlayerPos, xPicPos = 0, yPicPos = 0;
int moveLeft = 0, moveRight = 0, active = 0, existPic = 0, movePic = 0, gameOverPrinted = 0;
int lives = iniLives, points = iniPoints, difficulty = 377;
char name[] = "";

unsigned const char player[18] = {
   0x1C, 0x20, 0x4E, 0xD5, 0xF7, 0xD5, 0x4E, 0x20, 0x1C,
   0x80, 0xC0, 0xC0, 0xBF, 0x9F, 0xBF, 0xC0, 0xC0, 0x80
};


// Subrutine to output a character to USART1 using the polling method
void send(char c);

// Get character from virtual terminal
char receive();

// Get the name of the player
void write_name();

// Print the pic
void printPic();

// Clean the pic of the screen
void clearPic();

// Print the player
void printPlayer();

// Clean the player of the screen
void clearPlayer();

// Print splash 3 seconds
void print_intro();

// Print the score of the player in the screen
void print_points();

// Print the number of lives the player in the screen
void print_lives();

// Print the text in page p and column y of the screen
void print_text(unsigned char *txt, int p, int y);

// Send the number of points to virtual terminal
void send_points();

// The player lose, clean the screen and show a message and send it to virtual terminal as well
void game_over();

// Return a random number between min and max number
int random_number(int min, int max);

// Print all texts, initial points, pic and player in the screen
void start_game();
```

# game.c

```c
#include <xc.h>
#include <string.h>
#include "game.h"
#include "GLCD.h"
#include "Splash.h"

#define timeH 0xB7	// 3 seconds
#define timeL 0x1B

// Subrutine to output a character to USART1 using the polling method
void send(char c) {
   while (!PIR1bits.TX1IF);
      TXREG1 = c;
}

// Get character from virtual terminal
char receive() {
   while (!PIR1bits.RC1IF);
      return RCREG1;
}

// Get the name of the player
void write_name() {
   char text_name[] = "Escriba su apodo: ", welcome_text[] = "Bienvenido ";
   for (int i = 0; i < strlen(text_name); i++) { // Tell to the player to write the name
      send(text_name[i]);
   }

   // Save the name
   char c = receive();
   while (c != 0x0D) {
      strcat(name, &c);
      c = receive();
   }

   //Welcoming the player
   for (int i = 0; i < strlen(welcome_text); i++) send(welcome_text[i]);
   for (int i = 0; i < strlen(name); i++) {
      c = name[i];
      send(c);
   }

   char c = '\n';
   strcat(name, &c);
}

// Print the pic
void printPic() {
   for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 7; j++) {
	 if ((i == 0 || i == 4) && j % 2 != 0) SetDot(j + yPicPos, i + xPicPos);
	 else if (i != 0 && i != 4) SetDot(j + yPicPos, i + xPicPos);
      }
   }
}

// Clean the pic of the screen
void clearPic() {
   for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 7; j++) {
	 ClearDot(j + yPicPos, i + xPicPos);
      }
   }
}

// Print the player
void printPlayer() {
   for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 9; j++) {
	 writeByte(yPlayerPos + i, xPlayerPos + j, player[i * 9 + j]);
      }
   }
}

// Clean the player of the screen
void clearPlayer() {
   for (int i = yPlayerPos; i < 8; i++) {
      for (int j = xPlayerPos; j < xPlayerPos + 9; j++) {
	 if (i == 7) writeByte(i, j, 0x80);
	 else writeByte(i, j, 0);
      }
   }
}

// Print splash
void print_intro() {
   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 128; j++) {
	 writeByte(i, j, bitmap[i*128 + j]);
      }
   }
   TMR0H = 0x00 - timeH;	// 0x0000 - 0xB71B
   TMR0L = 0x00 - timeL;
   INTCONbits.TMR0IF = 0;
   while(!INTCONbits.TMR0IF);
   clearGLCD(0, 7, 0, 127);	// Clean screen
}


// Print the score of the player
void print_points() {
   int n = points;
   if (n == 0) writeNum(pointsPage, numPointsColumn, 0);
   for (int i = 0; n != 0; i++) {
      writeNum(pointsPage, numPointsColumn - i, n % 10);
      n /= 10;
   }
}

// Print the number of lives the player
void print_lives() {

   // Print a x that represents the multiplication symbol
   putchGLCD(livesPage, numLivesColumn + 1, 'x');

   // Print the heart
   putchGLCD(livesPage, numLivesColumn + 2, '#');

   int n = lives;
   if (n == 0) writeNum(livesPage, numLivesColumn, 0);
   int lastDigit = 1;
   for (int i = 0; n != 0; i++) {	// Print the number of lives
      writeNum(livesPage, numLivesColumn - i, n % 10);
      n /= 10;
      lastDigit = i + 1;
   }
}

// Print the text in page p and column y of the screen
void print_text(unsigned char *txt, int p, int y) {
   for (int i = 0; txt[i] != '\n'; i++) {
      putchGLCD(p, y + i, txt[i]);
   }
}

// Send the points to virtual terminal
void send_points() {
   char points_text[] = "Points: \n";
   TXSTA1bits.TXEN = 1;	// Enables transmition
   send(0x0D);
   for (int i = 0; i < strlen(points_text); i++) send(points_text[i]);

   // Send the reverse number
   int size = 0;
   for (int n = points; n != 0; n /= 10) size++;
   int n = points;
   while (size != 0) {
      int pow = 1;
      for (int i = 1; i < size; i++) pow *= 10;
      send(n / pow + '0');
      n = n % pow;
      size--;
   }

   TXSTA1bits.TXEN = 0;	// Disbales transmition
}

// The player lose
void game_over() {
   char lose_text[] = "Has perdido \n";
   PIE1bits.TMR1IE = 0;		// Disable all interruptions
   INTCONbits.TMR0IE = 0;
   active = 1;
   clearGLCD(0, 7, 0, 127);	// Clean Screen
   print_text(lose_text, 2, 8);	// Print that the game ends
   print_text(name, 3, 10);
   gameOverPrinted = 1;

   // Send to device that the game ends
   TXSTA1bits.TXEN = 1;	// Enables transmition
   send(0x0D);
   for (int i = 0; i < strlen(lose_text); i++) send(lose_text[i]);
   for (int i = 0; i < strlen(name); i++) send(name[i]);
   RCSTA1bits.CREN = 0;	// Disbales receiver
}

// Return a random number between min and max number
int random_number(int min, int max) {
   return (rand() % (max - min + 1)) + min;
}

// Print all texts, initial points, pic and player in the screen
void start_game() {
   print_text((unsigned char*) "Points:\n", pointsPage, pointsColumn); 	// Print Points text
   print_text((unsigned char*) "Lives:\n", livesPage, livesColumn);	// Print Lives text
   print_lives();	// Print initial lives
   print_points();	// Print start points	
   for (int i = 0; i < 128; i++) SetDot(63, i);	// Print ground
   printPlayer();	// Print player on the initial position   
   
   TMR0 = 0x0000 - difficulty;
   INTCONbits.TMR0IE = 1;
   INTCONbits.TMR0IF = 1;
}
```
