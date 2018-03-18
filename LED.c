/********************************************
 *
 *  Name: Sai Praneetha Mutyala
 *  Section: 2 (2.30-3.50)
 *  Assignment: Project
 *
 ********************************************/

#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "speedtrap.h"
#include "LED.h"



/* Make the LED work */
void init_led(){
	DDRB |=(1<<PB5);		//enable led output
}

void red_on(){
	PORTB |=(1<< PB5);			//turn LED on
	
}
void red_off(){				//turn LED off
	PORTB &= ~(1<< PB5);
}

void led_on(){
	if(speed>count && flag ==1){			//if it was suposed to be on and is not, turn the LED on
		red_on();
		//buzzer_on();
	}
	
	else if (speed<count && flag==0){
		red_off();
	}
}