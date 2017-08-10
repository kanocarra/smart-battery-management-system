/**
  ******************************************************************************
  * File Name          : bms_controller.c
  * Description        : This file provides code to control the LTC6804 BMS 
  						 chip over SPI
  ******************************************************************************
*/

#include "bms.h"

// Writes the config registers of the LTC6804-2
void write_config_6804_2(void)
{
	//Holds the formatted config data
	uint8_t data[6] = {0};

	//Holds the command
	uint16_t command = 0;

	//Format the command
	command = LTC6804_2_ADDRESS_MODE<<15 | LTC6804_2_ADDRESS<<11 | WRCFG;

	//Populate the data
	data[0] = config_6804_buffer.GPIO5<<3 | config_6804_buffer.REFON<<2 | config_6804_buffer.SWTRD<<1 | config_6804_buffer.ADCOPT;
	data[1] = config_6804_buffer.VUVLB;
	data[2] = config_6804_buffer.VOVLB<<4|config_6804_buffer.VUVUB;
	data[3] = config_6804_buffer.VOVUB;
	data[4] = config_6804_buffer.DCCLB;
	data[5] = config_6804_buffer.DCTO<<4|config_6804_buffer.DCCUB;

	//Call the spi_write_word function
	SPI_transmit_word(command, data);

	return;
}

// Set the OV and UV levels in the config buffer
void set_UV_OV_threshold(void)
{
	//Need to check this works
	config_6804_buffer.VUVUB = (uint8_t)((UNDERVOLTAGE_ALARM_LEVEL&0xF00)>>8);
	config_6804_buffer.VUVLB = (uint8_t)(UNDERVOLTAGE_ALARM_LEVEL&0x0FF);

	//Set the OV threshold
	config_6804_buffer.VOVUB = (uint8_t)((OVERVOLTAGE_ALARM_LEVEL&0xFF0)>>4);
	config_6804_buffer.VOVLB = (uint8_t)(OVERVOLTAGE_ALARM_LEVEL&0x00F);

	//Update the 6804-2 chip
	write_config_6804_2();

	return;
}

// Read the status register block A and stores it in the status buffer
StatusA* read_status_A_6804_2(void)
{
	uint16_t command = 0;
	//Format the command
	command = LTC6804_2_ADDRESS_MODE<<15 | LTC6804_2_ADDRESS<<11 | RDSTATA;

	//Call the spi_write_word function, use a NULL pointer for read only
	SPI_transmit_word(command, NULL);

	//Read the status from the RX buffer
	status_regA.SOCLB	= SPI_recieve_buffer[0];
	status_regA.SOCUB	= SPI_recieve_buffer[1];
	status_regA.ITMPLB	= SPI_recieve_buffer[2];
	status_regA.ITMPUB	= SPI_recieve_buffer[3];
	status_regA.VALB	= SPI_recieve_buffer[4];
	status_regA.VAUB	= SPI_recieve_buffer[5];

	return &status_regA;
}

// Read the status register block B and stores it in the status buffer
StatusB* read_status_B_6804_2(void)
{
	uint16_t command = 0;
	//Format the command
	command = LTC6804_2_ADDRESS_MODE<<15 | LTC6804_2_ADDRESS<<11 | RDSTATB;

	//Call the spi_write_word function, use a NULL pointer for read only
	SPI_transmit_word(command, NULL);

	//Read the status from the RX buffer
	status_regB.VDLB = SPI_recieve_buffer[0];
	status_regB.VDUB = SPI_recieve_buffer[1];

	status_regB.C1UV = (SPI_recieve_buffer[2] & 0x01<<0)>>0;
	status_regB.C1OV = (SPI_recieve_buffer[2] & 0x01<<1)>>1;
	status_regB.C2UV = (SPI_recieve_buffer[2] & 0x01<<2)>>2;
	status_regB.C2OV = (SPI_recieve_buffer[2] & 0x01<<3)>>3;
	status_regB.C3UV = (SPI_recieve_buffer[2] & 0x01<<4)>>4;
	status_regB.C3OV = (SPI_recieve_buffer[2] & 0x01<<5)>>5;
	status_regB.C4UV = (SPI_recieve_buffer[2] & 0x01<<6)>>6;
	status_regB.C4OV = (SPI_recieve_buffer[2] & 0x01<<7)>>7;


	status_regB.C5UV = (SPI_recieve_buffer[3] & 0x01<<0)>>0;
	status_regB.C5OV = (SPI_recieve_buffer[3] & 0x01<<1)>>1;
	status_regB.C6UV = (SPI_recieve_buffer[3] & 0x01<<2)>>2;
	status_regB.C6OV = (SPI_recieve_buffer[3] & 0x01<<3)>>3;
	status_regB.C7UV = (SPI_recieve_buffer[3] & 0x01<<4)>>4;
	status_regB.C7OV = (SPI_recieve_buffer[3] & 0x01<<5)>>5;
	status_regB.C8UV = (SPI_recieve_buffer[3] & 0x01<<6)>>6;
	status_regB.C8OV = (SPI_recieve_buffer[3] & 0x01<<7)>>7;

	status_regB.C9UV = (SPI_recieve_buffer[4] & 0x01<<0)>>0;
	status_regB.C9OV = (SPI_recieve_buffer[4] & 0x01<<1)>>1;
	status_regB.C10UV = (SPI_recieve_buffer[4] & 0x01<<2)>>2;
	status_regB.C10OV = (SPI_recieve_buffer[4] & 0x01<<3)>>3;
	status_regB.C11UV = (SPI_recieve_buffer[4] & 0x01<<4)>>4;
	status_regB.C11OV = (SPI_recieve_buffer[4] & 0x01<<5)>>5;
	status_regB.C12UV = (SPI_recieve_buffer[4] & 0x01<<6)>>6;
	status_regB.C12OV = (SPI_recieve_buffer[4] & 0x01<<7)>>7;

	status_regB.THSD = (SPI_recieve_buffer[5] & 0x01<<0)>>0;
	status_regB.MUXFAIL = (SPI_recieve_buffer[5] & 0x01<<1)>>1;
	status_regB.RSVD = (SPI_recieve_buffer[5] & 0x03<<2)>>2;
	status_regB.REV = (SPI_recieve_buffer[5] & 0x0F<<4)>>4;
	
	return &status_regB;
}

// Transmit over SPI
void SPI_transmit_word(uint16_t cmd, uint8_t *data)
{
	//Inputs, cmd is the command to be used, data is the 6 bytes of data to be sent

	//This function transmits the contents of the SPI buffer
	//The data is sent in 12 byte frames
	//CMD0  CMD1  PEC0  PEC1  DATA5  DATA4  DATA3  DATA2  DATA1  DATA0  PEC0  PEC1
	//0     1     2     3     4      5      6      7      8      9      10    11

	//Local PEC
	uint16_t temp_pec;

	//place the command in the array
	SPI_transmit_buffer[0] = (uint8_t)(cmd >> 8);
	SPI_transmit_buffer[1] = (uint8_t)(cmd);

	//Calculate the command checksum
	temp_pec = pec15_calc(0,2, SPI_transmit_buffer);
	SPI_transmit_buffer[2] = (uint8_t)(temp_pec >> 8);
	SPI_transmit_buffer[3] = (uint8_t)(temp_pec);

	if(data == NULL)
	{
		//This is used for read only operations
		SPI_transmit_buffer[4] = 0x00;
		SPI_transmit_buffer[5] = 0x00;
		SPI_transmit_buffer[6] = 0x00;
		SPI_transmit_buffer[7] = 0x00;
		SPI_transmit_buffer[8] = 0x00;
		SPI_transmit_buffer[9] = 0x00;
	}
	else
	{
		//This is used when writing
		//Place the data in the array
		SPI_transmit_buffer[4] = data[0];
		SPI_transmit_buffer[5] = data[1];
		SPI_transmit_buffer[6] = data[2];
		SPI_transmit_buffer[7] = data[3];
		SPI_transmit_buffer[8] = data[4];
		SPI_transmit_buffer[9] = data[5];

	}

	//Calculate the second checksum start at 4 length of 6
	temp_pec = pec15_calc(4,6, SPI_transmit_buffer);
	SPI_transmit_buffer[10] = (uint8_t)(temp_pec >> 8);
	SPI_transmit_buffer[11] = (uint8_t)(temp_pec);

	//Wake up the LTC6804
	Slave_select(0);

	//Delay to ensure wakeup
	HAL_Delay(0.5);

	//Send the command bytes
	SPI_transmit(SPI_transmit_buffer[0]);
	SPI_transmit(SPI_transmit_buffer[1]);

	//Send the checksum
	SPI_transmit(SPI_transmit_buffer[2]);
	SPI_transmit(SPI_transmit_buffer[3]);

	//Send the data
	//Store the returned data in an array
	SPI_recieve_buffer[0] = SPI_transmit(SPI_transmit_buffer[4]);
	SPI_recieve_buffer[1] = SPI_transmit(SPI_transmit_buffer[5]);
	SPI_recieve_buffer[2] = SPI_transmit(SPI_transmit_buffer[6]);
	SPI_recieve_buffer[3] = SPI_transmit(SPI_transmit_buffer[7]);
	SPI_recieve_buffer[4] = SPI_transmit(SPI_transmit_buffer[8]);
	SPI_recieve_buffer[5] = SPI_transmit(SPI_transmit_buffer[9]);

	//Send the second checksum
	SPI_transmit(SPI_transmit_buffer[10]);
	SPI_transmit(SPI_transmit_buffer[11]);

	//SS inactive high
	Slave_deselect();

	return;
}

// Read the cell voltage registers and places the data in an array
void ADC_read_cell_voltages(Battery *const battery)
{
	//Use a dummy block of data
	uint8_t data[6] = {0};

	//Tell the ADC to run
	//1000 0011 1110 0000
	//MD:[8:7] ADC mode - 26kHz filtered
	//DCP:4 Discharge permit on
	//CH[2:0] 000, all cells
	//SEE PAGE 50 in datasheet for more options
	SPI_transmit_word(0x83F0, data);

	//Wait for ADC to run
	//This can be replaced with a poll loop to improve speed
	HAL_Delay(250);

	//Request the Group A cell voltages
	SPI_transmit_word(0x8004, data);
	//Copy the values from the SPI rx buffer
	battery->cells[0].voltage = ((uint16_t)SPI_recieve_buffer[1] << 8) | SPI_recieve_buffer[0];
	battery->cells[1].voltage = ((uint16_t)SPI_recieve_buffer[3] << 8) | SPI_recieve_buffer[2];
	// //Request the Group B cell voltages
	// //Copy the values from the SPI rx buffer
	//battery->cells[3].voltage = ((uint16_t)SPI_recieve_buffer[1] << 8) | SPI_recieve_buffer[0];
	// battery->cells[4].voltage = ((uint16_t)SPI_recieve_buffer[3] << 8) | SPI_recieve_buffer[2];
	// battery->cells[5].voltage = ((uint16_t)SPI_recieve_buffer[5] << 8) | SPI_recieve_buffer[4];

	

	// //Request the Group C cell voltages
	SPI_transmit_word(0x8008, data);
	// //Copy the values from the SPI rx buffer
	battery->cells[2].voltage = (((uint16_t)SPI_recieve_buffer[1] << 8) & 0xff00) | (SPI_recieve_buffer[0] & 0xff);
	battery->cells[3].voltage = ((uint16_t)SPI_recieve_buffer[3] << 8) | SPI_recieve_buffer[2];
	//battery->cells[8].voltage = ((uint16_t)SPI_recieve_buffer[5] << 8) | SPI_recieve_buffer[4];

    sprintf(UART_transmit_buffer, "%i %i %i %i \n", battery->cells[0].voltage, battery->cells[1].voltage, battery->cells[2].voltage, battery->cells[3].voltage);
    UART_transmit_word(); 

	// //Request the Group D cell voltages
	// SPI_transmit_word(0x800A, data);
	// //Copy the values from the SPI rx buffer
	// battery->cells[9].voltage = ((uint16_t)SPI_recieve_buffer[1] << 8) | SPI_recieve_buffer[0];
	// battery->cells[10].voltage = ((uint16_t)SPI_recieve_buffer[3] << 8) | SPI_recieve_buffer[2];
	// battery->cells[11].voltage = ((uint16_t)SPI_recieve_buffer[5] << 8) | SPI_recieve_buffer[4];

}

// Read the voltage and current of each cell
void read_voltage_and_current(Battery *const battery){
	uint8_t data[6] = {0};

	uint16_t command = LTC6804_2_ADDRESS_MODE<<15 | LTC6804_2_ADDRESS<<11 | ADCVAX;

	SPI_transmit_word(command, NULL);

	HAL_Delay(250);

	//Request the Group A cell voltages
	SPI_transmit_word(0x8004, data);
	//Copy the values from the SPI rx buffer
	battery->cells[0].voltage = ((uint16_t)SPI_recieve_buffer[1] << 8) | SPI_recieve_buffer[0];
	battery->cells[1].voltage = ((uint16_t)SPI_recieve_buffer[3] << 8) | SPI_recieve_buffer[2];
	
	// //Request the Group C cell voltages
	SPI_transmit_word(0x8008, data);
	// //Copy the values from the SPI rx buffer
	battery->cells[2].voltage = (((uint16_t)SPI_recieve_buffer[1] << 8) & 0xff00) | (SPI_recieve_buffer[0] & 0xff);
	battery->cells[3].voltage = ((uint16_t)SPI_recieve_buffer[3] << 8) | SPI_recieve_buffer[2];

	command = LTC6804_2_ADDRESS_MODE<<15 | LTC6804_2_ADDRESS<<11 | RDAUXA;
	
	SPI_transmit_word(command, NULL);
	
	battery->current = ((uint16_t)SPI_recieve_buffer[1] << 8) | SPI_recieve_buffer[0];

}
	




