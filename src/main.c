#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>



/* ESP32 includes */
#include <esp_adc/adc_cali_scheme.h>
#include <esp_adc/adc_cali.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include <driver/adc.h>
#include <esp_timer.h>
#include <nvs_flash.h>
#include <esp_wifi.h>



/* Pins definitions */
#define WAVE_A_VOLTAGE_CHANNEL      ADC1_CHANNEL_3
#define WAVE_B_VOLTAGE_CHANNEL      ADC1_CHANNEL_4
#define WAVE_C_VOLTAGE_CHANNEL      ADC1_CHANNEL_5
#define WAVE_A_CURRENT_CHANNEL      ADC1_CHANNEL_6
#define WAVE_B_CURRENT_CHANNEL      ADC1_CHANNEL_7
#define WAVE_C_CURRENT_CHANNEL      ADC1_CHANNEL_8
#define WAVE_N_CURRENT_CHANNEL      ADC1_CHANNEL_9



/* ADC definitions */
#define ADC_UNIT                    ADC_UNIT_1
#define ADC_WIDTH                   ADC_BITWIDTH_12
#define ADC_ATTENUATION             ADC_ATTEN_DB_12
#define ADC_DEFAULT_VREF            1100



/* UART definitions */
#define UART_BAUD_RATE              115200
#define UART_PORT                   UART_NUM_0
#define UART_BUFFER_SIZE            4096



/* Project definitions */
#define SYSTEM_FREQUENCY            60                                            // Change this value according to the schematic "Type Of Electrical System" table
#define CURRENT_TYPE                8                                             // Change this value according to the schematic "Resistor to Current" table
#define VOLTAGE_TYPE                2                                             // Change this value according to the schematic "Resistor to Voltage" table
#define SYSTEM_TYPE                 0                                             // Change this value according to the schematic "Type Of Electrical System" table
#define SENOIDE_SAMPLE_RATE         1920                                          // Number of samples to be taken from the sine wave SIGNAL
#define SENOIDE_SAMPLE_PERIOD       1000000 / SENOIDE_SAMPLE_RATE                 // Period of the sine wave SIGNAL
#define CALCULATION_SAMPLE_WINDOW   (SENOIDE_SAMPLE_RATE / SYSTEM_FREQUENCY)      // Number of samples to be taken from the sine wave SIGNAL to calculate the values
#define FILTER_WINDOW_SIZE          4                                             // Number of samples to be taken from the sine wave SIGNAL to calculate the values
#define FILTER_HALF_WINDOW_SIZE     FILTER_WINDOW_SIZE / 2                        // Number of samples to be taken from the sine wave SIGNAL to calculate the values
#define FREQUENCY_WINDOW            4                                             // Number of cycles to be taken from the sine wave SIGNAL to calculate the values



/* Constants */
double VOLTAGE_CORRECTION_FACTOR = 0;
double CURRENT_CORRECTION_FACTOR = 0;
double REFERENCE_VOLTAGE = 0;



/* Global variables */
double samples_in_acquisition[7][CALCULATION_SAMPLE_WINDOW];
double samples_filtered[7][CALCULATION_SAMPLE_WINDOW];
double samples[FREQUENCY_WINDOW][7][CALCULATION_SAMPLE_WINDOW];
static adc_cali_handle_t calibration_handle;
uint16_t read_interrupt_counter = 0;
char uart_buffer[UART_BUFFER_SIZE];
bool start_calculation = false;



/* Function prototypes */
void configureGPIOs();
void calibrateADC();
void setReferenceVoltage();
void setCorrectionFactors();
uint16_t getSample(adc1_channel_t channel);
double sampleToVoltage(uint16_t sample);
double sampleToCurrent(uint16_t sample);
void readInterrupt(void *arguments);
void movingAverageFilter();



/* Configs */
adc_cali_curve_fitting_config_t calibration_config = {
  .atten = ADC_ATTENUATION,
  .bitwidth = ADC_WIDTH,
  .unit_id = ADC_UNIT,
};
esp_timer_create_args_t timer_readings_args = {
  .callback = &readInterrupt,
  .arg = NULL,
  .dispatch_method = ESP_TIMER_TASK,
  .name = "readInterrupt",
};
uart_config_t uart_config = {
  .baud_rate = UART_BAUD_RATE,
  .data_bits = UART_DATA_8_BITS,
  .parity = UART_PARITY_DISABLE,
  .stop_bits = UART_STOP_BITS_1,
  .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
};



void app_main(void) {
  configureGPIOs();
  calibrateADC();
  setReferenceVoltage();
  setCorrectionFactors();

  // Create and configure timer
  esp_timer_handle_t timer_readings;
  esp_timer_create(&timer_readings_args, &timer_readings);
  esp_timer_start_periodic(timer_readings, SENOIDE_SAMPLE_PERIOD);

  // Initialize Serial
  uart_driver_install(UART_PORT, (UART_BUFFER_SIZE * 2), 0, 0, NULL, 0);
  uart_param_config(UART_PORT, &uart_config);

  sprintf(uart_buffer, "\nReference Voltage: %f\n", REFERENCE_VOLTAGE);
  uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));



  // Loop function
  while (1) {
    if (start_calculation) {
      //movingAverageFilter();

      double total_of_cycles = 0;
      double frequency = 0;
      double voltage_a_rms = 0;
      double current_a_rms = 0;
      double active_power_a = 0;

      for (uint8_t index_a = 0; index_a < FREQUENCY_WINDOW; index_a++) {
        for (int index_b = 0; index_b < CALCULATION_SAMPLE_WINDOW; index_b++) {
          double sample_voltage_a = samples[index_a][0][index_b];
          double sample_current_a = samples[index_a][1][index_b];


          if (index_a == (FREQUENCY_WINDOW - 1)) {
            voltage_a_rms += sample_voltage_a * sample_voltage_a;
            current_a_rms += sample_current_a * sample_current_a;
            active_power_a += sample_voltage_a * sample_current_a;
          }

          if (!(index_a == 0 && index_b == 0)) {
            double sample_time = SENOIDE_SAMPLE_PERIOD * ((index_a * CALCULATION_SAMPLE_WINDOW) + index_b);
          }
        }
      }
      
      //frequency = total_of_cycles != 0 ? (frequency / total_of_cycles) : 0;
      voltage_a_rms = sqrtf(voltage_a_rms / CALCULATION_SAMPLE_WINDOW);
      current_a_rms = sqrtf(current_a_rms / CALCULATION_SAMPLE_WINDOW);
      active_power_a = active_power_a / CALCULATION_SAMPLE_WINDOW;

      sprintf(uart_buffer, "\n");
      uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));
      //sprintf(uart_buffer, "%f , %f\n", voltage_a_rms, total_of_cycles);
      //uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));

      start_calculation = false;
    }
    vTaskDelay(1);
  }
}



/*
  * Function to configure the GPIOs
*/
void configureGPIOs() {
  gpio_set_direction(WAVE_A_VOLTAGE_CHANNEL, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_B_VOLTAGE_CHANNEL, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_C_VOLTAGE_CHANNEL, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_A_CURRENT_CHANNEL, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_B_CURRENT_CHANNEL, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_C_CURRENT_CHANNEL, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_N_CURRENT_CHANNEL, GPIO_MODE_INPUT);
}



/*
  * Function to calibrate the ADC
*/
void calibrateADC() {
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(WAVE_A_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_B_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_C_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_A_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_B_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_C_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_N_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc_cali_create_scheme_curve_fitting(&calibration_config, &calibration_handle);
}



/*
  * Function to set the reference voltage
*/
void setReferenceVoltage() {
  uint16_t max_voltage = 0;
  adc_cali_raw_to_voltage(calibration_handle, 4096, &max_voltage);
  uint8_t part_integer = max_voltage / 1000;
  uint16_t part_decimal = max_voltage % 1000;
  REFERENCE_VOLTAGE = (part_integer + (part_decimal * 0.001)) / 2;
}



/*
  * Function to set the correction factors
*/
void setCorrectionFactors() {
  switch (CURRENT_TYPE) {
    case 0: CURRENT_CORRECTION_FACTOR = 1363.64; break;         // 1000A 3000:1
    case 1: CURRENT_CORRECTION_FACTOR = 1000; break;            // 750A 3000:1
    case 2: CURRENT_CORRECTION_FACTOR = 769.231; break;         // 500A 3000:1
    case 3: CURRENT_CORRECTION_FACTOR = 441.176; break;         // 300A 3000:1
    case 4: CURRENT_CORRECTION_FACTOR = 300; break;             // 200A 3000:1
    case 5: CURRENT_CORRECTION_FACTOR = 166.667; break;         // 120A 3000:1
    case 6: CURRENT_CORRECTION_FACTOR = 111.111; break;         // 80A 3000:1
    case 7: CURRENT_CORRECTION_FACTOR = 133.333; break;         // 100A 2000:1
    case 8: CURRENT_CORRECTION_FACTOR = 26.667; break;          // 80A 2000:1
    default: CURRENT_CORRECTION_FACTOR = 1363.64; break;
  }

  switch (VOLTAGE_TYPE) {
    case 0: VOLTAGE_CORRECTION_FACTOR = 627.907; break;         // 440Vac
    case 1: VOLTAGE_CORRECTION_FACTOR = 574.468; break;         // 380Vac
    case 2: VOLTAGE_CORRECTION_FACTOR = 384.615; break;         // 220Vac
    case 3: VOLTAGE_CORRECTION_FACTOR = 232.558; break;         // 127Vac
    default: VOLTAGE_CORRECTION_FACTOR = 627.907; break;
  }
}



/*
  * Function to get the voltage from a specific channel
  * 
  * @param channel: The channel to be read
  * @return uint16_t: The voltage value
*/
uint16_t getSample(adc1_channel_t channel) {
  uint16_t sample = 0;
  adc_cali_raw_to_voltage(calibration_handle, adc1_get_raw(channel), &sample);
  sprintf(uart_buffer, "%i,", sample);
  uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));
  return sample;
}


/*
  * Function to get the voltage from a specific channel
  * 
  * @param channel: The channel to be read
  * @return double: The voltage value
*/
double sampleToVoltage(uint16_t sample) {
  uint8_t part_integer = sample / 1000;
  uint16_t part_decimal = sample % 1000;
  double voltage = ((part_integer + (part_decimal * 0.001)) - REFERENCE_VOLTAGE) * VOLTAGE_CORRECTION_FACTOR;
  return voltage;
}



/*
  * Function to get the current from a specific channel
  * 
  * @param channel: The channel to be read
  * @return double: The current value
*/
double sampleToCurrent(uint16_t sample) {
  uint8_t part_integer = sample / 1000;
  uint16_t part_decimal = sample % 1000;
  double current = ((part_integer + (part_decimal * 0.001)) - REFERENCE_VOLTAGE) * CURRENT_CORRECTION_FACTOR;
  return current;
}



/*
  * Function to read the interrupt
  * 
  * @param arguments: The arguments to be passed
*/
void readInterrupt(void *arguments) {
  uint16_t sample_voltage_a = getSample(WAVE_A_VOLTAGE_CHANNEL);

  uint16_t sample_current_a = getSample(WAVE_A_CURRENT_CHANNEL);

  samples_in_acquisition[0][read_interrupt_counter] = sampleToVoltage(sample_voltage_a);
  samples_in_acquisition[1][read_interrupt_counter] = sampleToCurrent(sample_current_a);

  read_interrupt_counter++;
  if (read_interrupt_counter == CALCULATION_SAMPLE_WINDOW) {
    read_interrupt_counter = 0;
    start_calculation = true;
    memcpy(samples[FREQUENCY_WINDOW - 1], samples_in_acquisition, sizeof(samples_in_acquisition));
  }
}



/*
  * Function to apply the moving average filter
  * 
  * @param samples: The samples to be filtered
*/
void movingAverageFilter() {
  double zero = 0;
  for (uint16_t i = 0; i < CALCULATION_SAMPLE_WINDOW; i++) {
    int16_t start_index = i - FILTER_HALF_WINDOW_SIZE;
    int16_t end_index = i + FILTER_HALF_WINDOW_SIZE;
    start_index = start_index < 0 ? 0 : start_index;
    end_index = end_index > CALCULATION_SAMPLE_WINDOW ? CALCULATION_SAMPLE_WINDOW : end_index;
    
    double voltage_a_sum = 0;
    double current_a_sum = 0;

    for (uint16_t j = start_index; j < end_index; j++) {
      voltage_a_sum += samples[FREQUENCY_WINDOW - 1][0][j];
      current_a_sum += samples[FREQUENCY_WINDOW - 1][1][j];
    }

    samples_filtered[0][i] = voltage_a_sum != zero ? voltage_a_sum / FILTER_WINDOW_SIZE : zero;
    samples_filtered[1][i] = current_a_sum != zero ? current_a_sum / FILTER_WINDOW_SIZE : zero;
  }
  
  for (uint8_t index = 0; index < (FREQUENCY_WINDOW - 1); index++) {
    memcpy(samples[index], samples[index + 1], sizeof(samples[index + 1]));
  }
  memcpy(samples[FREQUENCY_WINDOW - 1], samples_filtered, sizeof(samples_filtered));
}