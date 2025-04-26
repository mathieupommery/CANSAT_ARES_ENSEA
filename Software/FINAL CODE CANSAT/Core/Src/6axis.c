/*
 * 6axis.c
 *
 *  Created on: Jan 31, 2025
 *      Author: louisvoz
 */

#include "6axis.h"

extern I2C_HandleTypeDef hi2c3;


void check_device_communication() {
    uint8_t deviceID = 0;
    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Read(&hi2c3, (uint8_t)(SENSOR_ADDRESS), 0x0F, I2C_MEMADD_SIZE_8BIT, &deviceID, 1, HAL_MAX_DELAY);

    if (status == HAL_OK) {
        printf("Communication réussie.\r\n");
    } else {
        printf("Échec de la communication.\r\n");
    }
}

// Fonction d'initialisation du capteur en mode haute performance
void Init_HighPerf_Mode_6_axis(void)
{
    uint8_t data = 0;

    // Activer le mode haute performance pour l'accéléromètre et le gyroscope
    data = 0x54; // 208 Hz, ±16g pour l'accéléromètre
    HAL_I2C_Mem_Write(&hi2c3, SENSOR_ADDRESS, CTRL1_XL, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    data = 0x4C; // 208 Hz, ±2000 dps pour le gyroscope
    HAL_I2C_Mem_Write(&hi2c3, SENSOR_ADDRESS, CTRL2_G, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Activer l'incrémentation automatique des adresses et l'update des données
    data = 0x00; // Incrémentation automatique activée, BDU activé
    HAL_I2C_Mem_Write(&hi2c3, SENSOR_ADDRESS, CTRL3_C, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Configurer la bande passante et autres options
    data = 0x00; // Paramètre par défaut pour CTRL6_C
    HAL_I2C_Mem_Write(&hi2c3, SENSOR_ADDRESS, CTRL6_C, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Configuration supplémentaire du gyroscope
    data = 0x00; // Paramètre par défaut pour CTRL7_G
    HAL_I2C_Mem_Write(&hi2c3, SENSOR_ADDRESS, CTRL7_G, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Configuration supplémentaire pour l'accéléromètre
    data = 0x00; // Paramètre par défaut pour CTRL8_XL
    HAL_I2C_Mem_Write(&hi2c3, SENSOR_ADDRESS, CTRL8_XL, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    uint8_t status = 0;

        // Lire le registre de statut via STATUS_REG pour voir si les données sont prêtes (XLDA et GDA)
        HAL_I2C_Mem_Read(&hi2c3, SENSOR_ADDRESS, STATUS_REG, I2C_MEMADD_SIZE_8BIT, &status, 1, HAL_MAX_DELAY);

        if (status & 0x01) {
            printf("Les données de l'accéléromètre sont prêtes. q\r\n");
        }
        if (status & 0x02) {
            printf("Les données du gyroscope sont prêtes.\r\n");
        }

}


HAL_StatusTypeDef Read_sensor_data(AXIS6 *data) {
	int16_t accel_data[3],gyro_data[3];
    HAL_StatusTypeDef status;
    uint8_t status_reg;
    uint8_t low_byte, high_byte;
    // Étape 1 : Lire STATUS_REG pour vérifier XLDA et GDA
    status = HAL_I2C_Mem_Read(&hi2c3, SENSOR_ADDRESS, STATUS_REG, I2C_MEMADD_SIZE_8BIT, &status_reg, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) return status;

    // Vérifier si les bits XLDA (bit 0) et GDA (bit 1) sont à 1
    if (!(status_reg & 0x01) || !(status_reg & 0x02)) {
        // Pas de nouvelles données prêtes
        return HAL_ERROR;
    }
	// Lire les données gyro
	for (int i = 0; i < 3; i++) {
		// Adresses des registres pour chaque axe
		uint8_t low_addr = G_X_OUT_L + i * 2;
		uint8_t high_addr = low_addr + 1;

		// Lire l'octet bas
		status = HAL_I2C_Mem_Read(&hi2c3, SENSOR_ADDRESS, low_addr, I2C_MEMADD_SIZE_8BIT, &low_byte, 1, HAL_MAX_DELAY);
		if (status != HAL_OK) return status;

		// Lire l'octet haut
		status = HAL_I2C_Mem_Read(&hi2c3, SENSOR_ADDRESS, high_addr, I2C_MEMADD_SIZE_8BIT, &high_byte, 1, HAL_MAX_DELAY);
		if (status != HAL_OK) return status;

		// Combiner les octets pour obtenir la valeur 16 bits
		gyro_data[i] = (int16_t)((high_byte << 8) | low_byte);
	}
	// Lire les données accel
	for (int i = 0; i < 3; i++) {
		// Adresses des registres pour chaque axe
		uint8_t low_addr = XL_X_OUT_L + i * 2;
		uint8_t high_addr = low_addr + 1;

		// Lire l'octet bas
		status = HAL_I2C_Mem_Read(&hi2c3, SENSOR_ADDRESS, low_addr, I2C_MEMADD_SIZE_8BIT, &low_byte, 1, HAL_MAX_DELAY);
		if (status != HAL_OK) return status;

		// Lire l'octet haut
		status = HAL_I2C_Mem_Read(&hi2c3, SENSOR_ADDRESS, high_addr, I2C_MEMADD_SIZE_8BIT, &high_byte, 1, HAL_MAX_DELAY);
		if (status != HAL_OK) return status;

		// Combiner les octets pour obtenir la valeur 16 bits
		accel_data[i] = (int16_t)((high_byte << 8) | low_byte);
	}
	data->AccelX = accel_data[0];
	data->AccelY = accel_data[1];
	data->AccelZ = accel_data[2];
	data->GyroX = gyro_data[0];
	data->GyroY = gyro_data[1];
	data->GyroZ = gyro_data[2];
	data->temp  = Read_temp()-8.5;

	return HAL_OK;
}

double Read_temp() {
    HAL_StatusTypeDef status;
    uint8_t status_reg;
    uint8_t low_byte, high_byte;

    // Lire l'octet bas
    status = HAL_I2C_Mem_Read(&hi2c3, SENSOR_ADDRESS, 0x20, I2C_MEMADD_SIZE_8BIT, &low_byte, 1, HAL_MAX_DELAY);

    // Lire l'octet haut
    status = HAL_I2C_Mem_Read(&hi2c3, SENSOR_ADDRESS, 0x21, I2C_MEMADD_SIZE_8BIT, &high_byte, 1, HAL_MAX_DELAY);

    // Reconstruction du 16 bits signé
    int16_t temp = (int16_t)((high_byte << 8) | low_byte);

    // Conversion en °C
    double temperature = (temp / 256.0) + 25.0;

    return temperature;
}




/* USER CODE END 0 */
