/*
 * IMU_10DOFv2.h
 *
 *  Created on: 13 déc. 2022
 *      Author: quent
 */

#ifndef INC_IMU_10DOFV2_H_
#define INC_IMU_10DOFV2_H_

//#include "Const.h"
#include "i2c.h"
#include "main.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"


#define MAX_COM_BUF 8 //8bit et 6 registres
#define PWR_MGMT_1  0x6B





#define BMP_ADD 					0x18
#define BMP_ADDR 					0xEE
#define BMP_WHO_AM_I				0x00	//offset
#define BMP_WHO_AM_I_ID_VAL 		0x48    //valeur qu'on doit trouver 72
#define BMP_RESET_VALUE 			0xB6

#define MAGNETO_CNTL 				0x0A	//offset
#define MAGNETO_I2CDIS				0x0F	//offset

#define MAGNETO_ASAX				0x10 //valeur ajustement valeur brut selon X
#define MAGNETO_ASAY				0x11
#define MAGNETO_ASAZ				0x12

#define MAGNETO_HXL					0x03 //valeur brut
#define MAGNETO_HXH					0x04
#define MAGENTO_HYL					0x05
#define MAGNETO_HYH					0x06
#define MAGNETO_HZL					0x07
#define MAGNETO_HZH					0x08


#define MPU_ADDR 0xD0 //adresse sur le hardware
#define MPU_ID_REG 0x75
#define MPU_WHO_AM_I_ID_VAL 0x71 //valeur qu'on doit trouver
#define MPU_RESET_VALUE 0x80
#define MPU_INTERNAL_BYPASS_CONFIG 0x37


#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define ACCEL_FS_SEL_2G	16384.0



#define GYRO_XOUT_H      0x43
#define GYRO_XOUT_L      0x44
#define GYRO_YOUT_H      0x45
#define GYRO_YOUT_L      0x46
#define GYRO_ZOUT_H      0x47
#define GYRO_ZOUT_L      0x48
#define SENSITIVITY_SCALE_FACTOR 131.0








void SearchBMP280();
void SearchMPU9250();
void TestSensorOnI2C(int * tab);
void PrintTab(int * tab);
void Init_IMU_10DOF(I2C_HandleTypeDef*);
void Measure_T(I2C_HandleTypeDef* i2c_handle, double* temp);
void Measure_A(I2C_HandleTypeDef* i2c_handle, double* accel);
void Measure_G(I2C_HandleTypeDef* i2c_handle, double* gyro);
void Measure_M(I2C_HandleTypeDef* i2c_handle, double* magneto);

#endif /* INC_IMU_10DOFV2_H_ */
