#include "battery.h"

// Create the battery object with number of cells
Battery init_battery(void){	
	Battery battery = {
		.time_elapsed = 0,
		.current = 0,
		.is_charging = 0,
		.state_of_charge = 0 
	};

	for(int i = 0; i < NUM_CELLS; i++){
		Cell cell = {
			.voltage = 0,
			.internal_resistance = 0,
			.capacity = 0,
			.state_of_charge = 0
		};
		battery.cells[i] = cell;
	}  

	return battery;
}

