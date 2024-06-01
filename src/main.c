#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include <esp_adc/adc_cali_scheme.h>
#include <esp_adc/adc_cali.h>



/* Pins definitions */
#define WAVE_A_VOLTAGE_CHANNEL      ADC1_CHANNEL_3
#define WAVE_B_VOLTAGE_CHANNEL      ADC1_CHANNEL_4
#define WAVE_C_VOLTAGE_CHANNEL      ADC1_CHANNEL_5
#define WAVE_A_CURRENT_CHANNEL      ADC1_CHANNEL_6
#define WAVE_B_CURRENT_CHANNEL      ADC1_CHANNEL_7
#define WAVE_C_CURRENT_CHANNEL      ADC1_CHANNEL_8
#define WAVE_N_CURRENT_CHANNEL      ADC1_CHANNEL_9



/* ADC definitions */
#define ADC_ATTENUATION             ADC_ATTEN_DB_12
#define ADC_WIDTH                   ADC_BITWIDTH_12
#define ADC_DEFAULT_VREF            1100



/* Project definitions */
#define CURRENT_TYPE                6           // Change this value according to the schematic "Resistor to Current" table
#define VOLTAGE_TYPE                2           // Change this value according to the schematic "Resistor to Voltage" table
#define SYSTEM_TYPE                 0           // Change this value according to the schematic "Type Of Electrical System" table
#define SENOIDE_SAMPLE_RATE         2048        // Number of samples to be taken from the sine wave SIGNAL
#define ZERO_REFERENCE_VOLTAGE      1650        // Zero reference voltage in mV



/* Global variables */
uint16_t voltage_a[SENOIDE_SAMPLE_RATE];
uint16_t voltage_b[SENOIDE_SAMPLE_RATE];
uint16_t voltage_c[SENOIDE_SAMPLE_RATE];

uint16_t current_a[SENOIDE_SAMPLE_RATE];
uint16_t current_b[SENOIDE_SAMPLE_RATE];
uint16_t current_c[SENOIDE_SAMPLE_RATE];
uint16_t current_n[SENOIDE_SAMPLE_RATE];

float VOLTAGE_CORRECTION_FACTOR = 0;
float CURRENT_CORRECTION_FACTOR = 0;



/* Function prototypes */
uint16_t get_wave_sample(uint8_t channel);



/* FreeRTOS Task prototypes */
void vTaskHello(void *pvParameters);



/* Global variables */
adc_cali_curve_fitting_config_t cali_config = {
  .unit_id = ADC_UNIT_1,
  .atten = ADC_ATTENUATION,
  .bitwidth = ADC_WIDTH,
};



void app_main(void) {
  // Initialize Serial
  uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
  };
  uart_param_config(UART_NUM_0, &uart_config);
  uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0);

  // Calibrate ADC
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTENUATION, ADC_WIDTH, ADC_DEFAULT_VREF, &cali_config);

  // Set Correction Factors
  switch (CURRENT_TYPE) {
    case 0: CURRENT_CORRECTION_FACTOR = 1000; break;
    case 1: CURRENT_CORRECTION_FACTOR = 769.23077; break;
    case 2: CURRENT_CORRECTION_FACTOR = 535.71429; break;
    case 3: CURRENT_CORRECTION_FACTOR = 300; break;
    case 4: CURRENT_CORRECTION_FACTOR = 200; break;
    case 5: CURRENT_CORRECTION_FACTOR = 125; break;
    case 6: CURRENT_CORRECTION_FACTOR = 83.33333; break;
    default: CURRENT_CORRECTION_FACTOR = 1000; break;
  }

  switch (VOLTAGE_TYPE) {
    case 0: VOLTAGE_CORRECTION_FACTOR = 483.87097; break;
    case 1: VOLTAGE_CORRECTION_FACTOR = 435.48387; break;
    case 2: VOLTAGE_CORRECTION_FACTOR = 290.32258; break;
    case 3: VOLTAGE_CORRECTION_FACTOR = 176.47059; break;
    default: VOLTAGE_CORRECTION_FACTOR = 483.87097; break;
  }

  // Create FreeRTOS Task
  xTaskCreate(vTaskHello, "Hello", 2048, NULL, 1, NULL);
}



/*
 * Task to send "Hello World" in the UART every 100 ticks
 * @param pvParameters: Task parameters
 */
void vTaskHello(void *pvParameters) {
  while (true) {
    // send the portTICK_PERIOD_MS in the uart to test the FreeRTOS configuration
    uart_write_bytes(UART_NUM_0, "Hello World\n", strlen("Hello World\n"));
    vTaskDelay(100);
  }
}



/*
 * Get a sample from the wave
 * @param channel: ADC channel to read the sample
 * @return: The sample value
 */
uint16_t get_wave_sample(uint8_t channel) {
  return esp_adc_cal_raw_to_voltage(adc1_get_raw(channel), &cali_config);
}