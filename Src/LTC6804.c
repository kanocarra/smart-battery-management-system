#include "LTC6804.h"

//This function writes the config registers of the LTC6804-2
void write_config_6804_2(void)
{
	//Holds the formatted config data
	uint8_t data[6] = {0};

	//Holds the command
	uint16_t command = 0;

	//Format the command
	command = LTC6804_2_ADDRESS_MODE<<15 | LTC6804_2_ADDRESS<<11 | WRCFG;

	//Populate the data
	data[0] = config_6804_2_buffer.GPIO5<<3 | config_6804_2_buffer.REFON<<2 | config_6804_2_buffer.SWTRD<<1 | config_6804_2_buffer.ADCOPT;
	data[1] = config_6804_2_buffer.VUVLB;
	data[2] = config_6804_2_buffer.VOVLB<<4|config_6804_2_buffer.VUVUB;
	data[3] = config_6804_2_buffer.VOVUB;
	data[4] = config_6804_2_buffer.DCCLB;
	data[5] = config_6804_2_buffer.DCTO<<4|config_6804_2_buffer.DCCUB;

	//Call the spi_write_word function
	SPI_transmit_word(command, data);

	return;
}

//This function sets the OV and UV levels in the config buffer
void set_UV_OV_threshold(void)
{
	//Need to check this works
	config_6804_2_buffer.VUVUB = (uint8_t)((UNDERVOLTAGE_ALARM_LEVEL&0xF00)>>8);
	config_6804_2_buffer.VUVLB = (uint8_t)(UNDERVOLTAGE_ALARM_LEVEL&0x0FF);

	//Set the OV threshold
	config_6804_2_buffer.VOVUB = (uint8_t)((OVERVOLTAGE_ALARM_LEVEL&0xFF0)>>4);
	config_6804_2_buffer.VOVLB = (uint8_t)(OVERVOLTAGE_ALARM_LEVEL&0x00F);

	//Update the 6804-2 chip
	write_config_6804_2();

	return;
}

//This function reads the status register block A and stores it in the status buffer
uint8_t read_status_A_6804_2(void)
{
	uint16_t command = 0;
	//Format the command
	command = LTC6804_2_ADDRESS_MODE<<15 | LTC6804_2_ADDRESS<<11 | RDSTATA;

	//Call the spi_write_word function, use a NULL pointer for read only
	SPI_transmit_word(command, NULL);

	//Read the status from the RX buffer
	status_A_6804_2.SOCLB	= SPI_recieve_buffer[0];
	status_A_6804_2.SOCUB	= SPI_recieve_buffer[1];
	status_A_6804_2.ITMPLB	= SPI_recieve_buffer[2];
	status_A_6804_2.ITMPUB	= SPI_recieve_buffer[3];
	status_A_6804_2.VALB	= SPI_recieve_buffer[4];
	status_A_6804_2.VAUB	= SPI_recieve_buffer[5];

	return RETURN_VALUE_SUCCESS;
}

//This function reads the status register block B and stores it in the status buffer
uint8_t read_status_B_6804_2(void)
{
	uint16_t command = 0;
	//Format the command
	command = LTC6804_2_ADDRESS_MODE<<15 | LTC6804_2_ADDRESS<<11 | RDSTATB;

	//Call the spi_write_word function, use a NULL pointer for read only
	SPI_transmit_word(command, NULL);

	//Read the status from the RX buffer
	status_B_6804_2.VDLB = SPI_recieve_buffer[0];
	status_B_6804_2.VDUB = SPI_recieve_buffer[1];

	status_B_6804_2.C1UV = (SPI_recieve_buffer[2] & 0x01<<0)>>0;
	status_B_6804_2.C1OV = (SPI_recieve_buffer[2] & 0x01<<1)>>1;
	status_B_6804_2.C2UV = (SPI_recieve_buffer[2] & 0x01<<2)>>2;
	status_B_6804_2.C2OV = (SPI_recieve_buffer[2] & 0x01<<3)>>3;
	status_B_6804_2.C3UV = (SPI_recieve_buffer[2] & 0x01<<4)>>4;
	status_B_6804_2.C3OV = (SPI_recieve_buffer[2] & 0x01<<5)>>5;
	status_B_6804_2.C4UV = (SPI_recieve_buffer[2] & 0x01<<6)>>6;
	status_B_6804_2.C4OV = (SPI_recieve_buffer[2] & 0x01<<7)>>7;


	status_B_6804_2.C5UV = (SPI_recieve_buffer[3] & 0x01<<0)>>0;
	status_B_6804_2.C5OV = (SPI_recieve_buffer[3] & 0x01<<1)>>1;
	status_B_6804_2.C6UV = (SPI_recieve_buffer[3] & 0x01<<2)>>2;
	status_B_6804_2.C6OV = (SPI_recieve_buffer[3] & 0x01<<3)>>3;
	status_B_6804_2.C7UV = (SPI_recieve_buffer[3] & 0x01<<4)>>4;
	status_B_6804_2.C7OV = (SPI_recieve_buffer[3] & 0x01<<5)>>5;
	status_B_6804_2.C8UV = (SPI_recieve_buffer[3] & 0x01<<6)>>6;
	status_B_6804_2.C8OV = (SPI_recieve_buffer[3] & 0x01<<7)>>7;

	status_B_6804_2.C9UV = (SPI_recieve_buffer[4] & 0x01<<0)>>0;
	status_B_6804_2.C9OV = (SPI_recieve_buffer[4] & 0x01<<1)>>1;
	status_B_6804_2.C10UV = (SPI_recieve_buffer[4] & 0x01<<2)>>2;
	status_B_6804_2.C10OV = (SPI_recieve_buffer[4] & 0x01<<3)>>3;
	status_B_6804_2.C11UV = (SPI_recieve_buffer[4] & 0x01<<4)>>4;
	status_B_6804_2.C11OV = (SPI_recieve_buffer[4] & 0x01<<5)>>5;
	status_B_6804_2.C12UV = (SPI_recieve_buffer[4] & 0x01<<6)>>6;
	status_B_6804_2.C12OV = (SPI_recieve_buffer[4] & 0x01<<7)>>7;

	status_B_6804_2.THSD = (SPI_recieve_buffer[5] & 0x01<<0)>>0;
	status_B_6804_2.MUXFAIL = (SPI_recieve_buffer[5] & 0x01<<1)>>1;
	status_B_6804_2.RSVD = (SPI_recieve_buffer[5] & 0x03<<2)>>2;
	status_B_6804_2.REV = (SPI_recieve_buffer[5] & 0x0F<<4)>>4;


	return RETURN_VALUE_SUCCESS;
}

//includes the returned data, tested OK
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

//This function reads the cell voltage registers and places the data in an array
void ADC_read_cell_voltages(void)
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
	Cell_array[0].Cell_voltage = ((uint16_t)SPI_recieve_buffer[1] << 8) | SPI_recieve_buffer[0];
	Cell_array[1].Cell_voltage = ((uint16_t)SPI_recieve_buffer[3] << 8) | SPI_recieve_buffer[2];
	Cell_array[2].Cell_voltage = ((uint16_t)SPI_recieve_buffer[5] << 8) | SPI_recieve_buffer[4];
	//Request the Group B cell voltages
	SPI_transmit_word(0x8006, data);
	//Copy the values from the SPI rx buffer
	Cell_array[3].Cell_voltage = ((uint16_t)SPI_recieve_buffer[1] << 8) | SPI_recieve_buffer[0];
	Cell_array[4].Cell_voltage = ((uint16_t)SPI_recieve_buffer[3] << 8) | SPI_recieve_buffer[2];
	Cell_array[5].Cell_voltage = ((uint16_t)SPI_recieve_buffer[5] << 8) | SPI_recieve_buffer[4];

	//For the 4 cell case
	if(NUM_CELLS < 8)
	{
		return;
	}

	//Request the Group C cell voltages
	SPI_transmit_word(0x8008, data);
	//Copy the values from the SPI rx buffer
	Cell_array[6].Cell_voltage = (((uint16_t)SPI_recieve_buffer[1] << 8) & 0xff00) | (SPI_recieve_buffer[0] & 0xff);
	Cell_array[7].Cell_voltage = ((uint16_t)SPI_recieve_buffer[3] << 8) | SPI_recieve_buffer[2];
	Cell_array[8].Cell_voltage = ((uint16_t)SPI_recieve_buffer[5] << 8) | SPI_recieve_buffer[4];

	//Request the Group D cell voltages
	SPI_transmit_word(0x800A, data);
	//Copy the values from the SPI rx buffer
	Cell_array[9].Cell_voltage = ((uint16_t)SPI_recieve_buffer[1] << 8) | SPI_recieve_buffer[0];
	Cell_array[10].Cell_voltage = ((uint16_t)SPI_recieve_buffer[3] << 8) | SPI_recieve_buffer[2];
	Cell_array[11].Cell_voltage = ((uint16_t)SPI_recieve_buffer[5] << 8) | SPI_recieve_buffer[4];
}

void read_voltage_and_current(void){
	uint8_t data[6] = {0};

	uint16_t command = LTC6804_2_ADDRESS_MODE<<15 | LTC6804_2_ADDRESS<<11 | ADCVAX;

	//SPI_transmit_word(command);

}
	




