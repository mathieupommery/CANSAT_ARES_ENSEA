/*
 * 6axis.c
 *
 *  Created on: Jan 23, 2025
 *      Author: louisvoz
 */
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

int check_device_communication() {
    uint8_t deviceID = 0;
    HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Read(&hi2c1, (uint8_t)(SENSOR_ADDRESS), 0x0F, I2C_MEMADD_SIZE_8BIT, &deviceID, 1, HAL_MAX_DELAY);

    if (status == HAL_OK) {
        return 1;
    } else {
        return 0;
    }
}



// Fonction d'initialisation du capteur en mode haute performance
void Init_HighPerf_Mode_6_axis(void)
{
    uint8_t data = 0;

    // Activer le mode haute performance pour l'accéléromètre et le gyroscope
    data = 0x54; // 208 Hz, ±16g pour l'accéléromètre
    HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDRESS, CTRL1_XL, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);

    data = 0x4C; // 208 Hz, ±2000 dps pour le gyroscope
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
            printf("Les données de l'accéléromètre sont prêtes. q\r\n");
        }
        if (status & 0x02) {
            printf("Les données du gyroscope sont prêtes.\r\n");
        }

}

uint8_t Read_sensor_data() {
	int16_t accel_data[3];
	int16_t gyro_data[3];
    HAL_StatusTypeDef status;
    uint8_t status_reg;
    uint8_t low_byte, high_byte;
    uint8_t buffer[41];

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
		gyro_data[i] = (int16_t)((high_byte << 8) | low_byte);
		snprintf((uint8_t *)buffer,32,"%d",g);

		buffer[0+7i] = gyro_data[i] % 10;              // Unités
		buffer[1+7i] = (gyro_data[i] / 10) % 10;       // Dizaines
		buffer[2+7i] = (gyro_data[i] / 100) % 10;      // Centaines
		buffer[3+7i] = (gyro_data[i] / 1000) % 10;     // Milliers
		buffer[4+7i] = (gyro_data[i] / 10000) % 10;    // Dizaines de milliers
		buffer[5+7i] = (gyro_data[i] / 100000) % 10;   // Centaines de milliers
		buffer[6+7i] = ","
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
		accel_data[i] = (int16_t)((high_byte << 8) | low_byte);

		buffer[0+7i+21] = accel_data[i] % 10;              // Unités
		buffer[1+7i+21] = (accel_data[i] / 10) % 10;       // Dizaines
		buffer[2+7i+21] = (accel_data[i] / 100) % 10;      // Centaines
		buffer[3+7i+21] = (accel_data[i] / 1000) % 10;     // Milliers
		buffer[4+7i+21] = (accel_data[i] / 10000) % 10;    // Dizaines de milliers
		buffer[5+7i+21] = (accel_data[i] / 100000) % 10;   // Centaines de milliers
		if (i!=2){
			buffer[6+6i+21] = ",";
		}
	}
	return buffer;
}
// Reception de la Trame : $TT.T,ddmm.mmmm,N,dddmm.mmmm,E,hhmmss.s,hhh.h,XXXX,YYYYY,ZZZZ$ 60 octets


void Display_6_axis_data(void) {
    int16_t accel_data[3], gyro_data[3]; // Tableau pour stocker les données d'accéléromètre et de gyroscope
    HAL_StatusTypeDef status;

    // Lire les données du capteur
    status = Read_sensor_data(accel_data, gyro_data);

    if (status == HAL_OK) {
        // Afficher les valeurs dans le format demandé
        printf("XL/(X,Y,Z) : %d ; %d ; %d\r\n", accel_data[0], accel_data[1], accel_data[2]);
        printf("G/(X,Y,Z) : %d ; %d ; %d\r\n", gyro_data[0], gyro_data[1], gyro_data[2]);
    } else {
        printf("Erreur de lecture du capteur --> ");
        check_device_communication();
    }
}

/* USER CODE END 0 */
