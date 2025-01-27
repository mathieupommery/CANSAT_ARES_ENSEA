/*
 * 6axis.c
 *
 *  Created on: Jan 23, 2025
 *      Author: louisvoz
 */

#include "main.h"
#include "tarvos.h"
#include "usart.h"
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "app_fatfs.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "usb.h"
#include "gpio.h"
#include "fatfs_sd.h"
#include "nmea_parse.h"
#include <stdint.h>
#define CTRL1_XL 0x10
#define CTRL2_G  0x11
#define CTRL3_C  0x12
#define CTRL6_C  0x15
#define CTRL7_G  0x16
#define CTRL8_XL 0x17
#define STATUS_REG 0x1E
#define G_X_OUT_L 0x22             // Adresse du premier registre de sortie gyroscope (G_)
#define XL_X_OUT_L 0x28            // Adresse du premier registre de sortie accéléromètre (XL_)
// Sensibilités pour accéléromètre et gyroscope
#define ACC_SENSITIVITY_16G 0.488 // Sensibilité pour ±16g
#define GYRO_SENSITIVITY_2000DPS 70 // Sensibilité pour ±2000 dps
#define SENSOR_ADDRESS  0x6B << 1

int16_t buffer6axe[6];




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






int16_t read_sensor_axis(uint8_t base_addr, uint8_t axis_index) {
	uint8_t low_byte, high_byte;
	uint8_t low_addr = base_addr + axis_index * 2;
       uint8_t high_addr = low_addr + 1;
       HAL_StatusTypeDef status;

       // Lire l'octet bas
       status = HAL_I2C_Mem_Read(&hi2c3, SENSOR_ADDRESS, low_addr, I2C_MEMADD_SIZE_8BIT, &low_byte, 1, HAL_MAX_DELAY);
       if (status != HAL_OK) return -1; // Retourner une erreur en cas d'échec

       // Lire l'octet haut
       status = HAL_I2C_Mem_Read(&hi2c3, SENSOR_ADDRESS, high_addr, I2C_MEMADD_SIZE_8BIT, &high_byte, 1, HAL_MAX_DELAY);
       if (status != HAL_OK) return -1; // Retourner une erreur en cas d'échec

       // Combiner les octets
       return (int16_t)((high_byte << 8) | low_byte);
 }





void Read_sensor_data(void) {
    //static char buffer[42]; // Buffer statique pour stocker les données formatées
    //


    // Fonction pour lire une valeur 16 bits à partir de deux adresses


    // Fonction pour décomposer une valeur en chiffres et la copier dans le buffer
//    void decompose_to_buffer(int16_t value, char *buffer_offset) {
//        for (int i = 0; i < 6; i++) {
//            buffer_offset[5 - i] = (value % 10) + '0'; // Convertir en caractère ASCII
//            value /= 10;
//        }
//    }

    // Lire les données des gyroscopes
    for (int i = 0; i < 3; i++) {
    	buffer6axe[i] = read_sensor_axis(G_X_OUT_L, i);

    }

    for (int i = 0; i < 3; i++) {
        buffer6axe[i+3] = read_sensor_axis(XL_X_OUT_L, i);
    }
}
// Reception de la Trame : $TT.T,ddmm.mmmm,N,dddmm.mmmm,E,hhmmss.s,hhh.h,XXXX,YYYYY,ZZZZ$ 60 octets

/* USER CODE END 0 */
