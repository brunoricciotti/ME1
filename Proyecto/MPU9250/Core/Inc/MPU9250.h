/*
 * MPU9250.h
 *
 *  Created on: Sep 11, 2022
 *      Author: bruno
 */

#ifndef INC_MPU9250_H_
#define INC_MPU9250_H_

#include <stdint.h>

#define RAD2DEG 57.2957795131

// Defines
#define WHO_AM_I_9250_ANS 0x70
#define WHO_AM_I          0x75
#define AD0_LOW           0x68
#define AD0_HIGH          0x69
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define PWR_MGMT_1        0x6B
#define PWR_MGMT_2        0x6C
#define SMPLRT_DIV_REG	  0x19
#define ACCEL_XOUT_H      0x3B
#define I2C_TIMOUT_MS     1000

#define MPU9250_ADDR      0xD0 //0x68 << 1

void MPU9250_init(void);
int16_t MPU_readRawData(void);
void MPU9250_Read_Accel (void);

enum accelerometerFullScaleRange
{
    AFSR_2G,
    AFSR_4G,
    AFSR_8G,
    AFSR_16G
};

#endif /* INC_MPU9250_H_ */
