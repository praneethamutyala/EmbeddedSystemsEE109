/********************************************
*  Name: Sai Praneetha Mutyala
*  Lecture Time: 2-3.20 
*  Assignment: Lab 2 - Arduino Outputs
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD |= (1 << DDD7);	/* Set PD7 for output */

    while(1) {
	PORTD |= (1 << PD7);
	_delay_us(15);
	/* Output high */
	PORTD &= ~(1 << PD7);
	_delay_us(10);
	/* Output low */
	PORTD |= (1 << PD7);
	_delay_us(5);
	PORTD &= ~(1 << PD7);
	_delay_us(5);
	PORTD |= (1 << PD7);
	_delay_us(5);
	PORTD &= ~(1 << PD7);
	_delay_us(10);
	
	
    }
	

    return 0;   /* never reached */
}
