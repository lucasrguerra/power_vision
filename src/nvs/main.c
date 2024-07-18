#include "main.h"



/*
    * Function to initialize the NVS
*/
void initializeNVS() {
    esp_err_t error = nvs_flash_init();
    if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        error = nvs_flash_init();
    }
    ESP_ERROR_CHECK(error);
}



/*
  * Function to save a string in the NVS
  * 
  * @param key: The key to be saved
*/
void saveStringInNVS(char *key, char *value) {
    nvs_handle_t nvs_handle;
    nvs_open(NVS_KEY, NVS_READWRITE, &nvs_handle);
    nvs_set_str(nvs_handle, key, value);
    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
}



/*
  * Function to read a string from the NVS
  * 
  * @param key: The key to be read
  * @return char *: The value read
*/
char *readStringFromNVS(char *key) {
    nvs_handle_t nvs_handle;
    esp_err_t error = nvs_open(NVS_KEY, NVS_READONLY, &nvs_handle);
    if (error != ESP_OK) { return ""; }
    size_t required_size;
    nvs_get_str(nvs_handle, key, NULL, &required_size);
    char *value = (char *) malloc(required_size);
    nvs_get_str(nvs_handle, key, value, &required_size);
    nvs_close(nvs_handle);
    return value;
}



/*
  * Function to clear the NVS
*/
void clearNVS() {
    nvs_flash_erase();
}