/* UART Main Header */
#pragma once



/* includes */
#include <driver/uart.h>
#include <string.h>



/* Definitions */
#define UART_PORT                   UART_NUM_0
#define UART_BAUD_RATE              115200
#define UART_BUFFER_SIZE            1024



/* Global Variables */
extern uart_config_t uart_config;



/* Function Prototypes */
void initializeUART();
void sendUART(const char* data);