/*
 * Starter I2C.c
 *
 * Created: 4/21/2024 2:42:13 PM
 * Author : willi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


void initialize(){
	//Power Register
	PRR0 &= ~(1 << PRTWI0);
	//Enable TWI0
	TWCR0 |= (1 << TWEN);
	//PC4 -> SDA0
	//PC5 -> SCL0
	//TWI Interrupt
	TWCR0 |= (1 << TWIE);
}


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

