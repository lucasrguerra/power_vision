extern "C" {
    void initializeWiFi();
    void initializeWiFiAP();
}



#include "main.h"



/* WiFi Configures */
char WIFI_SSID[WIFI_STRING_SIZE] = "";
char WIFI_PASSWORD[WIFI_STRING_SIZE] = "";
char mac_address_string[18];

wifi_init_config_t wifi_initial_configures;
wifi_config_t wifi_configures = {
    .sta = {
        .ssid = "",
        .password = "",
    },
};
uint8_t wifi_retry_counter = 0;

bool wifi_connected = false;
bool wifi_ap_started = false;
bool wifi_connect_error = false;



/*
    * Function to handle the WiFi events
    * 
    * @param event_handler_arg: The event handler arguments
    * @param event_base: The event base
    * @param event_id: The event ID
    * @param event_data: The event data
*/
void wifiEventHandler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data) {
    if (event_id == WIFI_EVENT_STA_START) {
        printf("Connecting to %s\n", WIFI_SSID);
    } else if (event_id == WIFI_EVENT_STA_CONNECTED) {
        wifi_retry_counter = 0;
        printf("Connected to %s\n", WIFI_SSID);
    } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (wifi_retry_counter < WIFI_MAXIMUM_RETRY) {
            esp_wifi_connect();
            wifi_retry_counter++;
            printf("Connection to %s failed. Retrying...\n", WIFI_SSID);
        } else {
            wifi_connect_error = true;
            printf("Connection to %s failed. Maximum retries reached\n", WIFI_SSID);
        }
    } else if (event_id == IP_EVENT_STA_GOT_IP) {
        wifi_connected = true;
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        printf("Got IP: %s\n", ip4addr_ntoa((const ip4_addr_t*)&event->ip_info.ip));
    }
}



/*
  * Function to get the MAC address
*/
void getMacAddress() {
  uint8_t mac_address[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac_address);
  sprintf(mac_address_string, "%02x:%02x:%02x:%02x:%02x:%02x", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
}



/*
    * Function to initialize the WiFi
*/
void initializeWiFi() {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_initial_configures = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initial_configures);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifiEventHandler, NULL);

    strcpy((char *) wifi_configures.sta.ssid, WIFI_SSID);
    strcpy((char *) wifi_configures.sta.password, WIFI_PASSWORD);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_configures);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    getMacAddress();
    
    esp_wifi_connect();
}



/*
    * Function to initialize the WiFi AP
*/
void initializeWiFiAP() {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();

    wifi_initial_configures = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initial_configures);
    
    wifi_config_t wifi_ap_configures = {
        .ap = {
        .ssid = "Power Vision",
        .password = "12345678",
        .max_connection = 5,
        },
    };
    esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_configures);
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_start();
    wifi_ap_started = true;
}