/*
 * MotorControl.c
 *
 * Created: 4/9/2024 1:58:47 PM
 * Author : Zwe
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "inc/uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include "inc/I2C.h"

#define UART_BAUD_RATE      9600
#define UART_BAUD_PRESCALER (((F_CPU / (UART_BAUD_RATE * 16UL))) - 1)

#define F_CPU               16000000UL   // 16MHz clock

volatile int direction = 1; 
double velocity = 0;

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
			//DDRB |= (1 << DDB3);      // Set PB3 to be an output pin
//
			//// Timer2, prescaler 1024
			//TCCR2B |= (1 << CS20);
			////TCCR1B |= (1 << CS11);
			//TCCR2B |= (1 << CS22);
//
			//// Timer2, Phase Correct PWM (mode 1)
			//TCCR2A |= (1 << WGM20);
//
			////CLear on Compare
			//TCCR2A |= (1 << COM2A1);
			//TCCR2A &= ~(1 << COM2A0);
//
			//OCR2A = OFF; //Duty Cycle
//
	////------------------------------------------------Ultrasonic 1------------------------------------------------------------------//
//
 		//DDRB |= (1 << DDB2); //Output //	 PB0 Echo		
  		//// Timer1, prescale of 1024
  		//TCCR1B |= (1 << CS10);
  		//TCCR1B &= ~(1 << CS11);
  		//TCCR1B |= (1 << CS12);
 //
  		//TCCR1B |= (1 << ICES1); //check rising edge
  		//TIMSK1 |= (1 << ICIE1); //enable input interrupt
 		//
		////------------------------------------------------Ultrasonic 2------------------------------------------------------------------//
 		//DDRB |= (1 << DDB5); //Output
 		//// Timer3, prescale of 1024 
		 ////PE2 Echo
 		//TCCR3B |= (1 << CS30);
 		//TCCR3B &= ~(1 << CS31);
 		//TCCR3B |= (1 << CS32);
//
 		//TCCR3B |= (1 << ICES3); //check rising edge
 		//TIMSK3 |= (1 << ICIE3); //enable input interrupt
//
//
		////------------------------------------------------Motor PWM------------------------------------// 
 					//DDRD |= (1<<DDD6);
 		 			//DDRD |= (1<<DDD5);
//
 		 			////phase correct mode
 		 			//TCCR0A |= (1<<WGM00); 
 		 			//TCCR0A &= ~(1<<WGM01); 
 		 			//TCCR0B |= (1<<WGM02); 
//
 		 			////prescaler 1
 		 			//TCCR0B |= (1 << CS00);
 		 			//TCCR0B |= (1 << CS01);
//
 		 			//TCCR0B &= ~(1 << CS02);
//
 		 			////Clear compare match 
 		 			//TCCR0A |= (1 << COM0A0);
 		 			//TCCR0A &= ~(1 << COM0A1);
//
 		 			//TCCR0A |= (1 << COM0B0);
 		 			//TCCR0A |= (1 << COM0B1);
//
 		 		   //OCR0A = 40;              // Sets frequency, 400kHz
//
		////-------------------------------------------H Bridge-----------------------------------------//
		//DDRD |= (1 << DDRD4); //In1 pin
		//DDRD |= (1 << DDRD3); //In2 pin

		//-------------------------------------------I2C/Accelerometer--------------------------------//	
		TWSR0 &= ~(1 << TWPS1) & ~(1 << TWPS0);
		TWBR0 = ((F_CPU/100000 -16)/2);
		//Power Register
		PRR0 &= ~(1 << PRTWI0);
		//Enable TWI0 & interrupt
		TWCR0 |= (1 << TWEN) | (1 << TWIE);
		
		//Internal Pull ups
		PORTC|= (1 << PORTC5);
		PORTC |= (1 << PORTC4);
			
			int configcheck = 0;
			
			configcheck = set_Clock();

			if (configcheck != 0){
					UART_putstring("Set clock failed \n");
			}


			configcheck = accel_config();

			if (configcheck != 0){
				UART_putstring("Sample Rate Divider Failed \n");
			}
			
			//PCICR |= (1 << PCIE1);
			//PCMSK1 |= (1 << PCINT11);
 		sei(); 

}


//ISR(PCINT1_vect) {
	//UART_putstring("ISR HIT \n");
	//int interrupt_check = 0;
	//interrupt_check = accel_read();
	//if (interrupt_check == 0) {
			//int16_t acc = (stored_data[2] << 8) | stored_data[3];
			//double true_acc = acc * 0.0048;
			//velocity += true_acc;
			//if (direction == 1 & range > 35) {
				//if (velocity > -3 & velocity < 13) {
					//OCR0B = 2.5*velocity + 7.5;
				//} else if (velocity >= 13) {
					//OCR0B = 40;
				//} else if (velocity <= -3) {
					//OCR0B = 0;
				//}
			//} else if (direction == 0 & range2 > 35) {
				//if (velocity < 3 & velocity > -13) {
					//OCR0B = -2.5*velocity + 7.5;
					//} else if (velocity <= -13) {
					//OCR0B = 40;
					//} else if (velocity >= 3) {
					//OCR0B = 0;
					//}
			//}
		//} 
	//else {
		//UART_putstring("ISR Failed /n");
	//}
//}


//ULtrasonic 1----------------------------//
 //ISR(TIMER1_CAPT_vect) {
 	//if (TCCR1B & (1 << ICES1)) {
 		//TCNT1 = 0;
 		//count = 0;
 		//start = TCNT1; 
 		//TCCR1B &= ~(1 << ICES1); //check falling edge
 		//} else {
 		//end = TCNT1;
 		//TCCR1B |= (1 << ICES1); //check rising edge
//
 		//range = (end - start);
		//
		//int avg = (1 - weight) * prev + weight * range;
 		////// Convert integer to string
 		////char s[20]; //  string representation of the integer
 		////itoa(avg, s, 10);
		////UART_putstring("U1: ");
 		////UART_putstring(s); //Print distance in centimeter
 		////UART_putstring( "\r\n");
//
  		//if (avg < 15) {
  			//OCR0B = 40;
			//motorbreak();
  		//} else if (avg <= 35) {
  			//OCR0B = -avg + 45;
  		//}
 		//
		 //
		//prev = avg; 
 		//out = 1;
 	//}
 //}


//-----------------------Ultrasonic 2------------------------------------------//
//
 //ISR(TIMER3_CAPT_vect) {
 	//if (TCCR3B & (1 << ICES3)) {
 	////	UART_putstring( "MAYBe");
 		//TCNT3 = 0;
 		//count2 = 0;
 		//start2 = TCNT3;
 		//TCCR3B &= ~(1 << ICES3); //check falling edge
//
 		//} else {
 		//end2 = TCNT3;
 		//TCCR3B |= (1 << ICES3); //check rising edge
//
//
 		//range2 = (end2 - start2);
		//int avg = (1 - weight) * prev2 + weight * range2;
 		//// Convert integer to string
 		////char s[20]; //  string representation of the integer
 		////itoa(avg, s, 10);
		 ////UART_putstring("U2: ");
 		////UART_putstring(s); //Print distance in centimeter
 		////UART_putstring( "\r\n");
  		//if (avg < 15) {
	  		//OCR0B = 40;
			//motorbreak();
			//direction = 3;
	  	//} else if (avg < 35) {
	  		//OCR0B = -avg + 45;
	  	//} 
//
		//prev2 = avg;
 		//out2 = 1;
 	//}
 //}

//-------------------------------------H Bridge Functions--------------------------------------------------

//void motorbreak(){
	//PORTD &= ~(1 << PORTD4);
	//PORTD &= ~(1 << PORTD3);
	//_delay_ms(2500);
	//if (drop == 1) {
		//OCR2A = ON;
		//_delay_ms(4000);
		//drop = 0; 
		//OCR2A = OFF;
		//_delay_ms(2000);
		//backward();
	//} 
//}
//
//void forward(){
	//UART_putstring("Gone Forward"); //Print distance in centimeter
	//PORTD |= (1 << PORTD4);
	//PORTD &= ~(1 << PORTD3);
	//direction = 1; 
//}
//
//void backward(){
	//UART_putstring("Gone Back"); //Print distance in centimeter
	//PORTD &= ~(1 << PORTD4);
	//PORTD |= (1 << PORTD3);
	//direction = 0; 
//}

int main(void)
{

	Intialize();
	UART_init(UART_BAUD_PRESCALER); 
	//forward();
	
	//UART_putstring("on \n");
	
	accel_read();
	char buffer[16];
	sprintf(buffer, "%d \n", stored_data[0]);
	UART_putstring(buffer);
	 while (1) {

 			//if (direction == 1) {
 		 			 			//if (out == 1) {
 		 			 					////Print distance in centimeter
 		 			 					//PORTB |= (1<<PORTB2);     // Signal Out
 		 			 					//_delay_us(10);
 		 			 					//PORTB &= ~(1<<PORTB2);
 		 			 					//_delay_us(10);  // Signal Out
 		 			 					//out = 0;
 		 			 				//}
 			//} else if (direction == 0) {
 				//
 				 				//if (out2 == 1) {
 				 				    ////Print distance in centimeter
 				 					//PORTB |= (1<<PORTB5);     // Signal Out
 				 					//_delay_us(10);
 				 					//PORTB &= ~(1<<PORTB5);
 				 					//_delay_us(10);  // Signal Out
 				 					//out2 = 0;
 				 				//}
				//
				//
			//}
			
			

	 }

}












