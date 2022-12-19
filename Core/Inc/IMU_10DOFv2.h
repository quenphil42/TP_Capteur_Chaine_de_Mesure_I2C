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


#define MAX_COM_BUF 8 //8bit et 6 registres
#define PWR_MGMT_1  0x6B

#define BMP_ADDR 0x77<<1 // on decale de 1 car le lsb est dedié au R/W et est géré par HAL_Transmit et receive
#define BMP_ID_REG 0xD0
#define BMP_WHO_AM_I_ID_VAL 0x58 //valeur qu'on doit trouver
#define BMP_RESET_VALUE 0xB6


#define MPU_ADDR 0x68<<1 //adresse sur le hardware
#define MPU_ID_REG 0x75
#define MPU_WHO_AM_I_ID_VAL 0x71 //valeur qu'on doit trouver
#define MPU_RESET_VALUE 0x80

#define TEMP_OUT_H 0x65







void SearchBMP280();
void SearchMPU9250();
void TestSensorOnI2C(int * tab);
void PrintTab(int * tab);
void Init_IMU_10DOF(I2C_HandleTypeDef*);
void Measure_T(I2C_HandleTypeDef* i2c_handle, double* temp);

#endif /* INC_IMU_10DOFV2_H_ */
