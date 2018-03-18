/********************************************
*
*  Name: Sai Praneetha Mutyala
*  Lab section: 2 (Tue: 2.30-3.50) 
*  Assignment:Lab 3 (Buttons and LED's) 
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#define DOT_LENGTH  250         /* Define the length of a "dot" time in msec */
#define DASH_LENGTH 750         /* Define the length of a "dash" time in msec. It is 3 times that of the dot length. */ 

void dot(void);
void dash(void);
void makeOutput(char); 			//Takes input char and returns nothing
char checkInput(char);			// Takes input char and returns a char

int main(void)
{

    // Initialize appropriate DDR and PORT registers
	DDRB |= 0x01;  				// Initialize B0 to to output 
	PORTB |= 0x00;
	
	 // Enable pull up resistors 
	PORTD |= 0x04;     			// For D2
	PORTD |= 0x08;     			// For D3
	PORTD |= 0x10;     			// For D4
	 

    while (1) {
		// Loop forever
		if (!checkInput(2))
			{
				// Code for blinking a "U"				
				dot(); 
				dot(); 
				dash();		
			
			}
		if (!checkInput(3))
			{
				// Code for blinking a "S"
				dot(); 
				dot(); 
				dot();
			}
		
		if (!checkInput(4))
			{
				// Code for blinking a "C"
				dash(); 
				dot();
				dash();
				dot();				
			}
		
			
		
    }

    return 0;   /* never reached */
}

/*
  dot() - Makes the output LED blink a "dot".
*/
void dot()
{
	// Determining dot when output is 1
	makeOutput(1);
	_delay_ms(DOT_LENGTH);
	// Determining space when output is 0
	makeOutput(0);
	_delay_ms(350);

}

/*
  dash() - Makes the output LED blink a "dash".
*/
void dash()
{
	// Determining dash when output is 1
	makeOutput(1);
	_delay_ms(DASH_LENGTH);
	// Determining space when output is 0 
	makeOutput(0);
	_delay_ms(DOT_LENGTH);

}

/*
  makeOutput() - Changes the output bit to either a zero or one.
  If the argument is zero, the bit is cleared to zero,
  otherwise it is set to a one.
  */

void makeOutput(char value)
{
    if (value == 0)
        PORTB &= ~(1 << PB0);
    else
        PORTB |= (1 << PB0);
}

/*
  checkInput(bit) - Checks the state of the input bit given as
  the argument (0-7), and returns either 0 or 1 depending on its state.
*/
char checkInput(char bit)
{
    if ((PIND & (1 << bit)) != 0)
        return(1);
    else
        return(0);
} 
