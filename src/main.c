/* C includes */
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



/* ESP32 includes */
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <esp_timer.h>



/* Project includes */
#include "wifi/main.h"
#include "uart/main.h"
#include "adc/main.h"
#include "nvs/main.h"



/* Measurement definitions */
#define SENOIDE_SAMPLE_RATE         2160											// Number of samples to be taken from the sine wave SIGNAL
#define SENOIDE_SAMPLE_PERIOD       (1000000 / SENOIDE_SAMPLE_RATE)					// Period of the sine wave SIGNAL	
#define PULSE_PIN					GPIO_NUM_9										// Pin to be used as pulse pin



/* Global variables */
uint64_t last_time = 0;

static TaskHandle_t calculateData_handle = NULL;
static QueueHandle_t pulse_queue = NULL;



/* Function prototypes */
static void IRAM_ATTR pulseInterrupt(void *parameters);
static void IRAM_ATTR readInterrupt(void *parameters);
static void calculateData(void *parameters);



/* Configurations */
gpio_config_t pulse_pin_config = {
	.mode = GPIO_MODE_INPUT,
	.intr_type = GPIO_INTR_POSEDGE,
	.pull_up_en = 0,
	.pull_down_en = 1,
	.pin_bit_mask = (1ULL << PULSE_PIN),
};
esp_timer_create_args_t timer_readings_args = {
	.callback = &readInterrupt,
	.arg = NULL,
	.dispatch_method = ESP_TIMER_TASK,
	.name = "readInterrupt",
};




void app_main(void)
{
    // Configure board
	initializeUART();
	initializeNVS();
	calibrateADC();

	// Configure pulse interrupt
	gpio_config(&pulse_pin_config);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(PULSE_PIN, pulseInterrupt, NULL);
    pulse_queue = xQueueCreate(10, sizeof(uint32_t));

	// Create and configure timer
	esp_timer_handle_t timer_readings;
	esp_timer_create(&timer_readings_args, &timer_readings);
	esp_timer_start_periodic(timer_readings, SENOIDE_SAMPLE_PERIOD);

	// Create tasks
    xTaskCreate(calculateData, "calculateData", 4096, NULL, 1, &calculateData_handle);
}



/*
  * Function to handle the pulse interrupt
  * 
  * @param arguments: The arguments to be passed
*/
void pulseInterrupt(void *arguments) {
	uint64_t current_time = esp_timer_get_time();
	uint64_t cycle_duration = current_time - last_time;
	last_time = current_time;
    xQueueSendFromISR(pulse_queue, &cycle_duration, NULL);
}



/*
  * Function to read the interrupt
  * 
  * @param arguments: The arguments to be passed
*/
void readInterrupt(void *arguments) {
	uint16_t sample_voltage_a = getSample(WAVE_A_VOLTAGE_CHANNEL);
	uint16_t sample_current_a = getSample(WAVE_A_CURRENT_CHANNEL);

	char samples_string[24];
	sprintf(samples_string, "samples: %d %d\n", sample_voltage_a, sample_current_a);
	uart_write_bytes(UART_PORT, samples_string, strlen(samples_string));
}



/*
  * Function to calculate the data
  * 
  * @param arguments: The arguments to be passed
*/
void calculateData(void *arguments) {
	uint64_t cycle_duration;
    while (1) {
        if (xQueueReceive(pulse_queue, &cycle_duration, portMAX_DELAY)) {
			float frequency = 1000000.0 / cycle_duration;

			char frequency_string[20];
			sprintf(frequency_string, "frequency: %.5f\n", frequency);
			uart_write_bytes(UART_PORT, frequency_string, strlen(frequency_string));
        }
		vTaskDelay(1);
    }
}