
    //-----------------------------------------//
    // ECE 372A Spring 2016
    // Lab Group 201:
    // Zachary Finsterwald
    // Zakir Mukhida
    // Jimmy Lacey
    // Raun Rongguo
    //-----------------------------------------//

#include <xc.h>
#include "lcd.h"
#include "timer.h"
#include <string.h>

/* The pins that you will use for the lcd control will be
 * LCD_RS      RC4
 * LCD_E       RC2
 * LCD_D4      RE7
 * LCD_D5      RE5
 * LCD_D6      RE3
 * LCD_D7      RE1
 * Use these so that we will be able to test your code for grading and to
 * help you debug your implementation!
 */

#define TRIS_D7 TRISEbits.TRISE1
#define TRIS_D6 TRISEbits.TRISE3
#define TRIS_D5 TRISEbits.TRISE5
#define TRIS_D4 TRISEbits.TRISE7

#define TRIS_RS TRISCbits.TRISC4
#define TRIS_E  TRISCbits.TRISC2

#define LAT_D7 LATEbits.LATE1
#define LAT_D6 LATEbits.LATE3
#define LAT_D5 LATEbits.LATE5
#define LAT_D4 LATEbits.LATE7

#define LAT_RS LATCbits.LATC4
#define LAT_E  LATCbits.LATC2

#define OUTPUT  0

#define true 1
#define false 0

void toggleEnable(unsigned int delay)
{
    LAT_E = 1;
    delay_us(500);
    LAT_E = 0;
    delay_us(delay);
}

void setData(int db7, int db6, int db5, int db4)
{
    LAT_D7 = db7;
    LAT_D6 = db6;
    LAT_D5 = db5;
    LAT_D4 = db4;
}

/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATE. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int command_type, unsigned int delay_after, unsigned int lower){

    LAT_RS  = command_type;  // SET RS

    setData(0,0,0,0);       // Make pins zero
    if(lower == true) {
        LAT_D4  =  word&0x01;
        LAT_D5  = (word>>1)&0x01;
        LAT_D6  = (word>>2)&0x01;
        LAT_D7  = (word>>3)&0x01;
    } else {
        LAT_D4  = (word>>4)&0x01;
        LAT_D5  = (word>>5)&0x01;
        LAT_D6  = (word>>6)&0x01;
        LAT_D7  = (word>>7)&0x01;
    }
    toggleEnable(delay_after);
}

/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int command_type, unsigned int delay_after){
    writeFourBits(word, command_type, delay_after, 0);
    writeFourBits(word, command_type, delay_after, 1);
}

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) {
    writeLCD(c, 1, 500);
}
/*Initialize the LCD
 */
void initLCD(void) {
    // DATA PORTS
    TRIS_D7 = OUTPUT;
    TRIS_D6 = OUTPUT;
    TRIS_D5 = OUTPUT;
    TRIS_D4 = OUTPUT;
    // RS AND ENABLE
    TRIS_RS = OUTPUT;
    TRIS_E  = OUTPUT;

    //----- BEGIN SEQUENCE -----//

    delay_ms(40);
    
    // SPECIFIC SEQUENCE
    writeLCD(0x03, 0, 4500);
    writeLCD(0x03, 0, 110);
    writeLCD(0x03, 0, 50);
    writeLCD(0x02, 0, 50);
    // END

    writeLCD(0x28, 0, 500); // FUNCTION SET
    writeLCD(0x0C, 0, 500); // DISPLAY ON, CURSOR OFF, BLINK OFF
    writeLCD(0x01, 0, 500); // CLEAR DISPLAY
    writeLCD(0x06, 0, 500); // ENTRY MODE, SET INCREMENT

    //----- END SEQUENCE -----//
}

/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s) {
    while(*s)
        printCharLCD(*s++);
}

/*
 * Clear the display.
 */
void clearLCD(){
    unsigned char word = 0x01;
    writeLCD(word, 0, 1400); // CLEAR COMMAND
    moveCursorLCD('1', '1'); // RESET POSITION
}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char x, unsigned char y){
    unsigned char address;
    unsigned char row, col;
    row = x&(0x0f);
    col = y&(0x0f);
    address = (col - 1)*0x40 + (row);
    address = address + 0x80;
    writeLCD(address, 0, 500);
}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD(){
    clearLCD();
    moveCursorLCD(1,1);
    int i = 0;
    printCharLCD('c');
    for(i = 0; i < 4000; i++) delayUs(1000);
    clearLCD();
    moveCursorLCD(1,1);
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for(i = 0; i < 4000; i++) delayUs(1000);
    printStringLCD("Hello!");
    for(i = 0; i < 4000; i++) delayUs(1000);
}

string getTimeString(int min, int sec) {
    int SS = 0, FF =0;
    char stringTime[8];
    SS = sec / 100;
    FF = sec % 100;
    sprintf(string, "%02d:%02d:%02d",min,SS,FF);
    return stringTime;
}
