/********************************************
*
*  Name: Sai Praneetha Mutyala
*  Section: 2 (2.30-3.50)
*  Assignment: Lab 7- Stopwatch
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"

void init_ports(void);													
void init_adc(void);													
void init_timer1(unsigned short m);										



unsigned char state = 0; 													// initializing the initial state to 0
char time[3];																// array of time digits places 
char output[5]; 															// array of output 

int main(void){
	
	
	init_ports();															// Initialize the output ports 
	init_adc();																// Initialize the adc
	init_lcd();																// Initialize the lcd
	init_timer1(6250);														// Initialize the timer 
	
	
	sei();																	//Enable interrupts
	
	writecommand(1);														// Clears the screen 
	stringout("Welcome!");													// Print "Welcome" to screen and splash out 
	
	_delay_ms(2000);														// delay for 2 seconds
	
	writecommand(1);														// clear the screen 
	time[0] = 0;
	time[1] = 0;
	time[2] = 0;
	snprintf(output,5,"%d%d.%d", time[0], time[1], time[2]);				// Print the timer format 
	stringout(output);														// Print the timer to the screen 
	moveto(1,4);															// Move the cursor to the following place 
	
	int result;				
	
	while(1){																// Loop forever
	
	
		ADCSRA |= (1 << ADSC); 												// Start a conversion, wait for it to complete, read the ADC result,
		
		while ( (ADCSRA & (1 << 6)) != 0 ); 								// Stops looping if ADSC is a zero
		
		// ADC conversion done, grab result 
		result = ADCH; 														//ADCH is the register which holds the result
		
		
		// When state is 0, the up button makes the timer to go to state 1 where it keeps counting up, and the down button returns it back to its 0 state!
		if (state == 0){
			if (result >= 47 && result <= 57){								// For up button to be pressed 
				state = 1;
				_delay_ms(200);
			}
			else if (result >= 100 && result <= 110){						// For down button to be pressed 
				_delay_ms(200);
				time[0] = 0x00;
				time[1] = 0x00;
				time[2] = 0x00;
				moveto(1,1);
				snprintf(output,5,"%d%d.%d", time[0], time[1], time[2]);
				stringout(output);
			}
		}
		
		// When state is 1, the up button allows it to go the 0 state, and the down button allows it to go to the state 2, where it stops but keeps counting the time internally 		
		else if (state == 1){
			moveto(1,1);
			snprintf(output,5,"%d%d.%d", time[0], time[1], time[2]);
			stringout(output);
			if (result >= 47 && result <= 57){  							// For up button
				state = 0;
				_delay_ms(200);
			}
			else if (result >= 100 && result <= 110){ 						// For down button 
				state = 2;
				_delay_ms(200);
				
			}			
			
		}
		
		// When the state is 2, the up button allows it to come back to its state 1 and the down button too allows it to come back to state 1. 
		else {
			if (result >= 47 && result <= 57){ 								//for up button 
				state = 1;
				_delay_ms(200);
			}
			else if (result >= 100 && result <= 110){ 						// For down button 
				state = 1;
				_delay_ms(200);
				
			}			
			
		} 
         
	}
	
	
	return 0; /* never reached */
}

ISR(TIMER1_COMPA_vect){
	// increments every 0.1 seconds
	// time[0] = tenths place before the decimal point 
	// time[1] = ones place before the decimal point
	// time[2] = one/tenths place after the decimal point 
	if (state != 0){   											//If not stopped, it will keep on incrementing
		if (time[2] == 9){  									// When time at the tenths place after the decimal point strikes 9, it returns to 0 and starts counting 
			time[2] = 0;
			if (time[1] == 9){									// When time at ones place before the decimal point strikes 9, it returns to 0 and starts counting 
				time[1] = 0;
				if (time[0] == 5){								// When time at the tenths place before the decimal point strikes 5, all the places go back to 0 
					time[0] = 0;
					time[1] = 0;
					time[2] = 0;
				}
				else{
					time[0]++;									// Time at this place keeps counting until it does not strike 5 
				}
				
			}
			else{
				time[1]++;										// Time at this place keeps counting until it does not strike 9 
			}
		}
		else{
			time[2]++;  										// Time at this place keeps counting until it does not strike 9 
		}
			
	}
	
}

void init_timer1(unsigned short m) {
   TCCR1B |= (1 << WGM12); 										// Set to CTC mode
   TIMSK1 |= (1 << OCIE1A); 									// Enable Timer Interrupt
   OCR1A = m; 													//prescalar=256 counting to 6250 = 0.1s w/ 16 MHz clock
   
   // and start counter
   TCCR1B |= (1 << CS12);
  
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
 
 void init_ports()
 {
	DDRD |= 0xF0; //setting D4-D7 as outputs
	DDRB |= 0x03; //setting B1-B2 as outputs
 }
 
 