#include "neuron.h"
#include <math.h>

Neuron init_neuron(long double in_weights[MAX_INPUTS], bool in_is_sigmoid, int n_inputs, long double in_bias){
    Neuron neuron = { .is_sigmoid = in_is_sigmoid,
                      .n_inputs = n_inputs,
                      .bias = in_bias};
    for(int i = 0; i < n_inputs; i++){
        neuron.weights[i] = in_weights[i];
    }
    
    return neuron;
}

void set_weights(long double in_weights[MAX_INPUTS], Neuron* neuron){
    
    for(int i=0; i< MAX_INPUTS; i++){
       neuron->weights[i] = in_weights[i];
    }
}

long double* get_weights(Neuron* neuron) {
    return neuron->weights;
}

long double compute_output(Neuron* neuron, long double inputs[MAX_INPUTS]) {
    long double output = 0;
    
    for(int i = 0; i< neuron->n_inputs; i++ ){
        output += neuron->weights[i] * inputs[i];
    }
    
    output += neuron->bias;
    
    if(neuron->is_sigmoid) {
        output = 1.0 / (1.0 + expl(-1.0 * output));
    }
    
    return output;
}