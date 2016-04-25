//-----------------------------------------//
// ECE 372A Spring 2016
// Lab Group 201:
// Code Written by Zachary Finsterwald
//-----------------------------------------//

#ifndef __LCD_H
#define __LCD_H

void write_lcd(unsigned char word, unsigned int command_type, unsigned int delay_after);
void write_four_bits(unsigned char word, unsigned int command_type, unsigned int delay_after, unsigned int lower);
void init_lcd(void);
void clear_lcd(void);
void move_cursor_lcd(unsigned char x, unsigned char y);
void print_char_lcd(char c);
void print_string_lcd(const char* s);
void toggle_enable(unsigned int delay);
void set_data(int db7, int db6, int db5, int db4);

#endif /*__LCD_H */