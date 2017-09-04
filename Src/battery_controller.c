/**
  ******************************************************************************
  * File Name          : battery_controller.c
  * Description        : This file provides code for the battery model and
                        estimating the state of charge
  ******************************************************************************
*/

#include "battery.h"
#include "rtc.h"
#include "tim.h"


Layer* layers[N_LAYERS];
Layer input_layer;
Layer output_layer;

//Intialise nodes
long double sigmoid_node1_weights[MAX_INPUTS];
long double sigmoid_node2_weights[MAX_INPUTS];
long double sigmoid_node3_weights[MAX_INPUTS];
long double sigmoid_node1_bias;
long double sigmoid_node2_bias;
long double sigmoid_node3_bias;
// Initialise node
long double linear_node0_weights[3];
long double linear_node0_bias;
enum input{TIME_ELAPSED, VOLTAGE, CURRENT, CAPACITY, INTERNAL_RESISTANCE};
enum nodes{SM1, SM2, SM3};
char cell_identifiers[NUM_CELLS] = {'1', '2', '7', '8'};
double cell_capacities[NUM_CELLS] = {7.47651806373,7.43284105339,7.46789307815,7.50147635536};
double cell_ir[NUM_CELLS] = {3.0440087146,2.85555555556, 2.92527233115, 7.48954248366};
uint16_t start_time;

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
			.internal_resistance = cell_ir[i],
			.capacity = cell_capacities[i],
			.state_of_charge = 0,
            .cell_number = cell_identifiers[i],
            .temperature = 212
		};
		battery.cells[i] = cell;
	}  

	return battery;
}

void init_soc_model(){
    
    // Initialise hidden layer
    input_layer = init_layer();
    
    // Node 1
    sigmoid_node1_weights[TIME_ELAPSED] = 2.402974608182833;
    sigmoid_node1_weights[VOLTAGE] =  0.6247989307177287;
    sigmoid_node1_weights[CURRENT] = 0.5177655392866384;
    sigmoid_node1_weights[CAPACITY] = -0.41332625064237954;
    sigmoid_node1_weights[INTERNAL_RESISTANCE] = -0.0026848550716559684;
    sigmoid_node1_bias =  -2.854300632499299;
    add_neuron(sigmoid_node1_weights, &input_layer, 1, 5, sigmoid_node1_bias);
    
    //Node 2
    sigmoid_node2_weights[TIME_ELAPSED] =  1.937406595403493;
    sigmoid_node2_weights[VOLTAGE] =  -0.7000361379607273;
    sigmoid_node2_weights[CURRENT] = -0.591633593255045;
    sigmoid_node2_weights[CAPACITY] =  -0.15555135006119186;
    sigmoid_node2_weights[INTERNAL_RESISTANCE] = -0.005816391165762171;
    sigmoid_node2_bias =   -1.0056207683771963;
    add_neuron(sigmoid_node2_weights, &input_layer, 1, 5, sigmoid_node2_bias);
    
    //Node 3
    sigmoid_node3_weights[TIME_ELAPSED] = -2.357000962524356;
    sigmoid_node3_weights[VOLTAGE] = -0.1358046573815341;
    sigmoid_node3_weights[CURRENT] =  -0.05811834620115263;
    sigmoid_node3_weights[CAPACITY] = -0.015921542250270704;
    sigmoid_node3_weights[INTERNAL_RESISTANCE] = 0.006559453499680887;
    sigmoid_node3_bias =  -2.3009378756283105;
    add_neuron(sigmoid_node3_weights, &input_layer, 1, 5, sigmoid_node3_bias);

    layers[0] = &input_layer;
    
    // Initialise output layer
    output_layer = init_layer();
    
    // Output Node
    linear_node0_weights[SM1] =  -1.2324306598186197;
    linear_node0_weights[SM2] =    -1.3396042055728592;
    linear_node0_weights[SM3] = 1.4764981571210392;
    linear_node0_bias =   0.3642179801774134;
    add_neuron(linear_node0_weights, &output_layer, 0, 5, linear_node0_bias);

    layers[1] = &output_layer;
}

// Pass the inputs through the multiperceptron layer model
void get_soc(Battery *const battery) {

    for(int i= 0; i < NUM_CELLS; i++) {
        
        // Normalise inputs
        long double inputs[5] = {0,0,0,0,0};
        inputs[TIME_ELAPSED] = normalise_input(TIME_MAX, TIME_MIN, battery->time_elapsed);
        inputs[VOLTAGE] = normalise_input(VOLT_MAX, VOLT_MIN, (double)(battery->cells[i].voltage/10000.0));
        inputs[CURRENT] = normalise_input(CUR_MAX, CUR_MIN, ((double)(battery->current/1000.0)) * -1) ;
        inputs[CAPACITY] = normalise_input(CAP_MAX, CAP_MIN, battery->cells[i].capacity);
        inputs[INTERNAL_RESISTANCE] = normalise_input(RES_MAX, RES_MIN, battery->cells[i].internal_resistance);
       
       // Run the model        
        long double normalised_soc = compute_result(layers, inputs);
        long double soc = (((normalised_soc- -1.0) * 100.0) / 2.0);

        // Update state of charge estimate
        battery->cells[i].state_of_charge = (uint16_t)(soc * 100.0);
    }

}

// Normalises the input based on WEKA's normalisation of -1 to 1
long double normalise_input(double max, double min, double value) {
    long double old_range = (max- min);
    long double new_range = NORM_MAX - NORM_MIN;
    long double new_value = (((value - min) * new_range) / old_range) + NORM_MIN;
    return new_value;
}

void start_cycle(void) {
    total_seconds = 0;
    HAL_TIM_Base_Start_IT(&htim3);
}

uint16_t get_current_time(void) {
    if((HAL_RTC_GetTime(&hrtc,&RTCtime,RTC_FORMAT_BIN) == HAL_OK && HAL_RTC_GetDate(&hrtc,&RTCdate,RTC_FORMAT_BIN)) == HAL_OK)
    {
        uint16_t hours = RTCtime.Hours;
        uint16_t minutes =  RTCtime.Minutes;
        uint16_t seconds = RTCtime.Seconds;
        uint16_t subseconds = RTCtime.SubSeconds;
        uint16_t total = (60 * 60 * hours) + (60 * minutes) + seconds;
        return total;
    }
    return 0;
}

void get_time_elapsed(Battery *const battery) {
    uint16_t added_time = 0;
    battery->time_elapsed = total_seconds + added_time;
}

uint8_t balance_cells(Battery *const battery){ 
    uint16_t min_voltage = battery->cells[0].voltage;
    uint16_t voltage_dif = 0;
    uint8_t balance_reg = 0;
    uint8_t led_comb = 0;
    
    for(int i = 1; i< NUM_CELLS; i++) {
        if (battery->cells[i].voltage < min_voltage) {
            min_voltage = battery->cells[i].voltage;
        }
    }
    for(int i = 0; i < NUM_CELLS; i++){
        voltage_dif = battery->cells[i].voltage - min_voltage;
        // Determines the bit in the register to set in order to discharge the cell
        // DCC8 || DCC7 || DCC6 || DCC5 || DCC4 || DCC3 || DCC2 || DCC1
        // Discharges only if it is 5mV off
        if(voltage_dif > BAL_VOTLAGE_MV) {
            switch(battery->cells[i].cell_number) {
                case '1':
                    balance_reg += DCC1;
                    break;
                case '2':
                    balance_reg += DCC2;
                    break;
                case '7':
                    balance_reg += DCC7;
                    break;
                case '8':
                    balance_reg += DCC8;
                    break;

            }
            led_comb++;
        }
    }
    if(balance_reg > 0) {
        battery->is_balancing = 1;
    } else {
        battery->is_balancing = 0;
    }

    bal_led_flash(led_comb);

    return balance_reg;
}