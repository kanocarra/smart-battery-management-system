#ifndef LAYER_h
#define LAYER_h

#include <stdio.h>
#include "neuron.h"

#define MAX_NEURONS 10
#define N_LAYERS 2

typedef struct{
    Neuron neurons[MAX_NEURONS];
    int next_index;
} Layer;

Layer init_layer(void);
void add_neuron(long double weights[MAX_INPUTS], Layer* layer, bool is_sigmoid, int n_inputs, long double bias);
long double compute_result(Layer* layers[N_LAYERS], long double inputs[MAX_INPUTS]);



#endif /* Layer_h */