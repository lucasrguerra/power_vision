/* WiFi Main Header */
#pragma once



/* includes */
#include <lwip/ip4_addr.h>
#include <wifi/main.h>
#include <esp_wifi.h>
#include <lwip/err.h>
#include <lwip/sys.h>



/* Definitions */
#define WIFI_MAXIMUM_RETRY          5
#define WIFI_STRING_SIZE            64
#define WIFI_SSID_KEY               "ssid"
#define WIFI_PASSWORD_KEY           "pass"



/* Global Variables */
extern char WIFI_SSID[WIFI_STRING_SIZE];
extern char WIFI_PASSWORD[WIFI_STRING_SIZE];
extern char mac_address_string[18];

extern wifi_init_config_t wifi_initial_configures;
extern wifi_config_t wifi_configures;

extern uint8_t wifi_retry_counter;
extern bool wifi_connected;
extern bool wifi_ap_started;
extern bool wifi_connect_error;



/* Function Prototypes */
void initializeWiFi();
void initializeWiFiAP();