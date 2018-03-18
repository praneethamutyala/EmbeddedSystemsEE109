/********************************************
*
*  Name: Sai Praneetha Mutyala
*  Section: 2 (2.30-3.50)
*  Assignment: Lab 9 - Rotary Encoders
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "speedtrap.h"
#include "lcd.h"
void writenibble(unsigned char);

//void moveto(unsigned char, unsigned char);
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
    if (row == 1)
	{
		writecommand(0x80+ 0x00 + col-1);  // Command for row 1
	}
	if (row == 2)
	{
		writecommand(0x80 + 0x40 + col-1); // Command for row 2
	}
	
}

/*
  stringout - Write the string pointed to by "str" at the current position
*/
void stringout(char *str)
{
    int i = 0;
	while(str[i] != '\0')
	{
		writedata(str[i]);
		i+=1;
	}
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
