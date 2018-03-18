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
#include "speedtrap.h"
#include "buzzer.h"

/* Make the buzzer work */
void init_buzzer(){
	DDRC |= (1<<3);			// setting bit 3 of Port C to output 
	
}

void buzzer_on(){
	
	int i;
	for(i=0; i <5; i++){
		PORTC |= (1<<3);
		_delay_us(1);

		PORTC &= ~(1<<3);
		_delay_us(1);

	}
		
}