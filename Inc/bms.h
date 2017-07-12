#ifndef BMS_H_
#define BMS_H_

#include "spi.h"
#include "usart.h"
#include "PEC15.h"
#include "gpio.h"
#include <stdbool.h>


//LTC6804_2
#define LTC6804_2_ADDRESS		0b0000
#define LTC6804_2_ADDRESS_MODE	0b1

#define WRCFG	0b00000000001
#define RDCFG	0b00000000010
#define RDCVA	0b00000000100
#define RDCVB	0b00000000110
#define RDCVC	0b00000001000
#define RDCVD	0b00000001010
#define RDAUXA	0b00000001100
#define RDAUXB	0b00000001110
#define RDSTATA 0b00000010000	//Read Status Register Group A
#define RDSTATB 0b00000010010	//Read Status Register Group B
#define ADAX	0b10111100000	//Start GPIOs ADC Conversion and Poll Status (default all GPIOs and 2nd REF) mode 11 (27 Hz)
#define ADCVAX	0b10111111111	//Start combined cell votlage and GPIO1, GPIO2 conversion and poll status
#define CLRCELL 0b11100010001
#define CLRAUX	0b11100010010
#define CLRSTAT 0b11100010011
#define PLADC	0b11100010100
#define DIAGN 	0b11100010101

//Make #defines for the UV and OV levels
//Note: 12 bits long
#define OVERVOLTAGE_ALARM_LEVEL		2625	//4.2V --> Comparison voltage = VOV*16*100uV
#define UNDERVOLTAGE_ALARM_LEVEL	1874	//3.0 V --> Comparison voltage = (VUV+1)*16*100uV

// Number of cells in system
#define NUM_CELLS 4

//DATA STRUCTURES
//A struct that holds the status
struct status_A_6804_2_struct{
	uint8_t SOCLB;
	uint8_t SOCUB;
	uint8_t ITMPLB;
	uint8_t ITMPUB;
	uint8_t VALB;
	uint8_t VAUB;
};

struct status_B_6804_2_struct{
	uint8_t VDLB;
	uint8_t VDUB;
	uint8_t C4OV  : 1, C4UV  : 1, C3OV  : 1, C3UV  : 1, C2OV  : 1, C2UV  : 1, C1OV : 1, C1UV : 1;
	uint8_t C8OV  : 1, C8UV  : 1, C7OV  : 1, C7UV  : 1, C6OV  : 1, C6UV  : 1, C5OV : 1, C5UV : 1;
	uint8_t C12OV : 1, C12UV : 1, C11OV : 1, C11UV : 1, C10OV : 1, C10UV : 1, C9OV : 1, C9UV : 1;
	uint8_t REV   : 4, RSVD  : 2, MUXFAIL:1, THSD  : 1;
};

//Stores data for the cells
struct Cell_struct{
	//NOTE: Voltages are stored as unsigned ints, 16 bits, start at 0, 100uV per count
	uint16_t Cell_voltage;

	//Stores the current state of cell voltage
	//enum voltage_state Cell_voltage_state;

	//NOTE: temperatures are stored and processed as 10 bit ADC values, as they are easier to work with

	//DISPALAYED as singed 16 Bit, 10 m degC per count
	int16_t Cell_temp_degrees_C;

	//STORED as 10 (of 16) bit ADC values, start at 0V, 1024 = Vcc
	uint16_t Cell_temp_count;

	//Follow format for serial numbers
	//Index		Value
	//0			-1 = 200Ahr, 12V; -2 = 200Ahr, 12V
	//1			-A = Prototype; -B = Production
	//2			-Serial number
	//3			-Serial number
	//4			-Serial number
	//uint8_t	Cell_serial_no[CELL_SERIAL_NO_LENGTH];

	//Cell state of charge, need to consider starting at a less than zero value???
	//mC of charge to top
	uint32_t Cell_SOC;

	//Make 32 bit to fix bug consider a more memory efficient fix later
	int32_t Ballance_points;

	//Used to set the sensitivity of the balance algorithum
	//Consider smaller data size for SOC calculations??
	uint32_t Ballance_hystersis;
};

// Model for a cell
typedef struct  {
    double voltage; //in volts
    double internal_resistance; // in milliohms
    double capacity; // in mAhr
    double state_of_charge; // in %
} Cell;

// Model for parameters of the battery pack
typedef struct {
    double time_elapsed;
    double current;
    bool is_charging;
    double state_of_charge;
	Cell cells[NUM_CELLS];
} Battery;

//A struct that represents the config register block
typedef struct {
    uint8_t GPIO5 : 5, REFON : 1, SWTRD : 1, ADCOPT : 1;
	uint8_t VUVLB;
	uint8_t VOVLB  :4 ,   VUVUB:4;
	uint8_t VOVUB;
	uint8_t DCCLB;
	uint8_t DCTO :4,  DCCUB:4;
} Bms_Config;

//Use a global config array
volatile Bms_Config config_6804_buffer;

//Declare a global buffer for the two status structs
volatile struct status_A_6804_2_struct status_A_6804_2;
volatile struct status_B_6804_2_struct status_B_6804_2;

Battery init_battery(void);
void write_config_6804_2(void);
void set_UV_OV_threshold(void);
void set_GPIO_6804_2(uint8_t input);
uint8_t read_status_A_6804_2(void);
uint8_t read_status_B_6804_2(void);
void SPI_transmit_word(uint16_t cmd, uint8_t *data);
void ADC_read_cell_voltages(Battery* batter);


#endif /* BMS_H_ */
