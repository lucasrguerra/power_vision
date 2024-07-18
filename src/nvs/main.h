/* NVS Main Header */
#pragma once



/* includes */
#include <nvs_flash.h>
#include <nvs.h>



/* Definitions */
#define NVS_KEY                     "powervision"



/* Function Prototypes */
void initializeNVS();
void saveStringInNVS(char *key, char *value);
char *readStringFromNVS(char *key);
void clearNVS();