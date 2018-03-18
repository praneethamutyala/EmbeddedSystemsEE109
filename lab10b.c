/********************************************
 *
 *  Name: Sai Praneetha Mutyala
 *  Section: 2 (2.30-3.50)
 *  Assignment: Lab 10b - Part 2 - Pulse Width Modulation 
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>


void init_ports(void);									// Initialising the ports 


unsigned char bits, a, b;								// bits a and b 
unsigned char old_state;								//old state
volatile int count = 47;								// Initialises the Count to display
unsigned char new_state;								// new state 
volatile char hasChanged;								// changed state

int main(void){
	init_ports(); 										// Initialises the ports
	init_pwm();											// Initialises the pwm 
	
	sei();												// Enables the interrupt	
	
	bits = PINC;										// Read the two encoder input at the same time
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

	
	
	while(1){ 											//Loop forever 
	// Check for state (and count) change		
		if (hasChanged == 1) { 							// Condition to check whether - Did state change? And if changed, the following will occur
			
			OCR1B = count;								// The OCR1B is updated to be the new count 
			hasChanged = 0;								// Initial condition for hasChanged is 0. 
		}
	}
	return 0;
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
	if (count < 47){									// Condition for the count, if count goes less than 47, then it is equal to 47 
		count = 47;
	}
	else if(count > 141){								// And if the count goes more than 141, then it is equal to 141 
		count = 141;
	}
	old_state = new_state;								// Old state now is the new state  

	
}

/*
  init_ports - Do various things to initialize the ports
*/

void init_ports() {
    PCICR |= (1 << PCIE1);
    PCMSK1 |= ((1 << PCINT12)| (1 << PCINT13));
    DDRB |= 0x04; 										//setting the DDRB for B2 to be a 1 to set it to be output. 
	PORTB |= 0x04;										// setting B2 as output
    PORTC |= ((1 << PC4) | (1 << PC5));					
}

/*
  init_pwm - intitialize 
*/

void init_pwm(void) {
    TCCR1B |= (1 << WGM12); 							// Setting to CTC mode
    TCCR1B |= (1 << WGM13);
    TCCR1A |= (1 << WGM10);
    TCCR1A |= (1 << WGM11);
    TCCR1A |= (1 << COM1B1);
    TCCR1B |= (1 << CS12);
    OCR1A = 1250;
    OCR1B = 47; 										//(47 -> 0.75 ms and 141 -> 2.25 ms)
}
