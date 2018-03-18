/********************************************
*
*  Name: Sai Praneetha Mutyala
*  Section: 2 (2.30-3.50)
*  Assignment: Project
*
********************************************/
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "speedtrap.h"
#include <avr/eeprom.h>
void init_encoder(void);

//short int get_count();
//void set_count(short int x);
int count;
void encoder_on(void);
char ostr[20];
volatile char hasChanged;
unsigned char bits,a,b;
unsigned char old_state, new_state;
