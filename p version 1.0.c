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

 volatile int timesteps = 0;

void Intialize() {
		cli(); 
		//--------------------------------------------Servo 1---------------------------------------------------------------------//
			//Servo 1 will now use timer 0
			
			DDRD |= (1 << DDD6);      // Set PB3 to be an output pin

			// Timer0, prescaler 1024
			TCCR0B |= (1 << CS00);
			//TCCR1B |= (1 << CS11);
			TCCR0B |= (1 << CS02);

			// Timer0, Phase Correct PWM (mode 1)
			TCCR0A |= (1 << WGM00);
			
			//CLear on Compare
			TCCR0A |= (1 << COM0A1);
			TCCR0A &= ~(1 << COM0A0);
			
			OCR0A = OFF; //Duty Cycle
			
	//--------------------------------------------Servo 2---------------------------------------------------------------------//

	//------------------------------------------------Ultrasonic 1------------------------------------------------------------------//
		//will use timer 1
		DDRB |= (1 << DDB2); //Output
// 
 		//// Timer1, prescaler of 1024
 		//TCCR1B |= (1 << CS10);
 		//TCCR1B &= ~(1 << CS11);
 		//TCCR1B |= (1 << CS12);
 		
 		TCCR1B |= (1 << ICES1); //check rising edge
 		TIMSK1 |= (1 << ICIE1); //enable input interrupt
// 		
		//------------------------------------------------Ultrasonic 2------------------------------------------------------------------//


 	//--------------------------------------------------Accelerometer--------------------------//	

		
		//------------------------------------------------Motor PWM------------------------------------// 
 				//Motor, will use timer 2
				 
				 	//DDRB |= (1<<DDB3); //OC2A
 		 			DDRD |= (1<<DDD3); //OC2B
 		 			
 		 			//phase correct mode
 		 			TCCR2A |= (1<<WGM20); 
 		 			TCCR2A &= ~(1<<WGM21); 
 		 			TCCR2B |= (1<<WGM22); 
 		 			
 		 			//prescaler 64
 		 			TCCR2B |= (1 << CS20);
 		 			TCCR2B |= (1 << CS21); 
 		 			TCCR2B &= ~(1 << CS22);
 		 
 		 			//Toggle on Compare Match (We don't use pin)
 		 			//TCCR2A |= (1 << COM2A0);
 		 			//TCCR2A &= ~(1 << COM2A1);
 		 			
					 //set on compare match 
 		 			TCCR2A |= (1 << COM2B0);
 		 			TCCR2A |= (1 << COM2B1);
 		 
 		 		   OCR0A = 40;              // Sets frequency, 400kHz
					 
		//-------------------------------------------H Bridge-----------------------------------------//
		//in1 pin & in2 pin
		DDRD |= (1 << DDD0);
		DDRD |= (1 << DDD1);
		//forward
		PORTD |= (1 << PORTD1);
		PORTD |= (1 << PORTD0);		
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
 		// Convert integer to string
 		char s[20]; //  string representation of the integer
 		itoa(range, s, 10);
		UART_putstring("U1: ");
 		UART_putstring(s); //Print distance in centimeter
 		UART_putstring( "\r\n");
 		
  		if (range > 35) {
  			OCR2B = 10;
  		}else {
  			OCR2B = -range + 40;
  		}
 // 		
 		out = 1;
 	}
 }


//Ultrasonic 2------------------------------------------//






int main(void)
{
	
	UART_init(UART_BAUD_PRESCALER);
	Intialize(); 
	//forward();
	//backward();
	direction = 1; 
	OCR0B = 10;
	 while (1) {
		 //UART_putstring("Test \n");
		 //	 UART_putstring("WORKING");
			//if (direction == 1) {
		 			 				//if (out == 1) {
		 			 					////UART_putstring("WORK1");
		 			 					////UART_putstring("HERE"); //Print distance in centimeter
		 			 					//PORTB |= (1<<PORTB2);     // Signal Out
		 			 					//_delay_us(10);
		 			 					//PORTB &= ~(1<<PORTB2);
		 			 					//_delay_us(10);  // Signal Out
		 			 					//out = 0;
		 			 				//}
			//} else {
				//
				 				//if (out2 == 1) {
				 					////UART_putstring("HERE"); //Print distance in centimeter
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



/*-----------------------------------INITIALIZER CODE------------------------------------------------*/



		//------------------------------------------------Ultrasonic 2------------------------------------------------------------------//
		//DDRB |= (1 << DDB5); //Output
		//// Timer3, prescaler of 1024
		//TCCR3B |= (1 << CS30);
		//TCCR3B &= ~(1 << CS31);
		//TCCR3B |= (1 << CS32);
		//
		//TCCR3B |= (1 << ICES3); //check rising edge
		//TIMSK3 |= (1 << ICIE3); //enable input interrupt



	//--------------------------------------------Servo 2---------------------------------------------------------------------//
	//DDRD |= (1 << DDD1);      // Set PD1 to be an output pin
	//
	//// Timer4, prescaler  1024
	//TCCR4B |= (1 << CS40);
	////TCCR1B |= (1 << CS11);
	//TCCR4B |= (1 << CS42);
	//
	//// Timer4, Phase Correct PWM (mode 1)
	//TCCR4A |= (1 << WGM40);
	//TCCR4A |= (1 << WGM41);
	//TCCR4B &= ~(1 << WGM42);
	//TCCR4B |= (1 << WGM43);
	//
	////CLear on Compare
	//TCCR4A |= (1 << COM4A1);
	//TCCR4A &= ~(1 << COM4A0);
	//
	//OCR4A = 50; //Duty Cycle
	//



/*-----------------------------------WORKING CODE-----------------------------------------------------*/
//Ultrasonic 2------------------------------------------//

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
//
//// Convert integer to string
//char s[20]; //  string representation of the integer
//itoa(range2, s, 10);
//UART_putstring("U2: ");
//UART_putstring(s); //Print distance in centimeter
//UART_putstring( "\r\n");
//if (range2 > 13) {
//OCR0B = 10;
//} else {
//OCR0B = -3.54 * range2 + 43.54;
//}
//
//out2 = 1;
//}
//}


		//-------------------------------------------H Bridge-----------------------------------------//
		//DDRB |= (DDRB4); //forward pin
		//DDRB |= (DDRB1); //backward pin


					

/*---------------------------------------OLD CODE------------------------------------------------------*/		
// 	  			double currVel = Calculate_Velocity(0.005);
// 				int currentAdcRead = currVel;
// 				char rawAdcStringBuffer[10];                    // Buffer to hold the converted number
// 				itoa(currentAdcRead, rawAdcStringBuffer, 10);   // Convert integer to string
// 				UART_putstring("Raw Vel Read:\t");
// 				UART_putstring(rawAdcStringBuffer);
// 				UART_putstring( "\r\n"); 				
				//OCR0B = (0.039 * ADC);   // Sets duty cycle,

				//updateVelocity(currVel); 
			

			
 	//--------------------------------------------------Accelerometer--------------------------//
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
			
	//H Bridge Functions

	//void motorbreak(){
	//DDRB |= (1 << DDRB1);
	//DDRB |= (1 << DDRB4);
	//_delay_ms(100);
	//}
	//
	//void forward(){
	//DDRB &= ~(1 << DDRB1);
	//DDRB |= (1 << DDRB4);
	//}
	//
	//void backward(){
	//DDRB &= ~(1 << DDRB4);
	//DDRB |= (1 << DDRB1);
	//}
	
	/*-----------------------------------------Velocity Calcs-----------------------------*/
	
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
	
	
	//---------------------Servo1 Test----------------------------------------------//
	// ISR(TIMER2_OVF_vect){
	// 	// UART_putstring("WORK");
	// 	if(timesteps > 10){
	// 	timesteps = 0;
	// 	OCR2A = LEFT;
	// 	} else if (timesteps == 5) {
	// 	OCR2A = RIGHT;
	// 	timesteps++;
	// 	} else {
	// 	timesteps++;
	// 	}
	// }