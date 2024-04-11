/*
 * H Bridge Control.c
 *
 * Created: 4/9/2024 4:55:18 PM
 * Author : willi
 */ 

/*    Circuit Orientation
*	     -----------
*     pMOS1        pMOS2
*       ----(+M-)----
*     nMOS3        nMOS4
*        -----------
*/


#include <avr/io.h>
#include <avr/interrupt.h>
int check = 0;



void Initialize(){
	cli();
	DDRC |= (1 << DDRC1) | (1 << DDRC2) | (1 << DDRC3) | (1 <<DDRC4); //C1 -> pMOS 1, C2 -> pMOS 2, c3 -> nMOS 3, c4 -> nMOS 4 
	PORTC |= (1 << PORTC1) | (1 << PORTC2);
	PORTC &= ~(1 << PORTC3) & ~(1 << PORTC4);
	//TESTING CODE
	
	 // Timer0, prescale of 1024
	TCCR0B |= (1 << CS00);
	//TCCR0B |= (1 << CS01);
	TCCR0B |= (1 << CS02);
	
	//Overflow interrupt
	TIMSK0 |= (1 << TOIE0);
	sei();

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


//ISR(TIMER0_OVF_vect){
	//if(check > 0){
		//check--;
		//Negative();
		//} else {
		//check++;
		//Positive();
	//}
//}


int main(void)
{
    /* Replace with your application code */
	Initialize();
	Negative();
    while (1) 
    {
    }
}

