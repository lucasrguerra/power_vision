/* Framework includes */
#include <Arduino.h>



/* Board includes */
#include <esp32-hal-adc.h>
#include <esp_adc_cal.h>
#include <driver/adc.h>



/* Pins definitions */
#define WAVE_A_VOLTAGE_CHANNEL ADC1_CHANNEL_3
#define WAVE_B_VOLTAGE_CHANNEL ADC1_CHANNEL_4
#define WAVE_C_VOLTAGE_CHANNEL ADC1_CHANNEL_5
#define WAVE_AB_VOLTAGE_CHANNEL ADC1_CHANNEL_7
#define WAVE_BC_VOLTAGE_CHANNEL ADC1_CHANNEL_8
#define WAVE_CA_VOLTAGE_CHANNEL ADC1_CHANNEL_6
#define WAVE_A_CURRENT_CHANNEL ADC2_CHANNEL_0
#define WAVE_B_CURRENT_CHANNEL ADC2_CHANNEL_1
#define WAVE_C_CURRENT_CHANNEL ADC2_CHANNEL_2
#define WAVE_N_CURRENT_CHANNEL ADC2_CHANNEL_3



/* ADC definitions */
#define ADC_ATTENUATION ADC_ATTEN_DB_11
#define ADC_WIDTH ADC_WIDTH_BIT_12
#define ADC_DEFAULT_VREF 1100



/* Project definitions */
#define SENOIDE_SAMPLE_RATE 8192
#define SAMPLE_US_INTERVAL 65
#define WINDOW_SIZE 5
#define CORRECTION_FACTOR 0.1931



/* Global variables */
esp_adc_cal_characteristics_t adc1_characteristics;
esp_adc_cal_characteristics_t adc2_characteristics;
float voltage[SENOIDE_SAMPLE_RATE];



void moving_avarage_filter(float *voltage, uint16_t window_size);
void voltage_correction(float *voltage, float correction_factor);



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
