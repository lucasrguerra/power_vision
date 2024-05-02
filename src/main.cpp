/* Framework includes */
#include <Arduino.h>



/* Board includes */
#include <esp32-hal-adc.h>
#include <esp_adc_cal.h>
#include <driver/adc.h>



/* Pins definitions */
#define SYSTEM_TYPE_PIN             GPIO_NUM_1
#define WAVE_A_VOLTAGE_CHANNEL      ADC1_CHANNEL_3
#define WAVE_B_VOLTAGE_CHANNEL      ADC1_CHANNEL_4
#define WAVE_C_VOLTAGE_CHANNEL      ADC1_CHANNEL_5
#define WAVE_AB_VOLTAGE_CHANNEL     ADC1_CHANNEL_7
#define WAVE_BC_VOLTAGE_CHANNEL     ADC1_CHANNEL_8
#define WAVE_CA_VOLTAGE_CHANNEL     ADC1_CHANNEL_6
#define WAVE_A_CURRENT_CHANNEL      ADC2_CHANNEL_0
#define WAVE_C_CURRENT_CHANNEL      ADC2_CHANNEL_2
#define WAVE_B_CURRENT_CHANNEL      ADC2_CHANNEL_1
#define WAVE_N_CURRENT_CHANNEL      ADC2_CHANNEL_3



/* ADC definitions */
#define ADC_ATTENUATION             ADC_ATTEN_DB_11
#define ADC_WIDTH                   ADC_WIDTH_BIT_12
#define ADC_DEFAULT_VREF            1100



/* Project definitions */
#define SENOIDE_SAMPLE_RATE         2048
#define ZERO_REFERENCE_VOLTAGE      1650



/* Global variables */
esp_adc_cal_characteristics_t adc1_characteristics;
esp_adc_cal_characteristics_t adc2_characteristics;

uint16_t voltage_a[SENOIDE_SAMPLE_RATE];
uint16_t voltage_b[SENOIDE_SAMPLE_RATE];
uint16_t voltage_c[SENOIDE_SAMPLE_RATE];
uint16_t voltage_ab[SENOIDE_SAMPLE_RATE];
uint16_t voltage_bc[SENOIDE_SAMPLE_RATE];
uint16_t voltage_ca[SENOIDE_SAMPLE_RATE];

uint16_t current_a[SENOIDE_SAMPLE_RATE];
uint16_t current_b[SENOIDE_SAMPLE_RATE];
uint16_t current_c[SENOIDE_SAMPLE_RATE];
uint16_t current_n[SENOIDE_SAMPLE_RATE];



/* Function prototypes */
uint16_t read_voltage(adc1_channel_t channel);
uint16_t read_current(adc2_channel_t channel);



/* Setup function */
void setup() {
  Serial.begin(115200);


  /* ADC configuration */
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(WAVE_A_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_B_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_C_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_AB_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_BC_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc1_config_channel_atten(WAVE_CA_VOLTAGE_CHANNEL, ADC_ATTENUATION);
  adc2_config_channel_atten(WAVE_A_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc2_config_channel_atten(WAVE_B_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc2_config_channel_atten(WAVE_C_CURRENT_CHANNEL, ADC_ATTENUATION);
  adc2_config_channel_atten(WAVE_N_CURRENT_CHANNEL, ADC_ATTENUATION);


  /* Calibrate ADC */
  esp_adc_cal_value_t adc1_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTENUATION, ADC_WIDTH, ADC_DEFAULT_VREF, &adc1_characteristics);
  esp_adc_cal_value_t adc2_type = esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTENUATION, ADC_WIDTH, ADC_DEFAULT_VREF, &adc2_characteristics);
}



/* Loop function */
void loop() {

}



/*
  * Read voltage function
  * @param channel: ADC channel to read
  * @return: mV value
*/
uint16_t read_voltage(adc1_channel_t channel) {
  int channel_value;
  channel_value = adc1_get_raw(channel);
  return esp_adc_cal_raw_to_voltage(channel_value, &adc1_characteristics);
}


/*
  * Read current function
  * @param channel: ADC channel to read
  * @return: mV value
*/
uint16_t read_current(adc2_channel_t channel) {
  int * channel_value;
  adc2_get_raw(channel, ADC_WIDTH, channel_value);
  return esp_adc_cal_raw_to_voltage((uint16_t)channel_value, &adc2_characteristics);
}
