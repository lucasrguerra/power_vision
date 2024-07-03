#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



/* ESP32 includes */
#include <esp_timer.h>
#include <driver/adc.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>



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



/* Project definitions */
#define CURRENT_TYPE                6                               // Change this value according to the schematic "Resistor to Current" table
#define VOLTAGE_TYPE                2                               // Change this value according to the schematic "Resistor to Voltage" table
#define SYSTEM_TYPE                 0                               // Change this value according to the schematic "Type Of Electrical System" table
#define SENOIDE_SAMPLE_RATE         1920                            // Number of samples to be taken from the sine wave SIGNAL
#define CALCULATION_SAMPLE_WINDOW   (SENOIDE_SAMPLE_RATE / 4)       // Number of samples to be taken from the sine wave SIGNAL to calculate the values



/* Global variables */
uint16_t voltage_a[SENOIDE_SAMPLE_RATE];
uint16_t voltage_b[SENOIDE_SAMPLE_RATE];
uint16_t voltage_c[SENOIDE_SAMPLE_RATE];

uint16_t current_a[SENOIDE_SAMPLE_RATE];
uint16_t current_b[SENOIDE_SAMPLE_RATE];
uint16_t current_c[SENOIDE_SAMPLE_RATE];
uint16_t current_n[SENOIDE_SAMPLE_RATE];

double VOLTAGE_CORRECTION_FACTOR = 0;
double CURRENT_CORRECTION_FACTOR = 0;

static adc_cali_handle_t calibration_handle = NULL;
double REFERENCE_VOLTAGE = 0;
uint16_t read_interrupt_counter = 0;
double samples_in_acquisition[7][CALCULATION_SAMPLE_WINDOW];
double samples[7][CALCULATION_SAMPLE_WINDOW];
bool start_calculation = false;



/* Function prototypes */
double getReferenceVoltage();
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
esp_timer_create_args_t timer_args = {
  .callback = &readInterrupt,
  .arg = NULL,
  .dispatch_method = ESP_TIMER_TASK,
  .name = "readInterrupt",
};



void app_main(void) {
  // Calibrate ADC and get reference voltage
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(WAVE_A_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_B_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_C_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_A_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_B_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_C_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_N_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc_cali_create_scheme_curve_fitting(&calibration_config, &calibration_handle);
  REFERENCE_VOLTAGE = getReferenceVoltage();



  // Set Correction Factors
  switch (CURRENT_TYPE) {
    case 0: CURRENT_CORRECTION_FACTOR = 1363.64; break;
    case 1: CURRENT_CORRECTION_FACTOR = 1000; break;
    case 2: CURRENT_CORRECTION_FACTOR = 769.231; break;
    case 3: CURRENT_CORRECTION_FACTOR = 441.176; break;
    case 4: CURRENT_CORRECTION_FACTOR = 300; break;
    case 5: CURRENT_CORRECTION_FACTOR = 166.667; break;
    case 6: CURRENT_CORRECTION_FACTOR = 111.111; break;
    default: CURRENT_CORRECTION_FACTOR = 1363.64; break;
  }

  switch (VOLTAGE_TYPE) {
    case 0: VOLTAGE_CORRECTION_FACTOR = 627.907; break;
    case 1: VOLTAGE_CORRECTION_FACTOR = 574.468; break;
    case 2: VOLTAGE_CORRECTION_FACTOR = 384.615; break;
    case 3: VOLTAGE_CORRECTION_FACTOR = 232.558; break;
    default: VOLTAGE_CORRECTION_FACTOR = 627.907; break;
  }



  // Create and configure timer
  esp_timer_handle_t timer;
  esp_timer_create(&timer_args, &timer);
  esp_timer_start_periodic(timer, (uint16_t) (1000000 / SENOIDE_SAMPLE_RATE));



  printf("Reference Voltage: %f\n", REFERENCE_VOLTAGE);



  // Loop function
  while (1) {
    if (start_calculation) {
      movingAverageFilter();

      double voltage_a_rms = 0;
      double current_a_rms = 0;
      double active_power_a = 0;
      double apparent_power_a = 0;

      for (int i = 0; i < CALCULATION_SAMPLE_WINDOW; i++) {
        double sample_voltage_a = samples[0][i];
        double sample_current_a = samples[1][i];

        voltage_a_rms += sample_voltage_a * sample_voltage_a;
        current_a_rms += sample_current_a * sample_current_a;
        active_power_a += sample_voltage_a * sample_current_a;
      }

      voltage_a_rms = sqrtf(voltage_a_rms / CALCULATION_SAMPLE_WINDOW);
      current_a_rms = sqrtf(current_a_rms / CALCULATION_SAMPLE_WINDOW);
      active_power_a = active_power_a / CALCULATION_SAMPLE_WINDOW;
      apparent_power_a = voltage_a_rms * current_a_rms;
      
      printf("Voltage A RMS: %f\n", voltage_a_rms);
      printf("Current A RMS: %f\n", current_a_rms);
      printf("Apparent Power A: %f\n", apparent_power_a);
      printf("Active Power A: %f\n", active_power_a);
      printf("\n");

      start_calculation = false;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}



/*
  * Function to get the reference voltage
  * 
  * @return double: The reference voltage
*/
double getReferenceVoltage() {
  uint16_t max_voltage = 0;
  adc_cali_raw_to_voltage(calibration_handle, 4096, &max_voltage);
  uint8_t part_integer = max_voltage / 1000;
  uint16_t part_decimal = max_voltage % 1000;
  double reference_voltage = (part_integer + (part_decimal * 0.001)) / 2;
  return reference_voltage;
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
    memcpy(samples, samples_in_acquisition, sizeof(samples_in_acquisition));
  }
}



/*
  * Function to apply the moving average filter
  * 
  * @param samples: The samples to be filtered
*/
void movingAverageFilter() {
  uint8_t half_window_size = 2;

  double samples_filtered[7][CALCULATION_SAMPLE_WINDOW];
  for (uint16_t i = 0; i < CALCULATION_SAMPLE_WINDOW; i++) {
    uint16_t start_index = i - half_window_size;
    uint16_t end_index = i + half_window_size;
    start_index = start_index < 0 ? 0 : start_index;
    end_index = end_index > CALCULATION_SAMPLE_WINDOW ? CALCULATION_SAMPLE_WINDOW : end_index;
    
    double voltage_a_sum = 0;
    double current_a_sum = 0;

    for (uint16_t j = start_index; j < end_index; j++) {
      voltage_a_sum += samples[0][j];
      current_a_sum += samples[1][j];
    }
    samples_filtered[0][i] = voltage_a_sum / (end_index - start_index);
    samples_filtered[1][i] = current_a_sum / (end_index - start_index);
  }
  memcpy(samples, samples_filtered, sizeof(samples_filtered));
}