#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdbool.h>
#include "neuron.h"
#include "layer.h"
#include "usart.h"

// Number of cells in system
#define NUM_CELLS 4
#define CHARGE 99U

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
    double time_elapsed;
    uint16_t current;
    bool is_charging;
    uint16_t state_of_charge;
	Cell cells[NUM_CELLS];
} Battery;

Battery init_battery(void);
void get_soc(Battery *const battery);
void init_model(void);

#endif /* BATTERY_H_ */
