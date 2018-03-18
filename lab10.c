/********************************************
 *
 *  Name: Sai Praneetha Mutyala
 *  Section: 2 (2.30-3.50)
 *  Assignment: Lab 10a - Part 1- Pulse Width Modulation 
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>


void init_ports(void);
void init_adc(void);

unsigned char bits, a, b;								// bits a and b 
unsigned char old_state;								//old state
volatile int count = 0;									// Count to display
unsigned char new_state;								// new state 
volatile char hasChanged;								// changed state

int main(void){
	init_ports(); 										// Initialises the ports
	init_pwm();											// Initialises the pwm 
	//init_adc();										// Initialises the adc 
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

	
	
	while(1){ //Loop forever 
		// Check for state (and count) change
		
		if (hasChanged == 1) { 							// Condition to check whether - Did state change?
			
			if (count< 0){								// Condition for the count less than 0 
				count =0;								// count is upated to be 0. 
			}
			if (count > 255){							// and condition for if the count gets more than 255
				count = 255;							// it gets updated to be 255. 
			}
			OCR0A = count;								// OCR0A is updated to be the new updated count 	
			hasChanged = 0;								// Initial condition for hasChanged to be 0. 
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
	old_state = new_state;								// Old state now is the new state   
}

/*
  init_ports - Do various things to initialize the ports
*/

void init_ports() {
    PCICR |= (1 << PCIE1);
    PCMSK1 |= ((1 << PCINT12)| (1 << PCINT13));
    PORTC |= ((1 << PC4) | (1 << PC5));
	DDRD |= 0x40; 										//setting DDR D6 bit to 1 to be it as output.
	PORTD |= 0x40;										// setting D6 as output. 
}

/*
  init_pwm - intitialize 
*/

void init_pwm(void) {
    TCCR0A |= (0b11 << WGM00); 							// Fast PWM mode, modulus = 256
    TCCR0A |= (0b10 << COM0A0); 						// Turn D6 on at 0x00 and off at OCR0A
    OCR0A = 128; 										// Initial pulse duty cycle of 50%
    TCCR0B |= (0b101 << CS00); 							// Prescaler = 1024 => 16ms period
}
