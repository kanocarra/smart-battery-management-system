#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdbool.h>
#include "neuron.h"
#include "layer.h"

// Number of cells in system
#define NUM_CELLS 4

// Model for a cell
typedef struct {
    double voltage; //in volts
    double internal_resistance; // in milliohms
    double capacity; // in mAhr
    double state_of_charge; // in %
} Cell;

// Model for parameters of the battery pack
typedef struct Battery {
    double time_elapsed;
    double current;
    bool is_charging;
    double state_of_charge;
	Cell cells[NUM_CELLS];
} Battery;

Battery init_battery(void);

#endif /* BATTERY_H_ */
