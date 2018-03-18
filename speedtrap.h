
/********************************************
*
*  Name: Sai Praneetha Mutyala
*  Section: 2 (2.30-3.50)
*  Assignment: Lab 9 - Rotary Encoders
*
********************************************/
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include <avr/eeprom.h>


void init_adc();
int speed;
char time[3];
char output[5];
