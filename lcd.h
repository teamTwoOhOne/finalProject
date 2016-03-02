/*
 * File:   lcd.h
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */
#ifndef __LCD_H
#define __LCD_H
#include <string.h>
void writeLCD(unsigned char word, unsigned int command_type, unsigned int delay_after);
void writeFourBits(unsigned char word, unsigned int command_type, unsigned int delay_after, unsigned int lower);
void initLCD(void);
void clearLCD(void);
void moveCursorLCD(unsigned char x, unsigned char y);
void printCharLCD(char c);
void printStringLCD(const char* s);
void testLCD();
void toggleEnable(unsigned int delay);
void setData(int db7, int db6, int db5, int db4);
string getTimeString(int min, int sec);
#endif /*__LCD_H */