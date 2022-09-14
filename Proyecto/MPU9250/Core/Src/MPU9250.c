/*
 * MPU9250.c
 *
 *  Created on: Sep 11, 2022
 *      Author: bruno
 */

#include "MPU9250.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

void MPU9250_init(void){

	uint8_t check = 0;
	uint8_t Data;

	// check device ID WHO_AM_I

	HAL_I2C_Mem_Read (&hi2c1, MPU9250_ADDR,WHO_AM_I,1, &check, 1, I2C_TIMOUT_MS);

	if (check == WHO_AM_I_9250_ANS)  // 0x71 will be returned by the sensor if everything goes well
	{
		// power management 1 register 0X6B we should write all 0's to wake the sensor up
		Data = 0;
		HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, PWR_MGMT_1, 1,&Data, 1, 1000);

		// power management 2 register 0X6C habilito solo el eje x del acelerometro
		Data = 0x20;
		HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, PWR_MGMT_2, 1,&Data, 1, 1000);

		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		Data = 0x07;
		HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, 1000);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
		Data = 0x00;
		HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, ACCEL_CONFIG, 1, &Data, 1, 1000);
	}

}

uint16_t MPU_readRawData(void)
{
    // Init buffer
    uint8_t buf[2];
    uint16_t Ax;

    // Subroutine for reading the raw data
    HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDR, ACCEL_XOUT_H, 1, buf, 2, I2C_TIMOUT_MS);

    // Bit shift the data
    Ax = buf[0] << 8 | buf[1];

    return Ax;

}
