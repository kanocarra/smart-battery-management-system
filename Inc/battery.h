#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdbool.h>
#include "neuron.h"
#include "layer.h"
#include "usart.h"

// Number of cells in system
#define NUM_CELLS 4
#define CHARGE 99U
#define DISCHARGE 100U
#define COMMS 's'

// Normalisation paramters
#define TIME_MAX 3728.188F
#define TIME_MIN 0.0F
#define VOLT_MAX 4.21F
#define VOLT_MIN 3.07F
#define CUR_MAX 0.002F
#define CUR_MIN -7.643F
#define CAP_MAX 7.769F
#define CAP_MIN 6.822F
#define RES_MAX 65.416F
#define RES_MIN 1.946F
#define NORM_MAX 1.0F
#define NORM_MIN -1.0F

#define DCC1 1
#define DCC2 2
#define DCC7 64
#define DCC8 128
#define BAL_VOTLAGE_MV 50


uint32_t total_seconds;

// Model for a cell
typedef struct {
    uint16_t voltage; //in volts
    double internal_resistance; // in milliohms
    double capacity; // in mAhr
    uint16_t state_of_charge; // in %
    char cell_number;
    uint16_t temperature;
} Cell;

// Model for parameters of the battery pack
typedef struct Battery {
    uint16_t time_elapsed;
    uint16_t current;
    bool is_charging;
    uint16_t state_of_charge;
	Cell cells[NUM_CELLS];
    uint8_t is_balancing;
} Battery;

Battery init_battery(void);
void get_soc(Battery *const battery);
void init_soc_model(void);
long double normalise_input(double max, double min, double value);
void start_cycle(void); 
uint16_t get_current_time(void); 
void get_time_elapsed(Battery *const battery);
uint8_t balance_cells(Battery *const battery);
void turn_off_balancing(Battery *const battery);

#endif /* BATTERY_H_ */
