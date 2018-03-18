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
#include "sensor.h"
unsigned char bit, c, d;						

/* Make the sensors work */
void init_sensor(){
	//DDRD |= (1<<2)|(1<<3);
	PORTD |= (1<<PD2)|(1<<PD3);								//pull up button
	PCICR |= (1 << PCIE2);									//enable interrupts on PORTD(buttons)
	PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);				//enable interrupt bit PD2 and PD3
	bit = PIND;
	c = bit & (1<<PD2);
	d = bit & (1<<PD3);
}


ISR(PCINT2_vect){
	bit = PIND;
	c = bit & (1<<PD2);
	d = bit & (1<<PD3);
	if(c!=0){												//if first object was detected 
		msec +=1;
	}
	if (c!=0 && d==0){
		msec+=1;
			
	}
	if(c!=0 && d!=0){
		msec=0;;
				
	}
	if (c==0 && d!=0){
		msec+=1;
				
	}
	if (c==0 && d==0){
		msec+=1;	
	}
		
	if(d!=0){												//if second object was detected 
	
		msec = msec;
		
	}
	
}