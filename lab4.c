/********************************************
*
*  Name: Sai Praneetha Mutyala
*  Lab section: 2 (2.30 - 3.50)
*  Assignment:  Lab 4 - ADC
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

void variable_delay_us(int);
unsigned int result;


unsigned int period[13] =     // Periods in microseconds for 13 tones
{ 7634, 6803, 6061, 5405, 4808, 4292, 3817, 3401, 3030, 2703, 2410, 2146, 1912 };

int main(void)
{
    // Initialize appropriate DDR and PORT registers
	DDRB |= (1 << 4);    		// Set PORTB, 4 for output 


    // Initialize the ADC
	ADMUX |= (1 << MUX0) | (1 << MUX1); // Choose input A3 on the arduino
	ADMUX |= (1 << REFS0);   // Set REFS0 to a 1 => 5V for Vhi
	ADMUX |= (1 << REFS1);	 // Set REFS1 to a 1 => 5V for Vhi 
	ADCSRA |= (1 << ADPS2)| (1 << ADPS1) | (1 << ADPS0); // Prescaler to 128
	ADMUX |= (1 << ADLAR); // use 8-bit conversion 
	ADCSRA |= (1 << ADEN); // Turn on the ADC



    while (1) {                 // Loop forever
        // Start an ADC conversion
		ADCSRA |= (1 << ADSC); 
		
		
		while ( (ADCSRA & (1 << 6)) != 0 ); // Stops looping if ADSC is a zero
		
			
		// ADC conversion done, grab result 
		result = ((int)ADCH)/20; //ADCH is the register which holds the result and divide it by 20 to get equal 13 intervals 

		// Determine which tone (0-12) to play
		unsigned int tone = period[result]/2;


		// Generate 10 periods of the output tone
		int i = 0; 				// Let i be the number of periods 
		for (i=0; i<10; i++)    // Until i reaches to 10 periods, the loop will continue 
		{
			PORTB |= (1 << PB4);  // set to 1
			variable_delay_us(tone);
		
			PORTB &= ~(1 << PB4); // clear to 0
			variable_delay_us(tone);
			
			
		}	
		_delay_ms(200); // delay by 200 ms
					

    }
	
    return 0;   /* never reached */
}

/*
  variable_delay_us - Delay a variable number of microseconds
*/
void variable_delay_us(int delay)
{
    int i = (delay + 5) / 10;
    
    while (i--)
	_delay_us(10);
}
