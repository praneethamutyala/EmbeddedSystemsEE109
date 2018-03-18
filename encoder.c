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
#include "encoder.h"


unsigned char bits, a, b;								// bits a and b 
unsigned char old_state;								//old state
volatile int count = 0;						    		// Initialises the Count to display
unsigned char new_state;								// new state 
volatile char hasChanged;								// changed state
int count_ref = 0;
int max;




void encoder_on(void){
	char ostr[20];	
		
	writecommand(1);									// clear the screen 
	snprintf(ostr, 20, "Max=%2d", count);				// print the count 
	moveto(2,1);
	stringout(ostr);									// print the message to the screen 
	
}

ISR(PCINT0_vect) {	
    
	bits = PINB;										// Read the two encoder input at the same time
	a = bits & (1 << PB3);
	b = bits & (1 << PB4);
    hasChanged = 1;
		
	// State transitions
	if (old_state == 0) {
		if (a) {
		new_state = 1;
		count++;
		
		}
		else if (b) {
		new_state = 2;
		count--;
		
		}
	}
	else if (old_state == 1) {
		if (!a) {
		new_state = 0;
		count--;
		
		}
		else if (b) {
		new_state = 3;
		count++;
		
		}
	}
	else if (old_state == 2) {
		if (a) {
		new_state = 3;
		count--;
		
		}
		else if (!b) {
		new_state = 0;
		count++;
		
		}
	}
	else {   // old_state = 3
		if (!a) {
		new_state = 2;
		count++;
		
		}
		else if (!b) {
		new_state = 1;
		count--;
		
		}
	}
	if (count < 1){									// Condition for the count, if count goes less than 47, then it is equal to 47 
		count = 1;
	}
	else if(count > 99){								// And if the count goes more than 141, then it is equal to 141 
		count = 99;
	}
	
	old_state = new_state;								// Old state now is the new state 
	

	
}
/*
  init_ports - Do various things to initialize the ports
*/
void init_encoder(void){
	PCICR |= (1 << PCIE0);
    PCMSK0 |= ((1 << PCINT3)| (1 << PCINT4));
    //DDRD |= 0xF0; 										//setting D4-D7 as outputs
    //DDRB |= 0x03; 										//setting B1-B2 as outputs
    PORTB |= ((1 << PB3) | (1 << PB4));					// Enable the pull up resistors 
	

}



