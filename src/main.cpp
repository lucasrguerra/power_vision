extern "C" {
  void app_main(void);
}



/* Project includes */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>



/* ESP32 includes */
#include <esp_adc/adc_cali_scheme.h>
#include <esp_adc/adc_cali.h>
#include <esp_http_client.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include <driver/adc.h>
#include <nvs_flash.h>
#include <esp_timer.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <lwip/err.h>
#include <lwip/sys.h>



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



/* ADC definitions */
#define ADC_UNIT                    ADC_UNIT_1
#define ADC_ATTENUATION             ADC_ATTEN_DB_12
#define ADC_DEFAULT_VREF            1100



/* UART definitions */
#define UART_BAUD_RATE              115200
#define UART_PORT                   UART_NUM_0
#define UART_BUFFER_SIZE            4096



/* Measurement definitions */
#define SYSTEM_FREQUENCY            60                                            // Change this value according to the schematic "Type Of Electrical System" table
#define CURRENT_TYPE                8                                             // Change this value according to the schematic "Resistor to Current" table
#define VOLTAGE_TYPE                2                                             // Change this value according to the schematic "Resistor to Voltage" table
#define SYSTEM_TYPE                 0                                             // Change this value according to the schematic "Type Of Electrical System" table
#define SENOIDE_SAMPLE_RATE         2160                                          // Number of samples to be taken from the sine wave SIGNAL
#define SENOIDE_SAMPLE_PERIOD       (1000000 / SENOIDE_SAMPLE_RATE) + 1           // Period of the sine wave SIGNAL



/* WiFi definitions */
#define WIFI_MAXIMUM_RETRY          5
#define WIFI_SSID                   "War Zone"
#define WIFI_PASSWORD               "nao_tem_senha"



/* Global variables */
static adc_cali_handle_t calibration_handle;
char uart_buffer[UART_BUFFER_SIZE];

uint16_t samples_in_acquisition[7][SENOIDE_SAMPLE_RATE];
uint16_t samples[7][SENOIDE_SAMPLE_RATE];

uint16_t read_interrupt_counter = 0;
uint8_t wifi_retry_counter = 0;
bool start_calculation = false;



/* Function prototypes */
void configureGPIOs();
void calibrateADC();
void configureNVS();
uint16_t getSample(adc1_channel_t channel);
static void readInterrupt(void *arguments);
static void wifiEventHandler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data);
void configureAndStartWifi();



/* Configs */
adc_cali_curve_fitting_config_t calibration_config = {
  .atten = ADC_ATTENUATION,
  .bitwidth = ADC_BITWIDTH_12,
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
  // Initialize Serial
  uart_driver_install(UART_PORT, (UART_BUFFER_SIZE * 2), 0, 0, NULL, 0);
  uart_param_config(UART_PORT, &uart_config);
  sprintf(uart_buffer, "\n\n");
  uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));

  // Configure board
  configureGPIOs();
  calibrateADC();
  configureNVS();
  configureAndStartWifi();

  // Create and configure timer
  esp_timer_handle_t timer_readings;
  esp_timer_create(&timer_readings_args, &timer_readings);
  esp_timer_start_periodic(timer_readings, SENOIDE_SAMPLE_PERIOD);


  // Loop function
  while (1) {
    if (start_calculation) {
      
      sprintf(uart_buffer, "Voltage A: %d\n", samples[0][0]);
      uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));

      start_calculation = false;
    }
    vTaskDelay(1);
  }
}



/*
  * Function to configure the GPIOs
*/
void configureGPIOs() {
  gpio_set_direction(WAVE_A_VOLTAGE_PIN, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_B_VOLTAGE_PIN, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_C_VOLTAGE_PIN, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_A_CURRENT_PIN, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_B_CURRENT_PIN, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_C_CURRENT_PIN, GPIO_MODE_INPUT);
  gpio_set_direction(WAVE_N_CURRENT_PIN, GPIO_MODE_INPUT);
}



/*
  * Function to calibrate the ADC
*/
void calibrateADC() {
  adc1_config_width(ADC_WIDTH_BIT_12);
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
  * Function to get the voltage from a specific channel
  * 
  * @param channel: The channel to be read
  * @return uint16_t: The voltage value
*/
uint16_t getSample(adc1_channel_t channel) {
  int sample = 0;
  adc_cali_raw_to_voltage(calibration_handle, adc1_get_raw(channel), &sample);
  return (uint16_t) sample;
}



/*
  * Function to read the interrupt
  * 
  * @param arguments: The arguments to be passed
*/
static void readInterrupt(void *arguments) {
  samples_in_acquisition[0][read_interrupt_counter] = getSample(WAVE_A_VOLTAGE_CHANNEL);
  samples_in_acquisition[1][read_interrupt_counter] = getSample(WAVE_B_VOLTAGE_CHANNEL);
  samples_in_acquisition[2][read_interrupt_counter] = getSample(WAVE_C_VOLTAGE_CHANNEL);
  samples_in_acquisition[3][read_interrupt_counter] = getSample(WAVE_A_CURRENT_CHANNEL);
  samples_in_acquisition[4][read_interrupt_counter] = getSample(WAVE_B_CURRENT_CHANNEL);
  samples_in_acquisition[5][read_interrupt_counter] = getSample(WAVE_C_CURRENT_CHANNEL);
  samples_in_acquisition[6][read_interrupt_counter] = getSample(WAVE_N_CURRENT_CHANNEL);

  read_interrupt_counter++;
  if (read_interrupt_counter == SENOIDE_SAMPLE_RATE) {
    read_interrupt_counter = 0;
    start_calculation = true;
    memcpy(samples, samples_in_acquisition, sizeof(samples_in_acquisition));
  }
}



/*
  * Function to configure the NVS
*/
void configureNVS() {
  esp_err_t nvs_flash_error = nvs_flash_init();
  if (nvs_flash_error == ESP_ERR_NVS_NO_FREE_PAGES || nvs_flash_error == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    nvs_flash_error = nvs_flash_init();
  }
  ESP_ERROR_CHECK(nvs_flash_error);
}



/*
  * Function to handle the WiFi events
  * 
  * @param event_handler_arg: The event handler arguments
  * @param event_base: The event base
  * @param event_id: The event ID
  * @param event_data: The event data
*/
static void wifiEventHandler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data) {
  if (event_id == WIFI_EVENT_STA_START) {
    sprintf(uart_buffer, "Connecting to %s\n", WIFI_SSID);
    uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));
  } else if (event_id == WIFI_EVENT_STA_CONNECTED) {
    wifi_retry_counter = 0;
    sprintf(uart_buffer, "Connected to %s\n", WIFI_SSID);
    uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));
  } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (wifi_retry_counter < WIFI_MAXIMUM_RETRY) {
      esp_wifi_connect();
      wifi_retry_counter++;
      sprintf(uart_buffer, "Connection to %s failed. Retrying...\n", WIFI_SSID);
      uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));
    } else {
      sprintf(uart_buffer, "Connection to %s failed. Maximum retries reached.\n", WIFI_SSID);
      uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));
    }
  } else if (event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
    sprintf(uart_buffer, "Got IP: %d.%d.%d.%d\n", IP2STR(&event->ip_info.ip));
    uart_write_bytes(UART_PORT, uart_buffer, strlen(uart_buffer));
  }
}



/*
  * Function to configure and start the WiFi
*/
void configureAndStartWifi() {
  esp_netif_init();
  esp_event_loop_create_default();
  esp_netif_create_default_wifi_sta();
  wifi_init_config_t wifi_initial_configures = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&wifi_initial_configures);
  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, NULL);
  esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifiEventHandler, NULL);

  wifi_config_t wifi_configures = {
    .sta = {
      .ssid = "",
      .password = "",
    },
  };
  strcpy((char *) wifi_configures.sta.ssid, WIFI_SSID);
  strcpy((char *) wifi_configures.sta.password, WIFI_PASSWORD);
  esp_wifi_set_config(WIFI_IF_STA, &wifi_configures);
  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_start();
  esp_wifi_connect();
}