/* Corrections Main Header */
#pragma once



/* includes */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>



/* Global variables */
extern float VOLTAGE_CORRECTION_FACTOR_A;
extern float VOLTAGE_CORRECTION_FACTOR_B;
extern float CURRENT_CORRECTION_FACTOR;



/* Function Prototypes */
void setVoltageType(uint8_t voltage_type);
void setCurrentType(uint8_t current_type);
float getCorrectedVoltage(float voltage);
float getCorrectedCurrent(float current);