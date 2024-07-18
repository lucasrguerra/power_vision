/* ADC Main Header */
#pragma once



/* includes */
#include <esp_adc/adc_cali_scheme.h>
#include <esp_adc/adc_cali.h>
#include <driver/gpio.h>
#include <driver/adc.h>



/* Pins definitions */
#define WAVE_A_VOLTAGE_PIN          GPIO_NUM_4
#define WAVE_B_VOLTAGE_PIN          GPIO_NUM_5
#define WAVE_C_VOLTAGE_PIN          GPIO_NUM_6
#define WAVE_A_CURRENT_PIN          GPIO_NUM_7
#define WAVE_B_CURRENT_PIN          GPIO_NUM_8
#define WAVE_C_CURRENT_PIN          GPIO_NUM_9
#define WAVE_N_CURRENT_PIN          GPIO_NUM_10
#define WAVE_A_VOLTAGE_CHANNEL      ADC1_CHANNEL_3
#define WAVE_B_VOLTAGE_CHANNEL      ADC1_CHANNEL_4
#define WAVE_C_VOLTAGE_CHANNEL      ADC1_CHANNEL_5
#define WAVE_A_CURRENT_CHANNEL      ADC1_CHANNEL_6
#define WAVE_B_CURRENT_CHANNEL      ADC1_CHANNEL_7
#define WAVE_C_CURRENT_CHANNEL      ADC1_CHANNEL_8
#define WAVE_N_CURRENT_CHANNEL      ADC1_CHANNEL_9



/* Definitions */
#define ADC_UNIT                    ADC_UNIT_1
#define ADC_ATTENUATION             ADC_ATTEN_DB_12
#define ADC_BITWIDTH                ADC_BITWIDTH_12
#define ADC_DEFAULT_VREF            1100



/* Global Variables */
extern adc_cali_handle_t calibration_handle;
extern adc_cali_curve_fitting_config_t calibration_config;



/* Function Prototypes */
void calibrateADC();
uint16_t getSample(adc1_channel_t channel);