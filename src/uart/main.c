#include "main.h"



/*
    * UART Configuration
*/
uart_config_t uart_config = {
    .baud_rate = UART_BAUD_RATE,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
};



/*
    * Function to initialize the UART
*/
void initializeUART() {
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT, UART_BUFFER_SIZE * 2, 0, 0, NULL, 0);
}



/*
    * Function to send data through the UART
    * 
    * @param data: The data to be sent
*/
void sendUART(const char* data) {
    uart_write_bytes(UART_PORT, data, strlen(data));
}