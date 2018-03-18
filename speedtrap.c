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
#include "lcd.h"
#include "encoder.h"
#include "serial.h"
#include "speedtrap.h"
#include <avr/eeprom.h>
#include "sensor.h"
#include "LED.h"
#include "buzzer.h"
#include "Timer.h"

// Global Variables 
int rs; 										// receive speed 
volatile unsigned char flag = 0;				// state of the sensor
int speed;										// speed to be calculated
char time[3];									// time array 
char output[5];									// output array 
int distance = 2032;							// distance = 0.8 inches = 0.8x2.54 = 2.032 cm, but as msec = 10^-3 s, distance = 2032 while calculating speed 
char checkInput =0;								// check for input flag 
unsigned char bit, c, d;						
volatile int msec;
volatile int msecFinal =0;
int speed_Ref = 0;
int max;



int main(){
	count = eeprom_read_byte((void*)200);		// get new updated count as max
	
	init_adc();									// Initialize adc 
	init_lcd();									// Initialize lcd
	init_sensor();								// Initialize sensor
	init_led();									// Initialize led
	init_timer1(2000);							// Initialize timer 
	init_encoder();								// Initialize encoder
	init_buzzer();								// Initialize buzzer
	init_serial();								// Initialize serial 
	
	sei();										// Enable global interrupt 
	encoder_on();								// on the encoder function
	
	bits = PINB;									// Read the two encoder input at the same time
	a = bits & (1 << PB3);
	b = bits & (1 << PB4);
	
	// If order is switch b and then a 				// Transitions of the old state of encoder 
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
	
	while(1){													// loop forever 
		
		writecommand(1);										// clears the screen 
		

		encoder_on();											// encoder on 
		eeprom_update_byte (( void *) 200 , count );			// updating the new count 
		led_on();												// led on 
		
		if (flag ==1){											// if flag is 1, then timer starts incrementing at 0.001 s and red led is on and prints the timer counting 
			//TCCR1B |= (1 << CS10);
			red_on();
			msec+=1;
			moveto(1,1);
			snprintf(output, 5, "%3d", msec);
			stringout(output);
		}
		if (flag == 0){ 										// if flag is 0, then the timer stops and the red led stops and prints the time and speed of the object
			TCCR1B &= ~(1 << CS10);								// the timer counter stops 
			//msec = msecFinal;
			red_off();											// red led is switched off
			msec = msec; 										// the time is updated			
			moveto(1,1);										// moves the cursor to the following position 
			snprintf(output, 5, "%3d", msec);					// prints the updated time 
			stringout(output); 									// prints the updated time to the screen 
			
			moveto(1,5);										// moves the cursor to the following position 
			stringout("ms");									// stringouts to the screen - "ms"
			moveto(1,8);										// moves the cursor to the following position
			stringout("=");										// stringouts to the screen - "="
	
			if (rs > max){										// if received speed is greater than max count updated, buzzer is on and red led blinks 
				buzzer_on();										
				red_on();										
				_delay_ms(1);
				red_off();
				_delay_ms(1);
			}
		}
		if (speed != speed_Ref){								// if initial speed is changed, then print the new updated speed to the screen 
			speed = (distance/msec);
			snprintf(output, 5, "%d", speed);
			moveto(1,12);
			stringout(output);	
		}
		
		if (!checkInput){ 										// If there is something which obstructs the sensors, the red led and timer act accordingly 
			if (c==0){
				flag = 1;
			}
			if (c!=0){
				flag =1;
			}
			if (d!=0){
				flag = 0;
			}
			if (c!=0 && d==0){
				flag =1;
			
			}
			if(c!=0 && d!=0){
				flag = 0;
				
			}
			if (c==0 && d!=0){
				flag =1;
				
			}
			if (c==0 && d==0){
				flag =1;
				
			} 
			
			
		}
		else if (checkInput){ 										// if there isnt something which obstructs the sensors, flag = 0
			flag =0;				
			
			
		} 
		if (speed != speed_Ref){									//Condition to check whether the initial speed has changed 
			speed = 0;												// Initial condition for speed 
			speed = (distance*1000/msec);							// If yes, then calculate the new speed
			snprintf(output, 5, "%d", speed);						// print the new speed
			moveto(1,10);											// move the curson to row1,col 10
			stringout(output);										// print the new speed to the lcd screen 
		} 	
		if (hasChanged == 1) { 										// Condition to check whether - Did state change?
			hasChanged = 0;											// Initial condition for hasChanged is 0. 
			snprintf(ostr, 20, "Max = %2d", count);					// If changed, then print the message! 	
			moveto(2,1);											// move the cursor to the following position on the screen
			stringout(ostr);  										// print the updated count to the lcd screen 	
			eeprom_update_byte((void*)200, count);					// update the max count and store it in the address mentioned 
			
		}
		
		if ((PIND & (1<<PD1) !=0)){									// if pin is connected, send the serial
			send_serial();
		}
		if ((PIND & (1<<PD0) != 0)){								// if pin is connected, receive the serial 
			get_serial();
		}
	
	}
	
	return 0;
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