/********************************************
*
*  Name: Sai Praneetha Mutyala
*  Section: 2 (2.30-3.50)
*  Assignment: Project
*
********************************************/

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "lcd.h"
#include "encoder.h"
#include "speedtrap.h"
#include "serial.h"
#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0

short int i;
char serial[5],sent=0;
char ready=0,here[5];
int rs;

void init_serial(){						//initialize all the registers for using the serial
	DDRD |= (1<<PD0);
	PORTD &=~(1<<PD0);
	DDRD |= (1<<PD1);
	PORTD &=~ (1<<PD1);
	UBRR0 = MYUBRR; // Set baud rate
	UCSR0B |= (1<<RXCIE0);
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0);
	UCSR0C = (3<<UCSZ00);
}

void send_serial(){
		if(speed>=0){
			serial[0]=64;			//fisrt char == @
		}else{
			serial[4]=36;			//last char == $
		}
		serial[1]=(speed/1000)+48;				//isolate the numbers and convert to ASCII
		serial[2]=((speed%1000)/10)+48;
		serial[3]=(speed%100)+48;
		serial[4] = (speed%10)+48;
		for(i=0;i<5;i++){
			while((UCSR0A&(1<<UDRE0))==0){		//wait until it is good to send the next char
			
			}
			UDR0=serial[i];						//send the next char
		}
}

void get_serial(){
	if(ready==1){
		moveto(1, 12);
		stringout("    ");
		moveto(1, 12);
		
		rs=(here[1]-48)*1000;					//get the numbers, one by one and turn into a single variable
		rs+=(here[2]-48)*100;
		rs+=(here[3]-48)*10;
		rs+= (here[4]-48)*1;
		if(here[0]==45){
			rs*=-1;								//make it negative if necessary
		}
		snprintf(output,8,"%d",rs);
		stringout(output);						//print
		moveto(2,10);
		ready=0;
	}
}

ISR(USART_RX_vect){
	while ( !(UCSR0A & (1 << RXC0)) ) {}		//wait until its able to get the next signal
	char rx=UDR0;
	if((rx==64||rx==36)&&sent==0){				//check if its the first position, save in here[]
		ready=0;
		here[0]=rx;
		sent++;
	}
	else{
		here[sent]=rx;								//save in here[]
		sent++;
		if(sent==4){								//if it is the end, restart the count and flag it to print
			sent=0;
			ready=1;
		}
	}
}