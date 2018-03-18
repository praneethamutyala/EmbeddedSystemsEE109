/********************************************
 *
 *  Name: Sai Praneetha Mutyala
 *  Section: 2 (2.30-3.50)
 *  Assignment: Lab 9 - Rotary Encoder
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
void init_ports(void);
void init_adc(void);

char ostr[20];											// Array of ostr containing 20 elements 
unsigned char bits, a, b;								// bits a and b 
unsigned char old_state;								//old state
volatile int count = 0;									// Count to display
unsigned char new_state;								// new state 
volatile char hasChanged;								// changed state

int main(void){
		init_ports(); 									// Initialises the ports
		init_lcd();										// Initialises the lcd screen 
		init_adc										// Initialises the adc 
		sei();											// Enables the interrupt
		writecommand(1);								// Initial display		
		char ostr[20];
		
		
		stringout("EE109 Lab 9"); 						//prints the message onto the screen 
		moveto(1, 1);									// moves the cursor to the following position on the lcd screen 
		stringout("   0");								// prints the following message onto the screen 
		
		_delay_ms(5);									// delay time 
		writecommand(1);								// clear the screen 
		snprintf(ostr, 20, "%4d", count);				// print the count 
		stringout(ostr);								// print the message to the screen 
		
		bits = PINC;									// Read the two encoder input at the same time
		a = bits & (1 << PC4);
		b = bits & (1 << PC5);
	
		// If order is switch b and then a 
		if (!b && !a){
			old_state = 0;
		}
		else if (!b && a){
			old_state = 1;
		}
		else if (b && !a){
			old_state = 2;
		}
		else{
			old_state = 3;
		}

		
		
		while(1){ //Loop forever 
			// Check for state (and count) change
			
			if (hasChanged == 1) { 						// Condition to check whether - Did state change?
				hasChanged = 0;							// Initial condition for hasChanged is 0. 
				snprintf(ostr, 20, "%4d", count);		// If changed, then print the message! 	
				moveto(1, 1);							// move the cursor to the following position on the screen
				stringout(ostr);    					// Print value of count to the screen 
				
			}
		}
	}

ISR(PCINT1_vect) {	
    
	bits = PINC;										// Read the two encoder input at the same time
	a = bits & (1 << PC4);
	b = bits & (1 << PC5);
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
	old_state = new_state;								// Old state now is the new state   
}

void init_ports() {
    PCICR |= (1 << PCIE1);
    PCMSK1 |= ((1 << PCINT12)| (1 << PCINT13));
    DDRD |= 0xF0; 										//setting D4-D7 as outputs
    DDRB |= 0x03; 										//setting B1-B2 as outputs
    PORTC |= ((1 << PC4) | (1 << PC5));					// Enable the pull up resistors 
}
void init_adc()
 {
	// Initialize ADC modules of the arduino
	ADMUX = 0; 												// Choose input A0 on the arduino				
	ADMUX |= (1 << REFS0);   								// Set REFS0 to a 1 => 5V for Vhi
	ADMUX |= (1 << ADLAR); 									// use 8-bit conversion
	ADCSRA = 0;
	ADCSRA |= (1 << ADPS2)| (1 << ADPS1) | (1 << ADPS0); 	// Prescaler to 128
	ADCSRA |= (1 << ADEN); 									// Turn on the ADC	 
 }     
