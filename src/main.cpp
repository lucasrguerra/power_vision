/* Framework includes */
#include <Arduino.h>


/* FreeRTOS includes */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


/* Board includes */
#include <esp32-hal-adc.h>
#include <esp_adc_cal.h>
#include <driver/adc.h>


/* Math includes */
#include <arduinoFFT.h>


/* Project definitions */
#define SENOIDE_SAMPLE_RATE 512
#define ADC_ATTENUATION ADC_ATTEN_DB_11
#define ADC_WIDTH ADC_WIDTH_BIT_12
#define DEFAULT_VREF 1100


/* Tasks handlers */
TaskHandle_t hTask_readVoltageAndCurrent = NULL;
TaskHandle_t hTask_calculateData = NULL;
TaskHandle_t hTask_printData = NULL;


/* Tasks prototypes */
void vTask_readVoltageAndCurrent(void *pvParameters);
void vTask_calculateData(void *pvParameters);
void vTask_printData(void *pvParameters);


/* Global variables */
uint8_t system_frequency = 60;
float sample_us_interval = (float(1000) / float(system_frequency) / float(SENOIDE_SAMPLE_RATE)) * 950;
esp_adc_cal_characteristics_t adc_characteristics;
uint32_t samples_counter = 0;
uint16_t voltage[SENOIDE_SAMPLE_RATE];
uint16_t current[SENOIDE_SAMPLE_RATE];
arduinoFFT current_FFT = arduinoFFT();
float frequency = 0;
float voltage_rms = 0;
float current_rms = 0;
float apparent_power = 0;
float active_power = 0;
float reactive_power = 0;
float power_factor = 0;
float total_harmonic_distortion = 0;
float fundamental_harmonic_power = 0;

/* Setup function */
void setup() {
  /* Serial port configuration */
  Serial.begin(115200);


  /* ADC configuration */
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTENUATION);
  adc1_config_channel_atten(ADC1_CHANNEL_1, ADC_ATTENUATION);


  /* Calibrate ADC */
  esp_adc_cal_value_t adc_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTENUATION, ADC_WIDTH, DEFAULT_VREF, &adc_characteristics);
  if (adc_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.println("eFuse Vref");
  } else if (adc_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    Serial.println("Two Point");
  } else {
    Serial.println("Default");
  }


  /* Create task to read ADC */
  xTaskCreate(vTask_readVoltageAndCurrent, "Task_readVoltageAndCurrent", 2048, NULL, 1, &hTask_readVoltageAndCurrent);
  xTaskCreate(vTask_calculateData, "Task_calculateData", 16384, NULL, 1, &hTask_calculateData);
  vTaskDelay(2000);
  xTaskCreate(vTask_printData, "Task_printData", 2048, NULL, 1, &hTask_printData);
}


/* Loop function */
void loop() {
  vTaskDelay(10000);
}


/* Task readVoltageAndCurrent*/
void vTask_readVoltageAndCurrent(void *pvParameters) {
  uint16_t raw_voltage = 0;
  uint16_t raw_current = 0;
  while (1) {
    for (int i = 0; i < SENOIDE_SAMPLE_RATE; i++) {
      raw_voltage = adc1_get_raw(ADC1_CHANNEL_0);
      raw_current = adc1_get_raw(ADC1_CHANNEL_1);
      voltage[i] = esp_adc_cal_raw_to_voltage(raw_voltage, &adc_characteristics);
      current[i] = esp_adc_cal_raw_to_voltage(raw_current, &adc_characteristics);
      samples_counter++;
      esp_rom_delay_us(sample_us_interval);
    }
    vTaskDelay(1);
  }
}


/* Task calculateData */
void vTask_calculateData(void *pvParameters) {
  while (1) {
    uint64_t voltage_integral = 0;
    uint64_t current_integral = 0;
    uint64_t active_power_integral = 0;
    double reactive_power_integral = 0;
    int zero_crossings = 0;
    bool last_sample_positive = voltage[0] > 0;
    double current_real[SENOIDE_SAMPLE_RATE];
    double current_imaginary[SENOIDE_SAMPLE_RATE];

    for (int i = 0; i < SENOIDE_SAMPLE_RATE; i++) {
      uint16_t voltage_data = voltage[i];
      uint16_t current_data = current[i];

      voltage_integral += voltage_data * voltage_data;
      current_integral += current_data * current_data;
      int temp_apparent_power = voltage_data * current_data;
      active_power_integral += temp_apparent_power;
      reactive_power_integral += temp_apparent_power * sin(376.991118431  * (float(i) / float(SENOIDE_SAMPLE_RATE)));

      bool current_sample_positive = voltage_data > 0;
      if (current_sample_positive != last_sample_positive) {
        zero_crossings++;
      }
      last_sample_positive = current_sample_positive;

      current_real[i] = current_data;
      current_imaginary[i] = 0;
    }

    current_FFT.Windowing(current_real, SENOIDE_SAMPLE_RATE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    current_FFT.Compute(current_real, current_imaginary, SENOIDE_SAMPLE_RATE, FFT_FORWARD);
    current_FFT.ComplexToMagnitude(current_real, current_imaginary, SENOIDE_SAMPLE_RATE);
    double total_harmonic_power = 0;
    fundamental_harmonic_power = current_real[1];
    for (int i = 1; i < SENOIDE_SAMPLE_RATE / 2; i++) {
      total_harmonic_power += current_real[i] * current_real[i];
    }

    voltage_rms = sqrt(float(voltage_integral) / float(SENOIDE_SAMPLE_RATE));
    current_rms = sqrt(float(current_integral) / float(SENOIDE_SAMPLE_RATE));
    apparent_power = (voltage_rms * current_rms) / 1000;
    active_power = (active_power_integral / SENOIDE_SAMPLE_RATE) / 1000;
    reactive_power = (reactive_power_integral / SENOIDE_SAMPLE_RATE) / 1000;
    power_factor = active_power / apparent_power;
    frequency = (float(zero_crossings) / float(SENOIDE_SAMPLE_RATE));
    total_harmonic_distortion = (sqrt(total_harmonic_power) / fundamental_harmonic_power) * 100;
    vTaskDelay(20);
  }
}


/* Task printData */
void vTask_printData(void *pvParameters) {
  while (1) {
    String data;
    data = "Frequency: " + String(frequency) + " Hz\n";
    data += "Voltage RMS: " + String(voltage_rms) + " Vac\n";
    data += "Current RMS: " + String(current_rms) + " A\n";
    data += "Apparent Power: " + String(apparent_power) + " kVA\n";
    data += "Active Power: " + String(active_power) + " kW\n";
    data += "Power Factor: " + String(power_factor) + " %\n";
    data += "Reactive Power: " + String(reactive_power) + " kVAR\n";
    data += "Total Harmonic Distortion: " + String(total_harmonic_distortion) + " %\n";
    data += "Fundamental Harmonic Power: " + String(fundamental_harmonic_power) + " A\n";

    Serial.println(data);
    samples_counter = 0;
    vTaskDelay(1000);
  }
}