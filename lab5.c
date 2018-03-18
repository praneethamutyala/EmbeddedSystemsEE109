/********************************************
*
*  Name: Sai Praneetha Mutyala
*  Section: 2 (2.30-3.50)
*  Assignment: Lab 5 - Up/Down counter on LCD display 
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void init_lcd(void);
void moveto(unsigned char, unsigned char);
void stringout(char *);
void writecommand(unsigned char);
void writedata(unsigned char);
void writenibble(unsigned char);

int main(void) {

    /* Main program goes here */
	init_lcd();				// Initialize the lcd 
	DDRB |= (1 << 1);		// Initializing the ports necessary to control the LCD 
	DDRB |= (1 << 0);
	DDRD |= (1 << 4);
	DDRD |= (1 << 5);
	DDRD |= (1 << 6);
	DDRD |= (1 << 7);
	
	PORTB |= 0x00;
	PORTB |= 0x01;
	
	//Enable pull up resistors
	PORTC|= (1<<2);
	PORTC|= (1<<4);
	
	char count=0;   					// Initializing count to be 0
	unsigned char direction = 1; 		// initializing the direction to be up
	writecommand(1);   					// Clearing the screen


    while (1) {               // Loop forever
		int i,j;
		
		for (i =0; i<10; i++)     				// For the direction to change instantly as the buttons are pressed 
		{
			if ((PINC & (1 << 2)) == 0) {		// If button connected to A2 is pressed, then direction is up
				direction = 1; 
				
				
			
			}
			if ((PINC & (1 << 4)) == 0) {		// If button connected to A4 is pressed, then direction is down
				direction = 0;
				
				
			
			} 
			
			_delay_ms(50);						// delaying for 50 ms every time the buttons are pressed
			
		}
		
		
		if (direction == 1)						// The numbers will count up for direction up 
		{
			count++;
			
		}
		if (direction == 0)						// The numbers will count down for direction down 
		{
			count--;
		
		}
		
		if (count > 9)							// For count greater than 9, the number will come back to 0
		{
			count = 0;
			
			
		}
		if (count < 0)							// For count less than 0, the number will come back to 9
		{
			count = 9;
			
		}
		
		
		
		writecommand(1);						// Clear the screen
		writedata(count + 0x30);				// Print the numbers by updating the count 
		
		_delay_ms(500);							// Delay for 500ms 
		
		

    }

    return 0;   /* never reached */
}

/*
  init_lcd - Configure the I/O ports and send the initialization commands
*/
void init_lcd()
{
    /* ??? */	// Set the DDR register bits for ports B and D
	DDRB |= (1 << 1);
	DDRB |= (1 << 0);
	DDRD |= (1 << 4);
	DDRD |= (1 << 5);
	DDRD |= (1 << 6);
	DDRD |= (1 << 7);
	
	
	

    _delay_ms(15);              		// Delay at least 15ms

    writenibble(0b0011000);             // Use writenibble to send 0011
    _delay_ms(5);               		// Delay at least 5msec

    writenibble(0b0011000);             // Use writenibble to send 0011
    _delay_us(100);             		// Delay at least 100usec

    writenibble(0b0011000);             // Use writenibble to send 0011, no delay needed

    writenibble(0b00100000);            // Use writenibble to send 0010
    _delay_ms(2);                       // Delay at least 2ms
    
    writecommand(0x28);                 // Function Set: 4-bit interface, 2 lines
	_delay_ms(2);						// Delay 2 ms

    writecommand(0x0f);                 // Display and cursor on
	_delay_ms(2);						// Delay 2ms

}

/*
  moveto - Move the cursor to the row (1 to 2) and column (1 to 16) specified
*/
void moveto(unsigned char row, unsigned char col)
{
    /* Don't need this routine for Lab 5 */
}

/*
  stringout - Write the string pointed to by "str" at the current position
*/
void stringout(char *str)
{
    /* Don't need this routine for Lab 5 */
}

/*
  writecommand - Send the 8-bit byte "cmd" to the LCD command register
*/
void writecommand(unsigned char cmd)
{
	
	PORTB &= ~(1<<0);			// Clear R to 0
	writenibble(cmd);			// Print cmd 
	writenibble(cmd << 4);		//Sending lower 4 bits to the upper area 
	_delay_ms(2);				// Delay for 2ms 
	

}

/*
  writedata - Send the 8-bit byte "dat" to the LCD data register
*/
void writedata(unsigned char dat)
{
	PORTB |= (1 << PB0); 		// Clear R to 1
	writenibble(dat);			// Print dat 
	writenibble(dat << 4); 		//Sending lower 4 bits to the upper area
    _delay_ms(2);				// Delay for 2ms 

}

/*
  writenibble - Send four bits of the byte "lcdbits" to the LCD
*/
void writenibble(unsigned char lcdbits)
{
	PORTD &= 0x0F;					
    PORTD |= (lcdbits & 0xF0);		
    PORTB &= ~(1 << PB1); 			// Clear E to 0
    PORTB |= (1 << PB1); 			// Set E to 1
    PORTB |= (1 << PB1); 			// Set E to 1
    PORTB &= ~(1 << PB1); 			// Clear E to 0

}
