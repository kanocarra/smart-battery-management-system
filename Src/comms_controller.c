/**
  ******************************************************************************
  * File Name          : comms_controller.c
  * Description        : This file provides code for sending communications back
                        to the host computer
  ******************************************************************************
*/

#include "comms.h"
#include "usart.h"

// Sends data packet in the required format for each cell
void send_packet(Battery const* battery){
    for(int i =0; i < NUM_CELLS; i++){
        sprintf(UART_transmit_buffer, "v%c %u %u %u ", battery->cells[i].cell_number, battery->cells[i].voltage, battery->cells[i].temperature, battery->cells[i].state_of_charge);
        UART_transmit_word();
    }
    sprintf(UART_transmit_buffer, "i %u \n", battery->current);
    UART_transmit_word();   
    sprintf(UART_transmit_buffer, "t %u \n", battery->time_elapsed);
    UART_transmit_word();   
}

void send_status(StatusA* status_regA, StatusB* status_regB) {
    uint16_t analog_v =  ((uint16_t)status_regA->VAUB << 8) | status_regA->VALB;
    uint16_t int_temp =  ((uint16_t)status_regA->ITMPUB << 8) | status_regA->ITMPLB;
    uint16_t digital_v =  ((uint16_t)status_regB->VDUB << 8) | status_regB->VDLB;
    uint16_t sum_cells = ((uint16_t)status_regA->SOCUB << 8) | status_regA->SOCLB;
    // sprintf(UART_transmit_buffer, "Sum of Cells: %u \n", sum_cells);
    // UART_transmit_word(); 
    // sprintf(UART_transmit_buffer, "Analog Voltage: %u \n", analog_v);
    // UART_transmit_word(); 
    // sprintf(UART_transmit_buffer, "Digital Voltage: %u \n", digital_v);
    // UART_transmit_word();   

}