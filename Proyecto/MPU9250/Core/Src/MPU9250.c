/*
 * MPU9250.c
 *
 *  Created on: Sep 11, 2022
 *      Author: bruno
 */

#include "MPU9250.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

int16_t Ax;
int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;


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
		Data = 0x1F;
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

int16_t MPU_readRawData(void)
{
    // Init buffer
    uint8_t buf[2];

    // Subroutine for reading the raw data
    HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDR, ACCEL_XOUT_H, 1, buf, 2, I2C_TIMOUT_MS);

    // Bit shift the data
    Ax = buf[0] << 8 | buf[1];

	// Para leer el angulo: arcsin(Ax/16384)
    return Ax;

}

void MPU9250_Read_Accel (void)//esto no lo uso todavia
{
	uint8_t Rec_Data[6];

	//Solo usamos Ax

	HAL_I2C_Mem_Read (&hi2c1, MPU9250_ADDR, ACCEL_XOUT_H, 1, Rec_Data, 6, I2C_TIMOUT_MS);

	Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);//De esta forma leemos 2 bytes

	//Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	//Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	/*** convert the RAW values into acceleration in 'g'
	     we have to divide according to the Full scale value set in FS_SEL
	     I have configured FS_SEL = 0. So I am dividing by 16384.0
	     for more details check ACCEL_CONFIG Register              ****/

	//Ax = Accel_X_RAW/32768.0;
	//Ay = Accel_Y_RAW/32768.0;
	//Az = Accel_Z_RAW/32768.0;
}
