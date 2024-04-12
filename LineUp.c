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

#define UART_BAUD_RATE      9600
#define UART_BAUD_PRESCALER (((F_CPU / (UART_BAUD_RATE * 16UL))) - 1)

volatile uint32_t val = 0; 
volatile int direction = 0; 
static double consVel = 10.0; 

#define time_step 0.02
//double V_naught = 0;
double V_Total = 0;
//double X_Total = 0;

int check = 0;




void Intialize() {
		cli(); 
	//-------------------------------------------Direction Control HBridge--------------------------------// 	
		DDRC |= (1 << DDRC1) | (1 << DDRC2) | (1 << DDRC3) | (1 <<DDRC4); //C1 -> pMOS 1, C2 -> pMOS 2, c3 -> nMOS 3, c4 -> nMOS 4
		PORTC |= (1 << PORTC1) | (1 << PORTC2);
		PORTC &= ~(1 << PORTC3) & ~(1 << PORTC4);

// 		//TESTING CODE
// 		
// 		// Timer0, prescale of 1024
// 		TCCR0B |= (1 << CS00);
// 		//TCCR0B |= (1 << CS01);
// 		TCCR0B |= (1 << CS02);
// 		
// 		//Overflow interrupt
// 		TIMSK0 |= (1 << TOIE0)		
 	//--------------------------------------------------Acclerometer--------------------------//	
		PRR0 &= ~(1 << PRADC);

		// Select Vref
		ADMUX |= (1 << REFS0);


		// Set the ADC clock div by 128
		// 16M/128=125kHz
		ADCSRA |= (1 << ADPS0);
		ADCSRA |= (1 << ADPS1);
		ADCSRA |= (1 << ADPS2);

		// Select Channel ADC0 (pin C0)
		ADMUX &= ~(1 << MUX0);
		ADMUX &= ~(1 << MUX1);
		ADMUX &= ~(1 << MUX2);
		ADMUX &= ~(1 << MUX3);

		ADCSRA |= (1 << ADATE);   // Autotriggering of ADC

		// Free running mode ADTS[2:0] = 000
		ADCSRB &= ~(1 << ADTS0);
		ADCSRB &= ~(1 << ADTS1);
		ADCSRB &= ~(1 << ADTS2);

		// Disable digital input buffer on ADC pin
		DIDR0 |= (1 << ADC0D);
		
		// Enable ADC
		ADCSRA |= (1 << ADEN);

		// Start conversion
		ADCSRA |= (1 << ADSC);
		
		//------------------------------------------------Motor PWM------------------------------------// 
			DDRD |= (1<<DDD6);
			DDRD |= (1<<DDD5);
			
			//phase correct mode
			TCCR0A |= (1<<WGM00); 
			TCCR0A &= ~(1<<WGM01); 
			TCCR0B |= (1<<WGM02); 
			
			//prescaler 1
			TCCR0B |= (1 << CS00);
			TCCR0B &= ~(1 << CS01);
			TCCR0B &= ~(1 << CS02);

			//Clear compare match 
			TCCR0A |= (1 << COM0A0);
			TCCR0A &= ~(1 << COM0A1);
			
			TCCR0A |= (1 << COM0B0);
			TCCR0A |= (1 << COM0B1);

		   OCR0A = 40;              // Sets frequency, 400kHz
		sei(); 

}


void updateVelocity(double currSpeed) {
	if (abs(currSpeed) < consVel - 1) {
		OCR0B =  (3.222 * currSpeed) + 1;  // Sets duty cycle based on speed, less speed = more duty cycle 
	}  else {
		OCR0B = 40; //Stops motor 
	}
}





void Off() {
	PORTC |= (1 << PORTC1) | (1 << PORTC2);
	PORTC &= ~(1 << PORTC3) & ~(1 << PORTC4);
}

void Positive() {
	Off();
	PORTC &= ~(1 << PORTC1);
	PORTC |= (1 << PORTC4);
}

void Negative() {
	Off();
	PORTC &= ~(1 << PORTC2);
	PORTC |= (1 << PORTC3);
}



void ResetVelocity(){
	V_Total = 0;
}

// void ResetDistance(){
// 	X_Total = 0;
// }

double Calculate_Velocity(double unit_ratio){
	
	double acceleration = (ADC - 500.0) * unit_ratio;
	V_Total += acceleration*time_step;
	return V_Total;
}

// 
// double Calculate_Distance(){
// 	X_Total += ADC * ADC / 2.0 * time_step ;
// 	return X_Total;
// }



int main(void)
{
    Intialize(); 
	UART_init(UART_BAUD_PRESCALER);
	//Negative();
	//updateVelocity(10); 
	UART_putstring("Raw ADC Read:");
    while (1) 
  {
	  			double currVel = Calculate_Velocity(0.01);
				int currentAdcRead = currVel;
				char rawAdcStringBuffer[10];                    // Buffer to hold the converted number
				itoa(currentAdcRead, rawAdcStringBuffer, 10);   // Convert integer to string
				UART_putstring("Raw Vel Read:\t");
				UART_putstring(rawAdcStringBuffer);
				UART_putstring( "\r\n"); 				
				//OCR0B = (0.039 * ADC);   // Sets duty cycle,
	
				
			//	updateVelocity(currVel); 
				 
    }
}

