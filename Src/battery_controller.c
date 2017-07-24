#include "battery.h"

Layer* layers[N_LAYERS];
enum input{TIME_ELAPSED, VOLTAGE, CURRENT, CAPACITY, INTERNAL_RESISTANCE};
enum nodes{SM1, SM2, SM3};


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

int get_soc(Battery *const battery) {
    Layer input_layer = init_layer();
    
    long double sigmoid_node1_weights[MAX_INPUTS];
    long double sigmoid_node2_weights[MAX_INPUTS];
    long double sigmoid_node3_weights[MAX_INPUTS];
    long double sigmoid_node1_bias;
    long double sigmoid_node2_bias;
    long double sigmoid_node3_bias;
    
    sigmoid_node1_weights[TIME_ELAPSED] = 4.878289914735682;
    sigmoid_node1_weights[VOLTAGE] = 6.353756026857561;
    sigmoid_node1_weights[CURRENT] = 3.407684855151011;
    sigmoid_node1_weights[CAPACITY] = -0.06926945731506193;
    sigmoid_node1_weights[INTERNAL_RESISTANCE] =  -0.010043877987725111;
    sigmoid_node1_bias =  4.742042871760883;
    add_neuron(sigmoid_node1_weights, &input_layer, 1, 5, sigmoid_node1_bias);
    
    sigmoid_node2_weights[TIME_ELAPSED] = 1.176881690677296;
    sigmoid_node2_weights[VOLTAGE] =  0.025788514341405753;
    sigmoid_node2_weights[CURRENT] = -0.9214576810041776;
    sigmoid_node2_weights[CAPACITY] =  0.5822346570940141;
    sigmoid_node2_weights[INTERNAL_RESISTANCE] = 0.02865397891804468;
    sigmoid_node2_bias =   -2.9953645011246555;
    add_neuron(sigmoid_node2_weights, &input_layer, 1, 5, sigmoid_node2_bias);
    
    sigmoid_node3_weights[TIME_ELAPSED] = 0.2994308683068657;
    sigmoid_node3_weights[VOLTAGE] = 14.855021773095666;
    sigmoid_node3_weights[CURRENT] =  0.8361545963317351;
    sigmoid_node3_weights[CAPACITY] = 0.023966700635398273;
    sigmoid_node3_weights[INTERNAL_RESISTANCE] = 0.001179240596627643;
    sigmoid_node3_bias =  -1.2413421333276302;
    add_neuron(sigmoid_node3_weights, &input_layer, 1, 5, sigmoid_node3_bias);

    layers[0] = &input_layer;
    
    Layer output_layer = init_layer();
    
    long double linear_node0_weights[3];
    long double linear_node0_bias;
    
    linear_node0_weights[SM1] =  1.017107093283043;
    linear_node0_weights[SM2] =   0.7075539003596144;
    linear_node0_weights[SM3] = 1.0182853576502393;
    linear_node0_bias =   -1.0325764534548219;
    add_neuron(linear_node0_weights, &output_layer, 0, 5, linear_node0_bias);

    layers[1] = &output_layer;

    long double inputs[5] = {-0.871483, -0.009009, -0.98628, 0.903284, -0.922976};
    long double normalised_soc = compute_result(layers, inputs);
    long double soc = (((normalised_soc- -1.0) * 100.0) / 2.0);
    battery->cells[0].state_of_charge = soc;
    battery->cells[1].state_of_charge = soc;
    battery->cells[2].state_of_charge = soc;
    battery->cells[3].state_of_charge = soc;
    return (int)(soc*1000.0);
}

void init_model(void) {

   
}
