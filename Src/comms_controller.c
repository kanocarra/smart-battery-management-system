/**
  ******************************************************************************
  * File Name          : comms_controller.c
  * Description        : This file provides code for sending communications back
                        to the host computer
  ******************************************************************************
*/

#include "comms.h"

// Sends data packet in the required format for each cell
void send_packet(Battery const* battery){
    for(int i =0; i < NUM_CELLS; i++){
        sprintf(UART_transmit_buffer, "v%c %u %u %u ", battery->cells[i].cell_number, battery->cells[i].voltage, battery->cells[i].temperature, battery->cells[i].state_of_charge);
        UART_transmit_word();
    }
    sprintf(UART_transmit_buffer, "i %u \n", battery->current);
    UART_transmit_word();   
}