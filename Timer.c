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
#include "Timer.h"
ISR(TIMER1_COMPA_vect){
	// Increment every 0.001 s
	msec+=1;		
}


void init_timer1(unsigned short m) {
   TCCR1B |= (1 << WGM12); 										// Set to CTC mode
   TIMSK1 |= (1 << OCIE1A); 									// Enable Timer Interrupt
   OCR1A = m; 													//prescalar=8 counting to 2000 = 0.001s w/ 16 MHz clock
   
   // and start counter
   TCCR1B |= (1 << CS10);
   
  
}
