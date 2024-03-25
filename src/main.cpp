/* Framework includes */
#include <Arduino.h>



/* Board includes */
#include <esp32-hal-adc.h>
#include <esp_adc_cal.h>
#include <driver/adc.h>



/* Project definitions */
#define SENOIDE_SAMPLE_RATE 8192
#define ADC_ATTENUATION ADC_ATTEN_DB_11
#define ADC_WIDTH ADC_WIDTH_BIT_12
#define DEFAULT_VREF 1100
#define SAMPLE_US_INTERVAL 65
#define WINDOW_SIZE 5
#define CORRECTION_FACTOR 0.1931



/* Global variables */
esp_adc_cal_characteristics_t adc_characteristics;
float voltage[SENOIDE_SAMPLE_RATE];



void moving_avarage_filter(float *voltage, uint16_t window_size);
void voltage_correction(float *voltage, float correction_factor);



/* Setup function */
void setup() {
  /* Serial port configuration */
  Serial.begin(115200);


  /* ADC configuration */
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTENUATION);


  /* Calibrate ADC */
  esp_adc_cal_value_t adc_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTENUATION, ADC_WIDTH, DEFAULT_VREF, &adc_characteristics);
  if (adc_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.println("eFuse Vref");
  } else if (adc_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    Serial.println("Two Point");
  } else {
    Serial.println("Default");
  }
}



/* Loop function */
void loop() {
  /* Read ADC */
  for (int i = 0; i < SENOIDE_SAMPLE_RATE; i++) {
    uint16_t adc_reading = adc1_get_raw(ADC1_CHANNEL_3);
    voltage[i] = esp_adc_cal_raw_to_voltage(adc_reading, &adc_characteristics);
    delayMicroseconds(SAMPLE_US_INTERVAL);
  }


  /* Moving avarage filter */
  moving_avarage_filter(voltage, WINDOW_SIZE);
  voltage_correction(voltage, CORRECTION_FACTOR);


  float voltage_rms_integral = 0;
  for (int i = 0; i < SENOIDE_SAMPLE_RATE; i++) {
    float voltage_data = voltage[i];
    voltage_rms_integral += voltage_data * voltage_data;
  }
  float voltage_rms = sqrt(voltage_rms_integral / SENOIDE_SAMPLE_RATE);


  Serial.print("Voltage RMS: " + String(voltage_rms) + " Vac\n");


  delay(1000);
}



/* Moving avarage filter */
void moving_avarage_filter(float *voltage, uint16_t window_size) {
  float normalization_factor = 1.0 / window_size;

  for (int i = 0; i < SENOIDE_SAMPLE_RATE; i++) {
    float *voltage_data = &voltage[i];
    uint16_t start_value = max(0, i - int(window_size / 2)); 
    uint16_t end_value = min(SENOIDE_SAMPLE_RATE, i + int(window_size / 2));
    float sum = 0;
    for (int j = start_value; j < end_value; j++) {
      sum += voltage[j];
    }
    *voltage_data = sum * normalization_factor;
  }
}



/* Voltage correction */
void voltage_correction(float *voltage, float correction_factor) {
  for (int i = 0; i < SENOIDE_SAMPLE_RATE; i++) {
    float *voltage_data = &voltage[i];
    *voltage_data *= correction_factor;
  }
}
