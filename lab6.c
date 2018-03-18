/********************************************
*
*  Name: Sai Praneetha Mutyala
*  Section: 2 (2.30-3.50)
*  Assignment: Lab 6 - Writing strings to the LCD display 
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void init_lcd(void);
void moveto(unsigned char, unsigned char);
void stringout(char *);
void init_adc(void);

void writecommand(unsigned char);
void writedata(unsigned char);
void writenibble(unsigned char);

int main(void){
	
	//Enable pull up resistors
	PORTC|= (1<<2) | (1<<4);
    
	init_lcd();												// Initialize the lcd 
	init_adc();												// Initialize the adc			
	
	// Splash Screen: display name AND display birth date using snprintf AND delay for 2 seconds
	// Task 2
	writecommand(1); 										// Clear the screen 
	
	char birthdate[30];										// An array for dates 
	unsigned char month, day, year;	
	month = 11;					
	day = 13;
	year = 96;
	char *dayofweek = "Wednesday";	
	snprintf(birthdate, 30, "DOB is %d/%d/%d = %s", month, day, year, dayofweek);
	stringout("Sai, Praneetha");  							// Printing out the name to the first row to the screen
	moveto(2,1);											// move the cursor to the second row, first column
	stringout(birthdate);									// Print the birthday to the screen 
	_delay_ms(2000);										// delay for 2 seconds
	
	unsigned char result;
	
	char buf[10];											// Create an array of 10 elements in buf
	
	// Displaying the screen of the game!
	char row0[] = "  ##      ##    ";
	char row1[] = "     ## #     #F";
	moveto(1,1);											// move the cursor to the first row, first column
	stringout(row0);										// Print row0[] to the screen
	moveto(2,1);											// move the cursor to the second row, first column
	stringout(row1);										// Print row1[] to the screen 
	_delay_ms(200); 										// Delay for 200 ms. 
	
	// Initializing the game player's position 
	char x = 1;
	char y = 1;
	moveto(1,1);											// Initializing the game player to start from row 1 column 1 position 
	stringout("*");											// Variable for the game player
	char gamemove = 0;										// Initializing the move to be 0
	char moves[32];											// creating an array of 32 moves 
	char gamePlay = gamemove;		
	
	while(1){												// Loop forever
		
		ADCSRA |= (1 << ADSC); 								// Start a conversion, wait for it to complete, read the ADC result,
		
		while ( (ADCSRA & (1 << 6)) != 0 ); 				// Stops looping if ADSC is a zero
		
		// ADC conversion done, grab result 
		result = ADCH; 										//ADCH is the register which holds the result
		
		// Task 3 
		
		
		/* char buf[10];

		snprintf(buf, 10, "%3d", result);
		writecommand(1);
		stringout(buf);
		_delay_ms(300); */
		
		// no button pressed - 255, up button - 52, down button -102, left button - 157, right- 5, select-207
		
		
		// Task 4 for the game field 	
		
		
		if (result>=47 && result<=57 ) 						// For up to be pressed
		{
			moveto(x,y);									// initializing the position of the game player 			
			stringout(" ");									// printing space to the screen
			x -= 1;											// updating the row 
			y = y;											// updating the column which is constant here
			
			moveto(x,y);									// updating the new position 	
			stringout("*");									// printing asterisk to the new position 
			_delay_ms(300);									// delay for 300 ms. 
			
			gamemove++; 									// Keep counting the moves 
		
		} 
		if (result>=207 && result<= 217) 					// Select pressed, reset to 0
		{
			moveto(x,y);									// initializing the position 
			stringout(" ");									// printing space to the screen
			x = 1;											// updating the row 
			y = 1;											// updating the column
			
			moveto(x,y);									// updating the new position 
			stringout("*");									// printing asterisk to the new position 
			_delay_ms(300);									// delay for 300 ms. 
			
			gamemove++;										// Keep counting the moves 
			
		}
		else if (result>=155 && result<=165 )  				// For left to be pressed
		{
			moveto(x,y); 									// initializing the position 
			stringout(" ");									// printing space to the screen
			x = x;											// updating the row 
			y -= 1;											// updating the column
			
			moveto(x,y);									// updating the new position 
			stringout("*");									// printing asterisk to the new position 
			_delay_ms(300);									// delay for 300 ms. 
			
			gamemove++;										// Keep counting the moves 
		}
		else if (result>=100 && result<= 110) 				// For down to be pressed
		{
			moveto(x,y);									// initializing the position
			stringout(" ");									// printing space to the screen
			x += 1;											// updating the row 
			y = y;											// updating the column
			
			moveto(x,y);									// updating the new position 
			stringout("*");									// printing asterisk to the new position 
			_delay_ms(300);									// delay for 300 ms. 
			gamemove++;										// Keep counting the moves 
			
		}
		else if (result>=4 && result<=14)  					//For right to be pressed
		{
			moveto(x,y);									// initializing the position
			stringout(" ");									// printing space to the screen
			x =x;											// updating the row 
			y += 1;											// updating the column
			
			moveto(x,y);									// updating the new position 
			stringout("*");									// printing asterisk to the new position 
			_delay_ms(300);									// delay for 300 ms. 
			
			gamemove++;										// Keep counting the moves 
		} 
		
		// Setting the total no. of moves to print variable on the screen 
		gamePlay = gamemove;
		
		// Condition for winning or losing the game 
		
		if (y<0 || y>16)									// if game player goes off the game field i.e. below column 1 or above column 16, then he loses 
		{
			writecommand(1);
			stringout("You lost!");
			break;
		}
		
		if (x<1 || x>2)										// if game player goes off the game field i.e. below row 1 or above row 2, then he loses
		{
			writecommand(1);
			stringout("You lost!");
			break;
		}
		
		if (x==2 && y==16)              					// Condition for winning the game 
		{
			writecommand(1);
			stringout("You win!");
			break;
		}
		
		if ((y==3 || y==4 || y==11 || y==12) && x==1)		// condition for hitting the obstacle in row 1 and losing the game 
		{
			writecommand(1);
			stringout("You lost!");
			break;
			
		}
		
		if ((y==6 || y==7 || y==9 || y==15) && x==2)		// condition for hitting the obstacle in row 2 an losing the game 
		{
			writecommand(1);
			stringout("You lost!");
			break;
		
		}
		
		
		
	}
	
	// For printing out the number of moves onto the screen by tracing the moves from the beginning of the game 
	moveto(2,1);
	snprintf(moves, 32, "Moves is %d", gamePlay);
	stringout(moves);
	
	
	return 0; /* never reached */
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
	PORTB &= ~(1<<0);

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
