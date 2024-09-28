/* C includes */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cJSON.h>



/* ESP32 includes */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>



/* Project includes */
#include "corrections/main.h"
#include "wifi/main.h"
#include "uart/main.h"
#include "adc/main.h"
#include "nvs/main.h"



/* Measurement definitions */
#define SYSTEM_TYPE                 0 												// Change this value according to the schematic "Type Of Electrical System" table
#define CURRENT_TYPE                8 												// Change this value according to the schematic "Type Of Current" table
#define VOLTAGE_TYPE                2 												// Change this value according to the schematic "Type Of Voltage" table
#define SYSTEM_FREQUENCY            60												// System frequency in Hz
#define SENOIDE_SAMPLE_RATE         2160											// Number of samples to be taken from the sine wave SIGNAL
#define SENOIDE_SAMPLE_PERIOD       (1000000 / SENOIDE_SAMPLE_RATE) + 1				// Period of the sine wave SIGNAL
#define SAMPLES_PER_CYCLE           (SENOIDE_SAMPLE_RATE / SYSTEM_FREQUENCY)		// Number of samples per cycle
#define REFERENCE_VOLTAGE			1650											// Reference voltage in milivolts
#define CYCLES_TO_AVERAGE			20												// Number of cycles to average the RMS voltage



/* Global variables */
uint16_t samples_in_acquisition[7][SAMPLES_PER_CYCLE];
uint16_t samples[7][SAMPLES_PER_CYCLE];

uint16_t last_voltage_sample = 0;
uint64_t cycle_duration = 0;
uint64_t last_time = 0;

uint8_t read_interrupt_counter = 0;
uint8_t cycles_counter = 0;

float frequency = 0;
float voltage_a_rms = 0;
float data_to_average[2][CYCLES_TO_AVERAGE];

TaskHandle_t calculate_data_task;
bool make_calcs = false;
bool send_data = false;



/* Function prototypes */
void readInterrupt(void *parameters);
void calculateData(void *parameters);
void moveDataToAverage();
void calculateAverage();



/* Configurations */
esp_timer_create_args_t timer_readings_args = {
	.callback = &readInterrupt,
	.arg = NULL,
	.dispatch_method = ESP_TIMER_TASK,
	.name = "readInterrupt",
};



void app_main(void) {
	// Configure board
	initializeUART();
	initializeNVS();
	calibrateADC();
	setVoltageType(VOLTAGE_TYPE);
	setCurrentType(CURRENT_TYPE);

	// Create and configure timer
	esp_timer_handle_t timer_readings;
	esp_timer_create(&timer_readings_args, &timer_readings);
	esp_timer_start_periodic(timer_readings, SENOIDE_SAMPLE_PERIOD);

	// Create tasks
	xTaskCreatePinnedToCore(&calculateData, "calculateData", 4096, NULL, 1, &calculate_data_task, 1);
}



/*
  * Function to read the interrupt
  * 
  * @param arguments: The arguments to be passed
*/
void readInterrupt(void *arguments) {
	uint16_t actual_voltage_sample = getSample(WAVE_A_VOLTAGE_CHANNEL);
	uint64_t actual_time = esp_timer_get_time();

	samples_in_acquisition[0][read_interrupt_counter] = actual_voltage_sample;
	samples_in_acquisition[1][read_interrupt_counter] = getSample(WAVE_B_VOLTAGE_CHANNEL);
	samples_in_acquisition[2][read_interrupt_counter] = getSample(WAVE_C_VOLTAGE_CHANNEL);
	samples_in_acquisition[3][read_interrupt_counter] = getSample(WAVE_A_CURRENT_CHANNEL);
	samples_in_acquisition[4][read_interrupt_counter] = getSample(WAVE_B_CURRENT_CHANNEL);
	samples_in_acquisition[5][read_interrupt_counter] = getSample(WAVE_C_CURRENT_CHANNEL);
	samples_in_acquisition[6][read_interrupt_counter] = getSample(WAVE_N_CURRENT_CHANNEL);

	if (actual_voltage_sample >= REFERENCE_VOLTAGE && last_voltage_sample < REFERENCE_VOLTAGE) {
		cycle_duration = actual_time - last_time;
		last_time = actual_time;
		memcpy(samples, samples_in_acquisition, sizeof(samples_in_acquisition));
		make_calcs = true;
	}
	last_voltage_sample = actual_voltage_sample;

	read_interrupt_counter++;
	if (read_interrupt_counter == SAMPLES_PER_CYCLE) {
		read_interrupt_counter = 0;
	}
}



/*
  * Function to calculate the data
  * 
  * @param arguments: The arguments to be passed
*/
void calculateData(void *arguments) {
	while (1) {
		if (make_calcs) {
			frequency = 1000000.0 / cycle_duration;

			for (uint8_t index = 0; index < SAMPLES_PER_CYCLE; index++) {
				float voltage_a = getCorrectedVoltage(samples[0][index]);
				voltage_a_rms += voltage_a * voltage_a;
			}

			voltage_a_rms = sqrt(voltage_a_rms / SAMPLES_PER_CYCLE);

			make_calcs = false;

			moveDataToAverage();
			calculateAverage();
		}

		vTaskDelay(1);
	}
}



/*
  * Function to move the data to average
*/
void moveDataToAverage() {
	data_to_average[0][cycles_counter] = frequency;
	data_to_average[1][cycles_counter] = voltage_a_rms;
	cycles_counter++;

	if (cycles_counter == CYCLES_TO_AVERAGE) {
		cycles_counter = 0;
		send_data = true;
	}
}



/*
  * Function to calculate the average
*/
void calculateAverage() {
	float frequency_average = 0;
	float voltage_a_rms_average = 0;

	for (uint8_t index = 0; index < CYCLES_TO_AVERAGE; index++) {
		frequency_average += data_to_average[0][index];
		voltage_a_rms_average += data_to_average[1][index];
	}

	frequency_average /= CYCLES_TO_AVERAGE;
	voltage_a_rms_average /= CYCLES_TO_AVERAGE;

	if (send_data) {
		printf("%.4f,", frequency_average);
		printf("%.2f\n", voltage_a_rms_average);
	}
}