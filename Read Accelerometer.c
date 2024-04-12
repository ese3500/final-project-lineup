/*
 * MotorControl.c
 *
 * Created: 4/9/2024 1:58:47 PM
 * Author : Zwe
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"
#include <avr/interrupt.h>
#define F_CPU               16000000UL   // 16MHz clock
#define UART_BAUD_RATE      9600
#define UART_BAUD_PRESCALER (((F_CPU / (UART_BAUD_RATE * 16UL))) - 1)


#define time_step 0.02
double V_naught = 0;
double V_Total = 0; 
double X_Total = 0;


void Intialize() {
		cli(); 
		
		
	//--------------------------------------------------Acclerometer--------------------------//	
		// Setup for ADC (10bit = 0-1023)
		// Clear power reduction bit for ADC
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
			//TCCR0B &= ~(1 << CS01);
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
		   
		   //-------------------------------------Timer 1 OVF-------------------------------------------------//
			////Set prescaler by 1024
			//TCCR1B &= ~(1 << CS10);
			//TCCR1B &= ~(1 << CS11);
			//TCCR1B &= ~(1 << CS12);
				//
			////Normal
			//TCCR1A &= ~(1 << WGM10);
			//TCCR1A &= ~(1 << WGM11);
			//TCCR1B &= ~(1 << WGM12);
			////overflow
			//TIMSK1 = (1 << TOIE1);
			 
			
		sei(); 

}


//ISR(TIMER1_OVF_vect) {
	//
	//
//}

void ResetVelocity(){
	V_Total = 0;
}

void ResetDistance(){
	X_Total = 0;
}

double Calculate_Velocity(double unit_ratio){
	double acceleration = ADC * unit_ratio;
	V_Total += acceleration*time_step;
	return V_Total;
}

double Calculate_Distance(){
	X_Total += ADC * ADC / 2.0 * time_step ;
	return X_Total;
}



int main(void)
{
    Intialize(); 
	UART_init(UART_BAUD_PRESCALER);
    while (1) 
    {
				double currentadcread = Calculate_Velocity(0.01);
				char rawadcstringbuffer[10];                    // buffer to hold the converted number
				itoa(currentadcread, rawadcstringbuffer, 10);   // convert integer to string
				UART_putstring("Velocity read:\t");
				UART_putstring(rawadcstringbuffer);
				UART_putstring( "\r\n");
				
				//OCR0B = (0.039 * ADC);   // Sets duty cycle,
		 
    }
}