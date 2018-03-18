/********************************************
*
*  Name: Sai Praneetha Mutyala
*  Section: 2 (2.30-3.50)
*  Assignment: Lab 9 - Rotary Encoders
*
********************************************/
#include <avr/io.h>
#include <util/delay.h>
void init_lcd(void);
void moveto(unsigned char, unsigned char);
void stringout(char *);

void writecommand(unsigned char);
void writedata(unsigned char);