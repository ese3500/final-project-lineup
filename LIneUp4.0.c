/*
 * MotorControl.c
 *
 * Created: 4/9/2024 1:58:47 PM
 * Author : Zwe
 */ 

#include <avr/io.h>
#define F_CPU               16000000UL   // 16MHz clock
#include <avr/interrupt.h>
#include "inc/uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>

#define UART_BAUD_RATE      9600
#define UART_BAUD_PRESCALER (((F_CPU / (UART_BAUD_RATE * 16UL))) - 1)

//volatile uint32_t val = 0; 
volatile int direction = 1; 
//static double consVel = 10.0; 

//#define time_step 0.02
//double V_naught = 0;
//double V_Total = 0;
//double X_Total = 0;

 int out = 1;
 volatile long int count = 0;
 volatile long int start = 0;
 volatile long int end = 0;
 volatile uint16_t range = 0;
 // 
 volatile int out2 = 1;
 volatile long int count2 = 0;
 volatile long int start2 = 0;
 volatile long int end2 = 0;
 volatile uint16_t range2 = 0;

	#define OFF 67
	#define ON 25

	#define OFF2 67
	#define ON2 25

/* volatile int timesteps = 0;*/
	volatile int drop = 1;	 
 
	double weight = 0.55; 
	int prev = 0; 
	int prev2 = 0; 


void Intialize() {
		cli(); 
		//--------------------------------------------Servo 1---------------------------------------------------------------------//
			DDRB |= (1 << DDB3);      // Set PB3 to be an output pin

			// Timer2, prescaler 1024
			TCCR2B |= (1 << CS20);
			//TCCR1B |= (1 << CS11);
			TCCR2B |= (1 << CS22);

			// Timer2, Phase Correct PWM (mode 1)
			TCCR2A |= (1 << WGM20);

			//CLear on Compare
			TCCR2A |= (1 << COM2A1);
			TCCR2A &= ~(1 << COM2A0);

			OCR2A = OFF; //Duty Cycle

	//--------------------------------------------Servo 2---------------------------------------------------------------------//
// 			DDRD |= (1 << DDD1);      // Set PD1 to be an output pin
// 									  
// 			// Timer4, prescaler  1024
// 			TCCR4B |= (1 << CS40);
// 			TCCR4B |= (1 << CS42);
// 
// 			// Timer4, Phase Correct PWM (mode 1)
// 			TCCR4A  &= ~(1 << WGM40);
// 			TCCR4A  &= ~(1 << WGM41);
// 			TCCR4B  &= ~ (1 << WGM42);
// 			TCCR4B  &= ~(1 << WGM43);
// 
// 			//CLear on Compare
// // 			TCCR4A &= ~(1 << COM4A1);
// // 			TCCR4A |= (1 << COM4A0);
// 
// 			//OCR4A = 20; //Duty Cycle
// 			//OCR4B = 10; //Duty Cycle
// 			
// 			TIMSK4 |= (1<<TOIE4); 
	//------------------------------------------------Ultrasonic 1------------------------------------------------------------------//

 		DDRB |= (1 << DDB2); //Output //	 PB0 Echo		
  		// Timer1, prescale of 1024
  		TCCR1B |= (1 << CS10);
  		TCCR1B &= ~(1 << CS11);
  		TCCR1B |= (1 << CS12);
 
  		TCCR1B |= (1 << ICES1); //check rising edge
  		TIMSK1 |= (1 << ICIE1); //enable input interrupt
 		
		//------------------------------------------------Ultrasonic 2------------------------------------------------------------------//
 		DDRB |= (1 << DDB5); //Output
 		// Timer3, prescale of 1024 
		 //PE2 Echo
 		TCCR3B |= (1 << CS30);
 		TCCR3B &= ~(1 << CS31);
 		TCCR3B |= (1 << CS32);

 		TCCR3B |= (1 << ICES3); //check rising edge
 		TIMSK3 |= (1 << ICIE3); //enable input interrupt


		//------------------------------------------------Motor PWM------------------------------------// 
 					DDRD |= (1<<DDD6);
 		 			DDRD |= (1<<DDD5);

 		 			//phase correct mode
 		 			TCCR0A |= (1<<WGM00); 
 		 			TCCR0A &= ~(1<<WGM01); 
 		 			TCCR0B |= (1<<WGM02); 

 		 			//prescaler 1
 		 			TCCR0B |= (1 << CS00);
 		 			TCCR0B |= (1 << CS01);

 		 			TCCR0B &= ~(1 << CS02);

 		 			//Clear compare match 
 		 			TCCR0A |= (1 << COM0A0);
 		 			TCCR0A &= ~(1 << COM0A1);

 		 			TCCR0A |= (1 << COM0B0);
 		 			TCCR0A |= (1 << COM0B1);

 		 		   OCR0A = 40;              // Sets frequency, 400kHz

		//-------------------------------------------H Bridge-----------------------------------------//
		DDRD |= (1 << DDRD4); //In1 pin
		DDRD |= (1 << DDRD3); //In2 pin

 		sei(); 

}



//ULtrasonic 1----------------------------//
 ISR(TIMER1_CAPT_vect) {
 	if (TCCR1B & (1 << ICES1)) {
 		TCNT1 = 0;
 		count = 0;
 		start = TCNT1; 
 		TCCR1B &= ~(1 << ICES1); //check falling edge
 		} else {
 		end = TCNT1;
 		TCCR1B |= (1 << ICES1); //check rising edge

 		range = (end - start);
		
		int avg = (1 - weight) * prev + weight * range;
 		// Convert integer to string
 		char s[20]; //  string representation of the integer
 		itoa(avg, s, 10);
		UART_putstring("U1: ");
 		UART_putstring(s); //Print distance in centimeter
 		UART_putstring( "\r\n");

  		if (avg > 35) {
  			OCR0B = 10;
  		} else if (avg > 15) {
  			OCR0B = -avg + 45;
  		} else {
			motorbreak();
		}
 		
		prev = avg; 
 		out = 1;
 	}
 }


//-----------------------Ultrasonic 2------------------------------------------//

 ISR(TIMER3_CAPT_vect) {
 	if (TCCR3B & (1 << ICES3)) {
 	//	UART_putstring( "MAYBe");
 		TCNT3 = 0;
 		count2 = 0;
 		start2 = TCNT3;
 		TCCR3B &= ~(1 << ICES3); //check falling edge

 		} else {
 		end2 = TCNT3;
 		TCCR3B |= (1 << ICES3); //check rising edge


 		range2 = (end2 - start2);
		int avg = (1 - weight) * prev2 + weight * range2;
 		// Convert integer to string
 		char s[20]; //  string representation of the integer
 		itoa(avg, s, 10);
		 UART_putstring("U2: ");
 		UART_putstring(s); //Print distance in centimeter
 		UART_putstring( "\r\n");
  		if (avg > 35) {
	  		OCR0B = 10;
	  	} else if (avg > 15) {
	  		OCR0B = -avg + 45;
	  	} else {
	  		motorbreak();
			UART_putstring("STOP");
			direction = 3; 
  		}
  		

		prev2 = avg;
 		out2 = 1;
 	}
 }

//-------------------------------------H Bridge Functions--------------------------------------------------

void motorbreak(){
	PORTD &= ~(1 << PORTD4);
	PORTD &= ~(1 << PORTD3);
	_delay_ms(2500);
	if (drop == 1) {
		OCR2A = ON;
		_delay_ms(4000);
		drop = 0; 
		OCR2A = OFF;
		_delay_ms(2000);
		backward();
	} 
}

void forward(){
	UART_putstring("Gone Forward"); //Print distance in centimeter
	PORTD |= (1 << PORTD4);
	PORTD &= ~(1 << PORTD3);
	direction = 1; 
}

void backward(){
	UART_putstring("Gone Back"); //Print distance in centimeter
	PORTD &= ~(1 << PORTD4);
	PORTD |= (1 << PORTD3);
	direction = 0; 
}

int main(void)
{

	UART_init(UART_BAUD_PRESCALER);
	Intialize(); 
	forward();
	 while (1) {

 			if (direction == 1) {
 		 			 			if (out == 1) {
 		 			 					//Print distance in centimeter
 		 			 					PORTB |= (1<<PORTB2);     // Signal Out
 		 			 					_delay_us(10);
 		 			 					PORTB &= ~(1<<PORTB2);
 		 			 					_delay_us(10);  // Signal Out
 		 			 					out = 0;
 		 			 				}
 			} else if (direction == 0) {
 				
 				 				if (out2 == 1) {
 				 				    //Print distance in centimeter
 				 					PORTB |= (1<<PORTB5);     // Signal Out
 				 					_delay_us(10);
 				 					PORTB &= ~(1<<PORTB5);
 				 					_delay_us(10);  // Signal Out
 				 					out2 = 0;
 				 				}
				
				
			}
			
			

	 }

}













