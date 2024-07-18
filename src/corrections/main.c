#include "main.h"



/* Corrections Configuration */
float VOLTAGE_CORRECTION_FACTOR_A = 1.186;
float VOLTAGE_CORRECTION_FACTOR_B = -1956.427;
float CURRENT_CORRECTION_FACTOR = 0.866;



/*
    * Function to set the voltage type
    *
    * @param voltage_type: The voltage type
*/
void setVoltageType(uint8_t voltage_type) {
    switch (voltage_type) {
        case 0:
            VOLTAGE_CORRECTION_FACTOR_A = 1.186;
            VOLTAGE_CORRECTION_FACTOR_B = -1956.427;
            break;
        case 1:
            VOLTAGE_CORRECTION_FACTOR_A = 1.093;
            VOLTAGE_CORRECTION_FACTOR_B = -1802.399;
            break;
        case 2:
            VOLTAGE_CORRECTION_FACTOR_A = 0.698;
            VOLTAGE_CORRECTION_FACTOR_B = -1150.607;
            break;
        case 3:
            VOLTAGE_CORRECTION_FACTOR_A = 0.419;
            VOLTAGE_CORRECTION_FACTOR_B = -690.569;
            break;
        default:
            VOLTAGE_CORRECTION_FACTOR_A = 1.186;
            VOLTAGE_CORRECTION_FACTOR_B = -1956.427;
            printf("Invalid voltage type. Defaulting to 1.186 and -1956.427\n");
            break;
    }
}



/*
    * Function to set the current type
    *
    * @param current_type: The current type
*/
void setCurrentType(uint8_t current_type) {
    switch (current_type) {
        case 0:
            CURRENT_CORRECTION_FACTOR = 0.866;
            break;
        case 1:
            CURRENT_CORRECTION_FACTOR = 0.642;
            break;
        case 2:
            CURRENT_CORRECTION_FACTOR = 0.403;
            break;
        case 3:
            CURRENT_CORRECTION_FACTOR = 0.255;
            break;
        case 4:
            CURRENT_CORRECTION_FACTOR = 0.173;
            break;
        case 5:
            CURRENT_CORRECTION_FACTOR = 0.096;
            break;
        case 6:
            CURRENT_CORRECTION_FACTOR = 0.064;
            break;
        case 7:
            CURRENT_CORRECTION_FACTOR = 0.096;
            break;
        case 8:
            CURRENT_CORRECTION_FACTOR = 0.017;
            break;
        default:
            CURRENT_CORRECTION_FACTOR = 0.866;
            printf("Invalid current type. Defaulting to 0.866\n");
            break;
    }
}



/*
    * Function to get the corrected voltage
    *
    * @param voltage: The voltage
    * @return: The corrected voltage
*/
float getCorrectedVoltage(float voltage) {
    return (VOLTAGE_CORRECTION_FACTOR_A * voltage) + VOLTAGE_CORRECTION_FACTOR_B;
}



/*
    * Function to get the corrected current
    *
    * @param current: The current
    * @return: The corrected current
*/
float getCorrectedCurrent(float current) {
    return CURRENT_CORRECTION_FACTOR * current;
}
