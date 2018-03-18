/*********************************************************************
*
*	USC EE109, Fall 2015, Lab Assignment 1B
*
*	lab1.c - Turn on an LED on D7 based on input on D4
*
*       Input: Arduino D4 (PORTD, bit 4)
*       Output: Arduino D7 (PORTD, bit 7)
*
**********************************************************************/

#include <avr/io.h>

int main(void) {

    DDRD |= (1 << PD7);         // Set PORTD bit 7 for output

    while (1) {
	if (PIND & (1 << PD4))
	    PORTD &= ~(1 << PD7);   // Turn LED off
	else
	    PORTD |= (1 << PD7);    // Turn LED on
    }

    return 0;   /* never reached */
}
