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

void Intialize() {
		cli(); 
		
		
// 	//--------------------------------------------------Acclerometer--------------------------//	
// 		// Setup for ADC (10bit = 0-1023)
// 		// Clear power reduction bit for ADC
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


int main(void)
{
    Intialize(); 
	UART_init(UART_BAUD_PRESCALER);
	updateVelocity(9); 
    while (1) 
    {
// 				int currentAdcRead = ADC;
// 				char rawAdcStringBuffer[10];                    // Buffer to hold the converted number
// 				itoa(currentAdcRead, rawAdcStringBuffer, 10);   // Convert integer to string
// 				UART_putstring("Raw ADC Read:\t");
// 				UART_putstring(rawAdcStringBuffer);
// 				UART_putstring( "\r\n"				
				//OCR0B = (0.039 * ADC);   // Sets duty cycle,
		 
    }
}

