
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <stdlib.h>
#include <stdio.h>

#define F_CPU                16000000UL   // 16MHz clock
#define USART_BAUD_RATE      9600
#define USART_BAUD_PRESCALER (((F_CPU / (USART_BAUD_RATE * 16UL))) - 1)
#define time_out 12800
#define testAddress 0x68
#define testRegister 0x43
#define G = 9.8

#include <util/delay.h>

uint8_t status = 0xF8;
uint8_t stored_data[16];
uint8_t phrase = 0;

//READ & WRITE TEST REGISTERS
#define who_Am_I 0x75
//also read uses powerManagement
#define power_Default 0x40
#define who_Am_I_Default 0x68

//reset registers and values
#define powerManagement 0x6B
#define resetRegisters 0x40
#define gyroClock 0x01

//Accelerometer registers
#define accel_X_H 0x3B
#define accel_X_L 0x3C
#define accel_Y_H 0x3D
#define accel_Y_L 0x3E
#define accel_Z_H 0x3F
#define accel_Z_L 0x40
#define sample_Rate_Divider 0x19
#define acc_Config_Reg 0x1C
//Gyro registers
#define gyro_X_H 0x43
#define gyro_Config_Reg 0x1B

//Accelerometer values
#define two_G 0x00
#define four_G 0x08
#define eight_G 0x10
#define sixteen_G 0x18

#define one_kHz 0x07

//Gyroscope values
#define two_fifty 0x00
#define five_hundred 0x08
#define one_thousand 0x10
#define two_thousand 0x18


/*---------------------------------------------------------------------------------------------------------------------

SEND START SIGNAL

------------------------------------------------------------------------------------------------------------------------*/


static int twi_start(){
	int i = 0;
	TWCR0 = (1 << TWINT) | (1 <<TWSTA) | (1 << TWEN) | (1 << TWIE);
	
	while(status != (TW_START)){
		i++;
		if (i>= time_out){
			UART_putstring("Status Register was not TW_START \n");
			return 1;
		}
	}
	return 0;
}

/*--------------------------------------------------------------------------------------------------------------------

SEND REPEATED START SIGNAL

----------------------------------------------------------------------------------------------------------------------*/

static int twi_rep_start(){
	int i = 0;
		TWCR0 = (1 << TWINT) | (1 <<TWSTA) | (1 << TWEN) | (1 << TWIE);
		
		while(status != (TW_REP_START)){
			i++;
			if (i>= time_out){
				UART_putstring("Status Register was not TW_REP_START \n");
				return 1;
			}
		}
		return 0;
}

/*---------------------------------------------------------------------------------------------------------------------

STOP TRANSMISSION

-------------------------------------------------------------------------------------------------------------------------*/


static void twi_stop(){
	TWCR0 = (1 << TWINT) | (1 <<TWSTO) | (1 << TWEN) | (1 << TWIE);
	
}


/*-----------------------------------------------------------------------------------------------------------

CHECK ADDRESS WRITE ACK

-------------------------------------------------------------------------------------------------------------*/


static int twi_addr_write_ack() {
		int i = 0;
		TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		
		while(status != (TW_MT_SLA_ACK)){
			i++;
			if (i>= time_out){
				UART_putstring("Status Register was not TW_MT_SLA_ACK \n");
				return 1;
			}
		}
		return 0;
}


/*-------------------------------------------------------------------------------------------------------

CHECK ADDRESS READ ACK

---------------------------------------------------------------------------------------------------------*/


static int twi_addr_read_ack() {
		int i = 0;
		TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		
				while(status != (TW_MR_SLA_ACK)){
					i++;
					if (i>= time_out){
						UART_putstring("Status Register was not TW_MR_SLA_ACK \n");
						return 1;
					}
				}
				return 0;
}


/*

CHECK DATA WRITE ACK

*/


static int twi_data_write_ack() {
	int i = 0;
	TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
	
	while(status != (TW_MT_DATA_ACK)){
		i++;
		if (i>= time_out){
			UART_putstring("Status Register was not TW_MT_DATA_ACK \n");
			return 1;
		}
	}
	return 0;
}

/*----------------------------------------------------------------------------------------------------------

CHECK DATA READ ACK

-----------------------------------------------------------------------------------------------------------*/


static int twi_data_read_ack() {
	int i = 0;
	TWCR0 = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
	
	while(status != (TW_MR_DATA_ACK)){
		i++;
		if (i>= time_out){
			UART_putstring("Status Register was not TW_MR_DATA_ACK \n");
			return 1;
		}
	}
	return 0;
}

/*----------------------------------------------------------------------------------------------

CHECK DATA READ NACK (END BIT)

--------------------------------------------------------------------------------------------------*/

static int twi_data_read_nack() {
	int i = 0;
	TWCR0 = ((1 << TWINT) | (1 << TWEN) | (1 << TWIE)) & ~(1 << TWEA);
	
	while(status != (TW_MR_DATA_NACK)){
		i++;
		if (i>= time_out){
			UART_putstring("Status Register was not TW_MR_DATA_NACK \n");
			return 1;
		}
	}
	return 0;
}


/*-------------------------------------------------------------------------------------------------------

SINGLE READ FUNCTION

--------------------------------------------------------------------------------------------------------*/


int twi_read(uint8_t addr, uint8_t reg){
	int check = 0;
	check = twi_start();
	
	if (check != 0) {
		twi_stop();
		UART_putstring("Start failed \n");
		return check;
	}
	TWDR0 = (addr << 1) | 0;
	
	check = twi_addr_write_ack();
	if(check != 0) {
		twi_stop();
		UART_putstring("Address write failed \n");
		return check;
	}
	
	TWDR0 = reg;
	
	check = twi_data_write_ack();
	if(check != 0) {
		twi_stop();
		UART_putstring("Reg write failed \n");
		return check;
	}
	
	check = twi_rep_start();
	if(check != 0) {
		twi_stop();
		UART_putstring("Repeated start failed \n");
		return check;
	}

	TWDR0 = (addr << 1) | 1;
	
	check = twi_addr_read_ack();
	if(check != 0) {
		twi_stop();
		UART_putstring("Address read failed \n");
		return check;
	}	
	
	check = twi_data_read_nack();
	if(check != 0){
		twi_stop();
		UART_putstring("Data read failed \n");
		return check;
	}
	
	stored_data[0] = TWDR0;
	
	twi_stop();
	
//	UART_putstring("Read successful \n");
	
	return 0;	
}


/*----------------------------------------------------------------------------------------------------------

BURST READ FUNCTION

-------------------------------------------------------------------------------------------------------------*/


int twi_burst_read(uint8_t addr, uint8_t reg, uint8_t num){
	int check = 0;
	check = twi_start();
	
	if (check != 0) {
		twi_stop();
		UART_putstring("Start failed \n");
		return check;
	}
	TWDR0 = (addr << 1) | 0;
	
	check = twi_addr_write_ack();
	if(check != 0) {
		twi_stop();
		UART_putstring("Address write failed \n");
		return check;
	}
	
	TWDR0 = reg;
	
	check = twi_data_write_ack();
	if(check != 0) {
		twi_stop();
		UART_putstring("Reg write failed \n");
		return check;
	}
	
	check = twi_rep_start();
	if(check != 0) {
		twi_stop();
		UART_putstring("Repeated start failed \n");
		return check;
	}

	TWDR0 = (addr << 1) | 1;
	
	check = twi_addr_read_ack();
	if(check != 0) {
		twi_stop();
		UART_putstring("Address read failed \n");
		return check;
	}
	
	for (int i = 0; i < num - 1; i++) {
		check = twi_data_read_ack();
		if(check != 0) {
			twi_stop();
			UART_putstring("Burst data read failed\n");
			return check;
		}
		
		stored_data[i] = TWDR0;
		
	}
	
	check = twi_data_read_nack();
	if(check != 0){
		twi_stop();
		UART_putstring("Final data read failed \n");
		return check;
	}
	
	stored_data[num - 1] = TWDR0;
	
	
	twi_stop();
	
	//UART_putstring("Read successful \n");
	
	return 0;
}


/*--------------------------------------------------------------------------------------------------

WRITE FUNCTION
 
---------------------------------------------------------------------------------------------------*/


int twi_write(uint8_t addr, uint8_t reg, uint8_t data){
	//int i = 0; 
	int check = 0;
	check = twi_start();
	
	if (check != 0) {
		twi_stop();
		UART_putstring("Start failed \n");
		return check;
	}
	
	
	TWDR0 = (addr << 1);
	
	check = twi_addr_write_ack();
	if(check != 0) {
		twi_stop();
		UART_putstring("Address write failed \n");
		return check;
	}
	
	TWDR0 = reg;
	
	check = twi_data_write_ack();
	if(check != 0) {
		twi_stop();
		UART_putstring("Reg write failed \n");
		return check;
	}
	
	TWDR0 = data;
	
	check = twi_data_write_ack();
	if(check != 0) {
		twi_stop();
		UART_putstring("Data write failed \n");
		return check;
	}
	
	twi_stop();	
	UART_putstring("Write succeeded \n");
	return check;
	
}




/*--------------------------------------------------------------------------------------------------------------

INTERRUPT USED TO CHECK STATUS REGISTER

-------------------------------------------------------------------------------------------------------------------*/

ISR(TWI0_vect) {
	status = (TWSR0 & 0xF8);
}

/*----------------------------------------------------------------------------------------------------------

RESET REGISTERS

--------------------------------------------------------------------------------------------------------------*/

int reg_reset(){
	int check = 0;
	check = twi_write(testAddress, powerManagement, resetRegisters);
	
	if (check != 0) {
		UART_putstring("Reset registers failed \n");
		return check;
	}
	return check;
	
}

/*----------------------------------------------------------------------------------------------------

Set Clock to gyroscope x clock

-------------------------------------------------------------------------------------------------------*/

int set_Clock(){
	int check = 0;
	check = twi_write(testAddress, powerManagement, gyroClock);
	
	if (check != 0){
		UART_putstring("Setting gyro clock failed \n");
		return check;
	}
	
	check = twi_read(testAddress, powerManagement);
	
	if(stored_data[0] != gyroClock){
		UART_putstring("gyro clock was not written \n");
		check = 1;
	}
	
	return check;
}


/*----------------------------------------------------------------------------------------------------------------

READ ACCELEROMETER

------------------------------------------------------------------------------------------------------------------*/

int accel_read() {
	int check = 0;
	
	check = twi_burst_read(testAddress, accel_X_H, 12);
	
	
	if(check != 0) {
		UART_putstring("Reading accelerometer failed \n");
	}
	
	
	return check;
}


/*---------------------------------------------------------------------------

CONFIG ACCELEROMETER

-----------------------------------------------------------------------------*/

int accel_config() {
	int check = 0;
	check = twi_write(testAddress, acc_Config_Reg, sixteen_G);
	
	if (check != 0) {
		UART_putstring("Writing initial config failed \n");
		return check;
	}
	
	check = twi_read(testAddress, acc_Config_Reg);
	
	if (check != 0) {
		UART_putstring("Reading config post write failed \n");
		return check;
	}
	
	if (stored_data[0] != sixteen_G) {
		UART_putstring("Config post write was not the same as value written \n");
		twi_stop();
		check = 1;
		return check;
	}
	
	check = twi_write(testAddress, gyro_Config_Reg, two_thousand);
	
	if (check != 0) {
		UART_putstring("Writing Gyro Config failed \n");
		return check;
	}
	
	check = twi_write(testAddress, sample_Rate_Divider, one_kHz);
	
	
	if (check != 0) {
		UART_putstring("Writing sample_Rate_Divider failed \n");
		return check;
	}
	
	check  = twi_read(testAddress, sample_Rate_Divider);
	
	if (check != 0) {
		UART_putstring("Reading Sample Rate Divider failed \n");
		return check;
	}
	
	
	if (stored_data[0] != one_kHz) {
		UART_putstring("Sample rate post write was not the same as value written \n");
		check = 1;
	}
	
	
	return check;
}

/*-----------------------------------------------------------------------------------------------------------

READ FUNCTION CHECK

-------------------------------------------------------------------------------------------------------------*/

int read_Who_Am_I_test() {
	int check = 0;
	check = twi_read(testAddress, who_Am_I);
	
	if (check != 0) {
		return check;
	}
	
	
	if (stored_data[0] != who_Am_I_Default) {
		check = 1;
	}
	return check;	
}

int read_powerManagement_test() {
	int check = 0;
	
	check = reg_reset();
	
	if(check != 0) {
		UART_putstring("reset failed \n");
	}
	
	check = twi_read(testAddress, powerManagement);
	
	if (check != 0) {
		return check;
	}
	
	
	if (stored_data[0] != power_Default) {
		check = 1;
		
	}
	return check;
}