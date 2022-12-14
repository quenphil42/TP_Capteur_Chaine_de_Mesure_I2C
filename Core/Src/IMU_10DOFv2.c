/*
 * IMU_10DOFv2.c
 *
 *  Created on: 13 déc. 2022
 *      Author: quent
 */

#include "IMU_10DOFv2.h"



/**
 * @brief fonction qui permet de verifier qu'on arrive à communiquer avec le capteur BMP280
 *
 * @param bcom
 */
void SearchBMP280()
{
	int result;
	uint8_t bcom[MAX_COM_BUF];

	bcom[0] = BMP_ADDR;
	printf("Recherche Capteur BMP280\r\n\n");

	result = HAL_I2C_Master_Transmit(&hi2c1, BMP_ADDR, bcom, 1, HAL_TIMEOUT); //bcom = addresse de bcom[0]
	if(result != HAL_OK)
		{
			printf("Error I2C Transmit SearchBMP280\r\n");
			Error_Handler();
		}

	result = HAL_I2C_Master_Receive(&hi2c1, BMP_ADDR, bcom+1, 1, HAL_TIMEOUT);
	if(result != HAL_OK)
		{
			printf("Error I2C Receive SearchBMP280\r\n");
			Error_Handler();
		}

	printf("Registre : %d et contenu %d\r\n\n", bcom[0], bcom[1]);
	if (bcom[1]==BMP_WHO_AM_I_ID_VAL) printf("Capteur BMP280 trouve\r\n\n");
	else printf("Error : Capteur non trouve, verifier addresse ID_REG\r\n\n\n");
}


/**
 * @brief fonction qui permet de verifier qu'on arrive à communiquer avec le capteur MPU9250
 *
 * @param bcom (uint8)
 */
void SearchMPU9250()
{
	  int result;
	  uint8_t bcom[MAX_COM_BUF];

	  bcom[0] = MPU_ID_REG;
	  //Ici je vais envoyer à l'adresse hardware un handshake pr lui demander la valeur qu'il a à l'adresse WHO_AMI_I
	  printf("Recherche Capteur MPU9250\r\n\n");

	  result = HAL_I2C_Master_Transmit(&hi2c1, MPU_ADDR, bcom, 1, HAL_TIMEOUT); //bcom = addresse de bcom[0]
	  if(result != HAL_OK)
		{
		  printf("Error I2C Transmit SearchMPU9250\r\n");
		  Error_Handler();
		}

	  result = HAL_I2C_Master_Receive(&hi2c1, MPU_ADDR, bcom+1, 1, HAL_TIMEOUT);
	  if(result != HAL_OK)
	  {
	  	printf("Error I2C Receive SearchMPU9250\r\n");
	  	Error_Handler();
	  }

	  printf("En decimal, Registre : %d et contenu %d\r\n\n", bcom[0], bcom[1]);
	  if (bcom[1]==MPU_WHO_AM_I_ID_VAL) printf("Capteur MPU9250 trouve\r\n\n");
	  else printf("Error : Capteur non trouve, verifier addresse WHO_AM_I_MPU9250\r\n\n\n");
}

/**
 * @brief Fonction qui vient tester la presence de capteurs sur le bus I2C et les print via uart
 *
 */
void TestSensorOnI2C(int * tab)
{
	 int u = 0;

	 printf("debut de lecture du bus I2c\r\n\n");
	  for(int i =0; i< 256; i++)
	  {
		  int result = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i),2,2);
		  if (result != HAL_OK)//HAL_ERROR or HAL_TIMEOUT
		  {
			  printf("."); //No ACKreceived at the address
		  }
		  if (result == HAL_OK)
		  {
			  tab[u] = i;
			  u++;
			  printf("0x%X",i); //Received an ACK at the address
		  }
	  }
	  printf("\r\n\nfin de lecture du bus I2C\r\n\n\n");
}

/**
 * @brief cette fonction permet d'afficher le contenu du tableau tab
 *
 * @param tab
 */
void PrintTab(int * tab)
{
	printf("les adresses detectees sont :\r\n");
	for(int i = 0; i<8; i++)
	{
		if(tab[i]!=0) printf("%d\r\n", tab[i]);
		else printf("NULL\r\n");
	}

	printf("Rappel : \r\nLes adresses paires sont en ecritures\r\n"
			"les adresses impaires sont en lecture\r\n\n");

}

void Init_IMU_10DOF(I2C_HandleTypeDef* i2c_handle)
{
	int result;
	uint8_t bcom[MAX_COM_BUF];

	//HW Reset
	printf("debut reset value\r\n");
	printf("MPU\r\n");

	bcom[0] = MPU_RESET_VALUE;
	result = HAL_I2C_Mem_Write(&hi2c1, MPU_ADDR, PWR_MGMT_1,1, bcom, 1, HAL_MAX_DELAY); //bcom = addresse de bcom[0]
	if(result != HAL_OK)
	{
		printf("Error I2C Mem Write Init_IMU_10_DOF Reset BMP\r\n");
		Error_Handler();
	}
	result = HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, PWR_MGMT_1,1, bcom, 1, HAL_MAX_DELAY); //bcom = addresse de bcom[0]
	if(result != HAL_OK)
	{
		printf("Error I2C Mem Read Init_IMU_10_DOF Reset BMP\r\n");
		Error_Handler();
	}


	printf("fin reset value\r\n");
}
