/*
 * 6axis.c
 *
 *  Created on: Nov 12, 2024
 *      Author: nelven
 */

#include "6axis.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;




void check_device_communication() {
    uint8_t deviceID = 0;
    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Read(&hi2c1, (uint8_t)(SENSOR_ADDRESS), 0x0F, I2C_MEMADD_SIZE_8BIT, &deviceID, 1, HAL_MAX_DELAY);

    if (status == HAL_OK) {
        printf("Communication réussie.\r\n");
    } else {
        printf("Échec de la communication.\r\n");
    }
}

void Init_HighPerf_Mode_6_axis(void)
{
    uint8_t data = 0;

    // Activer le mode haute performance pour l'accéléromètre et le gyroscope
    data = 0xA0; // 208 Hz, ±16g pour l'accéléromètre 0x54 (Actuel 6,6 kHz et ±2g)
    HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, CTRL1_XL, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    data = 0xAC; // 208 Hz, ±2000 dps pour le gyroscope 0x4C (Actuel 6,6 kHz et 2000 dps)
    HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, CTRL2_G, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Activer l'incrémentation automatique des adresses et l'update des données
    data = 0x00; // Incrémentation automatique activée, BDU activé
    HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, CTRL3_C, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Configurer la bande passante et autres options
    data = 0x00; // Paramètre par défaut pour CTRL6_C
    HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, CTRL6_C, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Configuration supplémentaire du gyroscope
    data = 0x00; // Paramètre par défaut pour CTRL7_G
    HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, CTRL7_G, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    // Configuration supplémentaire pour l'accéléromètre
    data = 0x00; // Paramètre par défaut pour CTRL8_XL
    HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, CTRL8_XL, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    uint8_t status = 0;

        // Lire le registre de statut via STATUS_REG pour voir si les données sont prêtes (XLDA et GDA)
        HAL_I2C_Mem_Read(&hi2c1, SENSOR_ADDRESS, STATUS_REG, I2C_MEMADD_SIZE_8BIT, &status, 1, HAL_MAX_DELAY);

        if (status & 0x01) {
            printf("Les données de l'accéléromètre sont prêtes.\r\n");
        }
        if (status & 0x02) {
            printf("Les données du gyroscope sont prêtes.\r\n");
        }

}

HAL_StatusTypeDef Read_sensor_data(int16_t* accel_data, int16_t* gyro_data) {
    HAL_StatusTypeDef status;
    uint8_t status_reg;
    uint8_t low_byte, high_byte;

    // Étape 1 : Lire STATUS_REG pour vérifier XLDA et GDA
    status = HAL_I2C_Mem_Read(&hi2c1, SENSOR_ADDRESS, STATUS_REG, I2C_MEMADD_SIZE_8BIT, &status_reg, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) return status;

    // Vérifier si les bits XLDA (bit 0) et GDA (bit 1) sont à 1
//    if ((((status_reg & 0x01)) && (((status_reg) & 0x02)))){
//        // Pas de nouvelles données prêtes
//        return HAL_OK;
//    }else{
//        return HAL_ERROR;
//    }
    if ((status_reg & 0x03) != 0x03) {
        return HAL_OK; // Pas de nouvelles données prêtes
    }

	// Lire les données du gyroscope
	for (int i = 0; i < 3; i++) {
		// Adresses des registres pour chaque axe
		uint8_t low_addr = G_X_OUT_L + i * 2;
		uint8_t high_addr = low_addr + 1;

		// Lire l'octet bas
		status = HAL_I2C_Mem_Read(&hi2c1, SENSOR_ADDRESS, low_addr, I2C_MEMADD_SIZE_8BIT, &low_byte, 1, HAL_MAX_DELAY);
		if (status != HAL_OK) return status;

		// Lire l'octet haut
		status = HAL_I2C_Mem_Read(&hi2c1, SENSOR_ADDRESS, high_addr, I2C_MEMADD_SIZE_8BIT, &high_byte, 1, HAL_MAX_DELAY);
		if (status != HAL_OK) return status;

		// Combiner les octets pour obtenir la valeur 16 bits
		gyro_data[i] = (int16_t)(((high_byte << 8) | low_byte) * GYRO_SENSITIVITY_2000DPS);
	}
	// Lire les données de l'accéléromètre
	for (int i = 0; i < 3; i++) {
		// Adresses des registres pour chaque axe
		uint8_t low_addr = XL_X_OUT_L + i * 2;
		uint8_t high_addr = low_addr + 1;

		// Lire l'octet bas
		status = HAL_I2C_Mem_Read(&hi2c1, SENSOR_ADDRESS, low_addr, I2C_MEMADD_SIZE_8BIT, &low_byte, 1, HAL_MAX_DELAY);
		if (status != HAL_OK) return status;

		// Lire l'octet haut
		status = HAL_I2C_Mem_Read(&hi2c1, SENSOR_ADDRESS, high_addr, I2C_MEMADD_SIZE_8BIT, &high_byte, 1, HAL_MAX_DELAY);
		if (status != HAL_OK) return status;

		// Combiner les octets pour obtenir la valeur 16 bits
		accel_data[i] = (int16_t)(((high_byte << 8) | low_byte) * ACC_SENSITIVITY_2G);
	}

	return HAL_OK;
}

void Display_6_axis_data(void) {
    int16_t accel_data[3], gyro_data[3]; // Tableau pour stocker les données d'accéléromètre et de gyroscope
    HAL_StatusTypeDef status;

    // Lire les données du capteur
    status = Read_sensor_data(accel_data, gyro_data);

    if (status == HAL_OK) {
        // Afficher les valeurs dans le format demandé
        printf("XL/(X,Y,Z) [mg] : %d ; %d ; %d\r\n", accel_data[0], accel_data[1], accel_data[2]);
        printf("G/(X,Y,Z) [mdps] : %d; %d; %d\r\n", gyro_data[0], gyro_data[1], gyro_data[2]);
    } else {
        printf("Erreur de lecture du capteur --> ");
        check_device_communication();
    }
}






