#include "layer.h"
#include "usart.h"

Layer init_layer(void) {
    Layer layer = {.next_index = 0};
    return layer;
    
    
}

void add_neuron(long double weights[MAX_INPUTS], Layer* layer, bool is_sigmoid, int n_inputs, long double bias) {
    Neuron next_neuron = init_neuron(weights, is_sigmoid, n_inputs, bias);
    layer->neurons[layer->next_index] = next_neuron;
    layer->next_index++;
}

long double compute_result(Layer* layers[N_LAYERS], long double inputs[MAX_INPUTS]){
    long double result = 0;
    long double next_layer_inputs[N_LAYERS][MAX_INPUTS] = {
                                                    {0,0,0,0,0},
                                                    {0,0,0,0,0}
                                                    };
    for(int i= 0; i < N_LAYERS; i++) {
        Layer* tmp = layers[i];
        int n_neurons = tmp->next_index;
        long double layer_outputs[n_neurons];
        for(int j =0; j < n_neurons; j++){
            if (i == 0){
                layer_outputs[j] = compute_output(&(tmp->neurons[j]), inputs);
                next_layer_inputs[i][j] = layer_outputs[j];
            } else {
                layer_outputs[j] = compute_output(&(tmp->neurons[j]), next_layer_inputs[i-1]);
                next_layer_inputs[i][j] = layer_outputs[j];
                result = layer_outputs[j];
            }
        }
    }
    
    return result;
    
}
