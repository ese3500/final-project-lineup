/*
 * Servo Control.c
 *
 * Created: 4/9/2024 2:25:39 PM
 * Author : willi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define LEFT 3
#define RIGHT 19

int timesteps = 0;

void Initialize() {
	cli();
	DDRB |= (1 << DDB1);      // Set PB1 to be an output pin

	// Timer1, prescaler of 1
	TCCR1B |= (1 << CS10);
	//TCCR1B |= (1 << CS11);
	TCCR1B |= (1 << CS12);

	// Timer1, Phase Correct PWM (mode 1)
	TCCR1A |= (1 << WGM10);
	
	//CLear on Compare
	TCCR1A |= (1 << COM1A1);
	TCCR1A &= ~(1 << COM1A0);
	
	OCR1A = RIGHT; //Duty Cycle
	
	//Time Based Interrupt
	TIMSK1 |= (1 << OCIE1B);
	OCR1B = 65535;
	sei();
}

ISR(TIMER1_COMPB_vect){
	if(timesteps > 100){
		timesteps = 0;
		OCR1A = LEFT;
	} else if (timesteps == 50) {
		OCR1A = RIGHT;
		timesteps++;
	} else {
		timesteps++;
	}
}



int main(void) {
	Initialize();
	while (1)
	;
}
