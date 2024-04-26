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
#include <util/twi.h>

#define F_CPU               16000000UL   // 16MHz clock
#define UART_BAUD_RATE      9600
#define UART_BAUD_PRESCALER (((F_CPU / (UART_BAUD_RATE * 16UL))) - 1)

volatile int direction = 1; 


 int out = 1;
 volatile long int count = 0;
 volatile long int start = 0;
 volatile long int end = 0;
 volatile int16_t range = 0;
 
 volatile int out2 = 1;
 volatile long int count2 = 0;
 volatile long int start2 = 0;
 volatile long int end2 = 0;
 volatile int16_t range2 = 0;

	#define OFF 67
	#define ON 25

	#define OFF2 67
	#define ON2 25
	
	#define gyroScale 131

	volatile int drop = 1;	 
 
	double weight = 0.55; 
	int prev = 0; 
	int prev2 = 0; 
	
	#define time_step 0.02
	double V_naught = 0;
	double V_Total = 0;
	double X_Total = 0;


void initialize()	{
		cli();
		TWSR0 &= ~(1 << TWPS1) & ~(1 << TWPS0);
		TWBR0 = ((F_CPU/400000 -16)/2);
		//Power Register
		PRR0 &= ~(1 << PRTWI0);
		//Enable TWI0 & interrupt
		TWCR0 |= (1 << TWEN) | (1 << TWIE);
		
		//Internal Pull ups
		PORTC|= (1 << PORTC5);
		PORTC |= (1 << PORTC4);
		
		UART_init(USART_BAUD_PRESCALER);
		sei();
}
int16_t combineSignedBytes(char highByte, char lowByte) {
	// Convert the high byte to a short and shift left by 8 bits
	int16_t result = (int16_t)((highByte << 8) | lowByte);
	return result;
}

double Calculate_Velocity(double unit_ratio, int acc){
	double acceleration = (acc) * unit_ratio;
	V_Total += acceleration*time_step;
	return V_Total;
}



int main(void)
{
	initialize();
	int maincheck = 0;
	char buffer[8];
	
	maincheck = reg_reset();
	if(maincheck != 0){
		UART_putstring("Reg reset failed \n");
	}
	
	/*---------------------------------Testing Section----------------------------------*/
	maincheck = read_Who_Am_I_test();
	if(maincheck != 0){
		UART_putstring("Who Am I test failed \n");
	}
	
	sprintf(buffer, "Who Am I register is: %u \n", stored_data[0]);
	UART_putstring(buffer);
	
	
	maincheck = read_powerManagement_test();
	if(maincheck != 0){
		UART_putstring("Power management test failed \n");
	}
	
	maincheck = twi_read(testAddress, powerManagement);
	sprintf(buffer, "Power management register is: %u \n", stored_data[0]);
	UART_putstring(buffer);
	
	
	
	
	
	/*-------------------------Configuration Section---------------------------------*/
	maincheck = set_Clock();
	
	if (maincheck != 0){
		UART_putstring("Set clock failed \n");
	}
	

	maincheck = accel_config();
	
	if (maincheck != 0){
		UART_putstring("Sample Rate Divider Failed \n");
	}
	
	/*-------------------------Reading Accelerometer Loop-------------------------------*/
	while (1)
	{
		maincheck = accel_read();
		if(maincheck!= 0) {
			UART_putstring("Reading accelerometer failed \n");
			twi_stop();
			
		}
		
	//	UART_putstring("\n Accelerometer Values \n \n");
		
// 		for (int i = 0; i < 2; i++) {
// 			
// 			sprintf(buffer, "%d \n", stored_data[i]);
// 			UART_putstring(buffer);
// 			
// 		}
// 		
		int16_t acc = combineSignedBytes(stored_data[0], stored_data[1]);
		
		double true_acc = acc  * 0.0048;
		int true_acc_high = true_acc;
		int true_acc_low = abs((true_acc * 1000.0) - (true_acc_high * 1000));
		
		sprintf(buffer, "%d .  ", true_acc_high);
		UART_putstring(buffer);
		sprintf(buffer, "%d \n", true_acc_low);
		UART_putstring(buffer);
		//_delay_ms(2000);
	}
}
