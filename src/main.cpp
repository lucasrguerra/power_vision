/* Framework includes */
#include <Arduino.h>



/* Board includes */
#include <esp32-hal-adc.h>
#include <esp_adc_cal.h>
#include <driver/adc.h>



/* Pins definitions */
#define SYSTEM_TYPE_PIN             GPIO_NUM_1
#define WAVE_A_CURRENT_CHANNEL      ADC2_CHANNEL_3
#define WAVE_C_CURRENT_CHANNEL      ADC2_CHANNEL_4
#define WAVE_B_CURRENT_CHANNEL      ADC2_CHANNEL_5
#define WAVE_N_CURRENT_CHANNEL      ADC2_CHANNEL_6
#define WAVE_A_VOLTAGE_CHANNEL      ADC1_CHANNEL_7
#define WAVE_B_VOLTAGE_CHANNEL      ADC1_CHANNEL_8
#define WAVE_C_VOLTAGE_CHANNEL      ADC1_CHANNEL_9



/* ADC definitions */
#define ADC_ATTENUATION             ADC_ATTEN_DB_11
#define ADC_WIDTH                   ADC_WIDTH_BIT_12
#define ADC_DEFAULT_VREF            1100



/* Project definitions */
#define CURRENT_TYPE                6           // Change this value according to the schematic "Resistor to Current" table
#define VOLTAGE_TYPE                2           // Change this value according to the schematic "Resistor to Voltage" table
#define SENOIDE_SAMPLE_RATE         2048        // Number of samples to be taken from the sine wave
#define ZERO_REFERENCE_VOLTAGE      1650        // Zero reference voltage in mV



/* Global variables */
esp_adc_cal_characteristics_t adc_characteristics;

uint16_t voltage_a[SENOIDE_SAMPLE_RATE];
uint16_t voltage_b[SENOIDE_SAMPLE_RATE];
uint16_t voltage_c[SENOIDE_SAMPLE_RATE];

uint16_t current_a[SENOIDE_SAMPLE_RATE];
uint16_t current_b[SENOIDE_SAMPLE_RATE];
uint16_t current_c[SENOIDE_SAMPLE_RATE];
uint16_t current_n[SENOIDE_SAMPLE_RATE];

uint16_t VOLTAGE_CORRECTION_FACTOR = 0;
uint16_t CURRENT_CORRECTION_FACTOR = 0;



/* Function prototypes */
uint16_t read_channel(adc1_channel_t channel);



/* Setup function */
void setup() {
  Serial.begin(115200);


  /* ADC configuration */
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(WAVE_A_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_B_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_C_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc2_config_channel_atten(WAVE_A_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc2_config_channel_atten(WAVE_B_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc2_config_channel_atten(WAVE_C_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc2_config_channel_atten(WAVE_N_CURRENT_CHANNEL, ADC_ATTENUATION);


  /* Calibrate ADC */
  esp_adc_cal_value_t adc_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTENUATION, ADC_WIDTH, ADC_DEFAULT_VREF, &adc_characteristics);


  /* Seting the correction factors */
  switch (CURRENT_TYPE) {
    case 0: CURRENT_CORRECTION_FACTOR = 3111; break;
    case 1: CURRENT_CORRECTION_FACTOR = 3040; break;
    case 2: CURRENT_CORRECTION_FACTOR = 3205; break;
    case 3: CURRENT_CORRECTION_FACTOR = 2828; break;
    case 4: CURRENT_CORRECTION_FACTOR = 2828; break;
    case 5: CURRENT_CORRECTION_FACTOR = 3054; break;
    case 6: CURRENT_CORRECTION_FACTOR = 3243; break;
    default: CURRENT_CORRECTION_FACTOR = 3111; break;
  }

  switch (VOLTAGE_TYPE) {
    case 0: VOLTAGE_CORRECTION_FACTOR = 3108; break;
    case 1: VOLTAGE_CORRECTION_FACTOR = 3161; break;
    case 2: VOLTAGE_CORRECTION_FACTOR = 2963; break;
    case 3: VOLTAGE_CORRECTION_FACTOR = 3091; break;
    default: VOLTAGE_CORRECTION_FACTOR = 3108; break;
  }
}



/* Loop function */
void loop() {

}



/*
  * Read a channel and convert the value to mV
  * @param channel: ADC channel to be read
  * @return: Value in mV
*/
uint16_t read_channel(adc1_channel_t channel) {
  int channel_value;
  channel_value = adc1_get_raw(channel);
  return esp_adc_cal_raw_to_voltage(channel_value, &adc_characteristics);
}