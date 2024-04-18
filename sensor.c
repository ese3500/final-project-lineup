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

volatile uint32_t val = 0; 
volatile int direction = 1; 
static double consVel = 10.0; 

#define time_step 0.02
//double V_naught = 0;
double V_Total = 0;
//double X_Total = 0;

// int out = 1;
// volatile long int count = 0;
// volatile long int start = 0;
// volatile long int end = 0;
// volatile uint16_t range = 0;
// // 
// volatile int out2 = 1;
// volatile long int count2 = 0;
// volatile long int start2 = 0;
// volatile long int end2 = 0;
// volatile uint16_t range2 = 0;





void Intialize() {
		cli(); 
		
		
	//------------------------------------------------Ultrasonic 1------------------------------------------------------------------//
	

	//	DDRD |= (1 << DDD6);
		DDRB |= (1 << DDB2); //Output
// 
// 		// Timer1, prescale of 1024
// 		TCCR1B |= (1 << CS10);
// 		TCCR1B &= ~(1 << CS11);
// 		TCCR1B |= (1 << CS12);
// 		
// 		TCCR1B |= (1 << ICES1); //check rising edge
// 		TIMSK1 |= (1 << ICIE1); //enable input interrupt
// 		
		//------------------------------------------------Ultrasonic 2------------------------------------------------------------------//
// 		DDRB |= (1 << DDB5); //Output
// 		// Timer3, prescale of 1024
// 		TCCR3B |= (1 << CS30);
// 		TCCR3B &= ~(1 << CS31);
// 		TCCR3B |= (1 << CS32);
// 		
// 		TCCR3B |= (1 << ICES3); //check rising edge
// 		TIMSK3 |= (1 << ICIE3); //enable input interrupt
// 		
	//-------------------------------------------Direction Control HBridge--------------------------------// 	
// 		DDRC |= (1 << DDRC1) | (1 << DDRC2) | (1 << DDRC3) | (1 <<DDRC4); //C1 -> pMOS 1, C2 -> pMOS 2, c3 -> nMOS 3, c4 -> nMOS 4
// 		PORTC |= (1 << PORTC1) | (1 << PORTC2);
// 		PORTC &= ~(1 << PORTC3) & ~(1 << PORTC4);

 	//--------------------------------------------------Acclerometer--------------------------//	
// 		PRR0 &= ~(1 << PRADC);
// 
// 		// Select Vref
// 		ADMUX |= (1 << REFS0);
// 
// 
// 		// Set the ADC clock div by 128
// 		// 16M/128=125kHz
// 		ADCSRA |= (1 << ADPS0);
// 		ADCSRA |= (1 << ADPS1);
// 		ADCSRA |= (1 << ADPS2);
// 
// 		// Select Channel ADC0 (pin C0)
// 		ADMUX &= ~(1 << MUX0);
// 		ADMUX &= ~(1 << MUX1);
// 		ADMUX &= ~(1 << MUX2);
// 		ADMUX &= ~(1 << MUX3);
// 
// 		ADCSRA |= (1 << ADATE);   // Autotriggering of ADC
// 
// 		// Free running mode ADTS[2:0] = 000
// 		ADCSRB &= ~(1 << ADTS0);
// 		ADCSRB &= ~(1 << ADTS1);
// 		ADCSRB &= ~(1 << ADTS2);
// 
// 		// Disable digital input buffer on ADC pin
// 		DIDR0 |= (1 << ADC0D);
// 		
// 		// Enable ADC
// 		ADCSRA |= (1 << ADEN);
// 
// 		// Start conversion
// 		ADCSRA |= (1 << ADSC);
		
		//------------------------------------------------Motor PWM------------------------------------// 
// 					DDRD |= (1<<DDD6);
// 		 			DDRD |= (1<<DDD5);
// 		 			
// 		 			//phase correct mode
// 		 			TCCR0A |= (1<<WGM00); 
// 		 			TCCR0A &= ~(1<<WGM01); 
// 		 			TCCR0B |= (1<<WGM02); 
// 		 			
// 		 			//prescaler 1
// 		 			TCCR0B |= (1 << CS00);
// 		 			TCCR0B &= ~(1 << CS01);
// 		 			TCCR0B &= ~(1 << CS02);
// 		 
// 		 			//Clear compare match 
// 		 			TCCR0A |= (1 << COM0A0);
// 		 			TCCR0A &= ~(1 << COM0A1);
// 		 			
// 		 			TCCR0A |= (1 << COM0B0);
// 		 			TCCR0A |= (1 << COM0B1);
// 		 
// 		 		   OCR0A = 40;              // Sets frequency, 400kHz
//
// 		 		   TIMSK0 |= (1 << OCIE0A);
// 		 		   TIMSK1 |= (1 << OCIE0B);
 		sei(); 

}


//ULtrasonic 1----------------------------//
// ISR(TIMER1_CAPT_vect) {
// 	if (TCCR1B & (1 << ICES1)) {
// 		TCNT1 = 0;
// 		count = 0;
// 		start = TCNT1; 
// 		TCCR1B &= ~(1 << ICES1); //check falling edge
// 		} else {
// 		end = TCNT1;
// 		TCCR1B |= (1 << ICES1); //check rising edge
// 		
// 		range = (end - start);
// 		// Convert integer to string
// 		char s[20]; //  string representation of the integer
// 		itoa(range/2, s, 10);
// 		UART_putstring(s); //Print distance in centimeter
// 		UART_putstring( "\r\n");
// 		
// // 		if (range > 13) {
// // 			OCR0B = 0;
// // 		}else {
// // 			OCR0B = -3.54 * range + 43.54;
// // 		}
// // 		
// 		out = 1;
// 	}
// }


//Ultrasonic 2------------------------------------------//
// 
// ISR(TIMER3_CAPT_vect) {
// 	if (TCCR3B & (1 << ICES3)) {
// 	//	UART_putstring( "MAYBe");
// 		TCNT3 = 0;
// 		count2 = 0;
// 		start2 = TCNT3;
// 		TCCR3B &= ~(1 << ICES3); //check falling edge
// 		
// 		} else {
// 		end2 = TCNT3;
// 		TCCR3B |= (1 << ICES3); //check rising edge
// 		
// 		
// 		range2 = (end2 - start2);
// 		
// 		// Convert integer to string
// 		char s[20]; //  string representation of the integer
// 		itoa(range2/2, s, 10);
// 		UART_putstring(s); //Print distance in centimeter
// 		UART_putstring( "\r\n");
// // 		if (range2 > 13) {
// // 			OCR0B = 0;
// // 		} else {
// // 			OCR0B = -3.54 * range + 43.54;
// // 		}
// 		
// 		out2 = 1;
// 	}
// }


	//ISR(TIMER0_COMPB_vect){
		//if(V_Total < 40){
			//PORTC |= (1 << PORTC1) | (1 << PORTC2);
			//PORTC &= ~(1 << PORTC3) & ~(1 << PORTC4);
			//PORTC &= ~(1 << PORTC1);
			//PORTC |= (1 << PORTC4);
			//} else if (V_Total > 40) {
			//PORTC |= (1 << PORTC1) | (1 << PORTC2);
			//PORTC &= ~(1 << PORTC3) & ~(1 << PORTC4);
			//PORTC &= ~(1 << PORTC2);
			//PORTC |= (1 << PORTC3);
		//}
	//}
	//
	//ISR(TIMER0_COMPA_vect) {
		//PORTC |= (1 << PORTC1) | (1 << PORTC2);
		//PORTC &= ~(1 << PORTC3) & ~(1 << PORTC4);
	//}

// void Off() {
// 	PORTC |= (1 << PORTC1) | (1 << PORTC2);
// 	PORTC &= ~(1 << PORTC3) & ~(1 << PORTC4);
// }
// 
// void Positive() {
// 	Off();
// 	PORTC &= ~(1 << PORTC1);
// 	PORTC |= (1 << PORTC4);
// }

// void Negative() {
// 	Off();
// 	PORTC &= ~(1 << PORTC2);
// 	PORTC |= (1 << PORTC3);
// }
// // 
// void updateVelocity(double currSpeed) {
// 	
// 	if ((currSpeed < consVel - 1) && (currSpeed > 0)) {
// 		//Positive();
// 		OCR0B =  40 - currSpeed;  // Sets duty cycle based on speed, less speed = more duty cycle 
// 	} else if ((currSpeed > consVel + 1) && (currSpeed < 40)) {
// 		//Positive();
// 		OCR0B =  40 - currSpeed;
// 	} else if ((currSpeed > 40) && (currSpeed < 80)) {
// 	//	Negative();
// 		OCR0B = currSpeed - 40;
// 	} else if (currSpeed > 80) {
// 		//Negative();
// 		OCR0B = 40;
// 	} else if (currSpeed < 0) {
// 	//	Positive();
// 		OCR0B = 40;
// 	}
// 	
// 	
// 	else {
// 		OCR0B = 40; //Stops motor 
// 	}
// }

// 
// void ResetVelocity(){
// 	V_Total = 0;
// }

// void ResetDistance(){
// 	X_Total = 0;
// }

// double Calculate_Velocity(double unit_ratio){
// 	
// 	double acceleration = (ADC - 500.0) * unit_ratio;
// 	V_Total += acceleration*time_step;
// 	return V_Total;
// }

// 
// double Calculate_Distance(){
// 	X_Total += ADC * ADC / 2.0 * time_step ;
// 	return X_Total;
// }


int main(void)
{
	
	UART_init(UART_BAUD_PRESCALER);
	Intialize(); 

	//Positive();
	//updateVelocity(10); 
	//UART_putstring("Raw ADC Read:");
// 	Positive();
	
	//UART_putstring("Set Up");
	//OCR0B = 35;
	//UART_putstring( "HIII");
	 while (1) {
		 UART_putstring("HERE? \n");
	 }
   
}





// 			 				if (out == 1) {
// 			 					//UART_putstring("WORK1");
// 			 					//UART_putstring("HERE"); //Print distance in centimeter
// 			 					PORTB |= (1<<PORTB2);     // Signal Out
// 			 					_delay_us(10);
// 			 					PORTB &= ~(1<<PORTB2);
// 			 					_delay_us(10);  // Signal Out
// 			 					out = 0;
// 			 				}

						 	
		
// 	  			double currVel = Calculate_Velocity(0.005);
// 				int currentAdcRead = currVel;
// 				char rawAdcStringBuffer[10];                    // Buffer to hold the converted number
// 				itoa(currentAdcRead, rawAdcStringBuffer, 10);   // Convert integer to string
// 				UART_putstring("Raw Vel Read:\t");
// 				UART_putstring(rawAdcStringBuffer);
// 				UART_putstring( "\r\n"); 				
				//OCR0B = (0.039 * ADC);   // Sets duty cycle,

				//updateVelocity(currVel); 
			

			

// 				if (out2 == 1) {
// 					//UART_putstring("HERE"); //Print distance in centimeter
// 					PORTB |= (1<<PORTB5);     // Signal Out
// 					_delay_us(10);
// 					PORTB &= ~(1<<PORTB5);
// 					_delay_us(10);  // Signal Out
// 					out2 = 0;
// 				}
				
			
	
