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
#define SYSTEM_TYPE                 0                                             // Change this value according to the schematic "Type Of Electrical System" table
#define CURRENT_TYPE                8                                             // Change this value according to the schematic "Type Of Current" table
#define VOLTAGE_TYPE                2                                             // Change this value according to the schematic "Type Of Voltage" table
#define SYSTEM_FREQUENCY            60                                            // System frequency in Hz
#define SENOIDE_SAMPLE_RATE         2160                                          // Number of samples to be taken from the sine wave SIGNAL
#define SENOIDE_SAMPLE_PERIOD       (1000000 / SENOIDE_SAMPLE_RATE) + 1           // Period of the sine wave SIGNAL
#define SAMPLES_PER_CYCLE           (SENOIDE_SAMPLE_RATE / SYSTEM_FREQUENCY)      // Number of samples per cycle
#define REFERENCE_VOLTAGE			1650                                          // Reference voltage in milivolts



/* Global variables */
uint16_t samples_in_acquisition[7][SAMPLES_PER_CYCLE];
uint16_t samples[7][SAMPLES_PER_CYCLE];
uint16_t read_interrupt_counter = 0;

uint16_t number_of_calculated_cycles = 0;
bool make_calcs = false;



/* Function prototypes */
void readInterrupt(void *arguments);



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



	// Configure and start WiFi
	strcpy(WIFI_SSID, readStringFromNVS(WIFI_SSID_KEY));
	strcpy(WIFI_PASSWORD, readStringFromNVS(WIFI_PASSWORD_KEY));
	if (strlen(WIFI_SSID) == 0 || strlen(WIFI_PASSWORD) == 0) {
		initializeWiFiAP();
		wifi_ap_started = true;
	} else {
		initializeWiFi();
	}



	// JSON data
	cJSON *data_json = cJSON_CreateObject();
	cJSON *voltage_a_rms_json = cJSON_CreateArray();
	cJSON *voltage_b_rms_json = cJSON_CreateArray();
	cJSON *voltage_c_rms_json = cJSON_CreateArray();
	cJSON *current_a_rms_json = cJSON_CreateArray();
	cJSON *current_b_rms_json = cJSON_CreateArray();
	cJSON *current_c_rms_json = cJSON_CreateArray();
	cJSON *current_n_rms_json = cJSON_CreateArray();

	// Loop function
	while (1) {
		if (!wifi_connected && !wifi_ap_started && wifi_connect_error) {
			initializeWiFiAP();
			wifi_ap_started = true;
		}

		if (make_calcs && wifi_connected) {
			float voltage_a_rms = 0;
			float voltage_b_rms = 0;
			float voltage_c_rms = 0;
			float current_a_rms = 0;
			float current_b_rms = 0;
			float current_c_rms = 0;
			float current_n_rms = 0;

			for (int index = 0; index < SAMPLES_PER_CYCLE; index++) {
				float voltage_a = getCorrectedVoltage(samples[0][index]);
				float voltage_b = getCorrectedVoltage(samples[1][index]);
				float voltage_c = getCorrectedVoltage(samples[2][index]);
				float current_a = getCorrectedCurrent(samples[3][index]);
				float current_b = getCorrectedCurrent(samples[4][index]);
				float current_c = getCorrectedCurrent(samples[5][index]);
				float current_n = getCorrectedCurrent(samples[6][index]);

				voltage_a_rms += voltage_a * voltage_a;
				voltage_b_rms += voltage_b * voltage_b;
				voltage_c_rms += voltage_c * voltage_c;
				current_a_rms += current_a * current_a;
				current_b_rms += current_b * current_b;
				current_c_rms += current_c * current_c;
				current_n_rms += current_n * current_n;
			}

			voltage_a_rms = sqrt(voltage_a_rms / SAMPLES_PER_CYCLE);
			voltage_b_rms = sqrt(voltage_b_rms / SAMPLES_PER_CYCLE);
			voltage_c_rms = sqrt(voltage_c_rms / SAMPLES_PER_CYCLE);
			current_a_rms = sqrt(current_a_rms / SAMPLES_PER_CYCLE);
			current_b_rms = sqrt(current_b_rms / SAMPLES_PER_CYCLE);
			current_c_rms = sqrt(current_c_rms / SAMPLES_PER_CYCLE);
			current_n_rms = sqrt(current_n_rms / SAMPLES_PER_CYCLE);
			
			cJSON_AddItemToArray(voltage_a_rms_json, cJSON_CreateNumber(voltage_a_rms));
			cJSON_AddItemToArray(voltage_b_rms_json, cJSON_CreateNumber(voltage_b_rms));
			cJSON_AddItemToArray(voltage_c_rms_json, cJSON_CreateNumber(voltage_c_rms));
			cJSON_AddItemToArray(current_a_rms_json, cJSON_CreateNumber(current_a_rms));
			cJSON_AddItemToArray(current_b_rms_json, cJSON_CreateNumber(current_b_rms));
			cJSON_AddItemToArray(current_c_rms_json, cJSON_CreateNumber(current_c_rms));
			cJSON_AddItemToArray(current_n_rms_json, cJSON_CreateNumber(current_n_rms));

			number_of_calculated_cycles++;
			make_calcs = false;
		}

		if (number_of_calculated_cycles == SYSTEM_FREQUENCY && wifi_connected) {
			number_of_calculated_cycles = 0;
			
			cJSON_AddItemToObject(data_json, "voltage_a_rms", voltage_a_rms_json);
			cJSON_AddItemToObject(data_json, "voltage_b_rms", voltage_b_rms_json);
			cJSON_AddItemToObject(data_json, "voltage_c_rms", voltage_c_rms_json);
			cJSON_AddItemToObject(data_json, "current_a_rms", current_a_rms_json);
			cJSON_AddItemToObject(data_json, "current_b_rms", current_b_rms_json);
			cJSON_AddItemToObject(data_json, "current_c_rms", current_c_rms_json);
			cJSON_AddItemToObject(data_json, "current_n_rms", current_n_rms_json);

			printf("Sending data\n");

			data_json = cJSON_CreateObject();
			voltage_a_rms_json = cJSON_CreateArray();
			voltage_b_rms_json = cJSON_CreateArray();
			voltage_c_rms_json = cJSON_CreateArray();
			current_a_rms_json = cJSON_CreateArray();
			current_b_rms_json = cJSON_CreateArray();
			current_c_rms_json = cJSON_CreateArray();
			current_n_rms_json = cJSON_CreateArray();
		}

		vTaskDelay(1);
	}
}



/*
  * Function to read the interrupt
  * 
  * @param arguments: The arguments to be passed
*/
void readInterrupt(void *arguments) {
	uint16_t actual_voltage_sample = getSample(WAVE_A_VOLTAGE_CHANNEL);

	samples_in_acquisition[0][read_interrupt_counter] = actual_voltage_sample;
	samples_in_acquisition[1][read_interrupt_counter] = getSample(WAVE_B_VOLTAGE_CHANNEL);
	samples_in_acquisition[2][read_interrupt_counter] = getSample(WAVE_C_VOLTAGE_CHANNEL);
	samples_in_acquisition[3][read_interrupt_counter] = getSample(WAVE_A_CURRENT_CHANNEL);
	samples_in_acquisition[4][read_interrupt_counter] = getSample(WAVE_B_CURRENT_CHANNEL);
	samples_in_acquisition[5][read_interrupt_counter] = getSample(WAVE_C_CURRENT_CHANNEL);
	samples_in_acquisition[6][read_interrupt_counter] = getSample(WAVE_N_CURRENT_CHANNEL);

	read_interrupt_counter++;
	if (read_interrupt_counter == SAMPLES_PER_CYCLE) {
		read_interrupt_counter = 0;
		make_calcs = true;
		memcpy(samples, samples_in_acquisition, sizeof(samples_in_acquisition));
	}
}