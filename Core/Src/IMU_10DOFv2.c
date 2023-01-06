/**
  ******************************************************************************
  * @file           : IMU_10DOFv2.c
  * @brief          : Fichier contenant les fonctions en rapport avec le composant IMU_10DOFv2
  *
  * Created on: 13 déc. 2022
  * Author: quent
  ******************************************************************************
  */
#include "IMU_10DOFv2.h"



/**
 * @brief fonction qui permet de verifier qu'on arrive à communiquer avec le capteur BMP280
 *
 * @param bcom
 *
 * @retval None
 */
void SearchBMP280()
{
	int result;
	uint8_t bcom[MAX_COM_BUF];

	bcom[0] = BMP_ID_REG;
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
 *
 * @retval None
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
 * @brief Fonction qui vient tester la presence de capteurs sur le bus I2C et les print via UART
 *
 * @param None
 *
 * @retval None
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
 *
 * @retval None
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

/**
 * @brief Fonction qui reset les valeurs stockées dans les registres et initialises les PLL
 *
 * @param i2c_handle
 *
 * @retval None
 */
void Init_IMU_10DOF(I2C_HandleTypeDef* i2c_handle)
{
	int result;
	uint8_t bcom[MAX_COM_BUF];


	//HW Reset
	printf("debut reset value\r\n");

	printf("debut set PLL\r\n");
	bcom[0] = 0x02; //choix de la PLL donnant le plus de precision
	result = HAL_I2C_Mem_Write(&hi2c1, MPU_ADDR, PWR_MGMT_1,1, bcom, 1, HAL_MAX_DELAY); //bcom = addresse de bcom[0]
	if(result != HAL_OK)
	{
		printf("Error I2C Mem Write Init_IMU_10_DOF Reset MPU\r\n");
		Error_Handler();
	}

	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, PWR_MGMT_1, 1, bcom, 1, HAL_MAX_DELAY);
	printf("La valeur de la PLL est : 0x%x\r\n", bcom[0]);


	printf("Init MPU\r\n");

	bcom[0] = MPU_RESET_VALUE;
	result = HAL_I2C_Mem_Write(&hi2c1, MPU_ADDR, PWR_MGMT_1,1, bcom, 1, HAL_MAX_DELAY); //bcom = addresse de bcom[0]
	if(result != HAL_OK)
	{
		printf("Error I2C Mem Write Init_IMU_10_DOF Reset MPU\r\n");
		Error_Handler();
	}
	result = HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, PWR_MGMT_1,1, bcom+1, 1, HAL_MAX_DELAY); //bcom = addresse de bcom[0]
	/*if(result != HAL_OK)
	{
		printf("Error I2C Mem Read Init_IMU_10_DOF Reset MPU\r\n");
		printf("result = %d\r\n", result);
		Error_Handler();
	}
	printf("reset value MPU = 0x%x\r\n",bcom[1]);*/





	printf("\nInit BMP\r\n");


	bcom[0] = BMP_RESET_VALUE;
	result = HAL_I2C_Mem_Write(&hi2c1, BMP_ADDR, PWR_MGMT_1,1, bcom, 1, HAL_MAX_DELAY); //bcom = addresse de bcom[0]
	if(result != HAL_OK)
	{
		printf("Error I2C Mem Write Init_IMU_10_DOF Reset BMP\r\n");
		Error_Handler();
	}
	result = HAL_I2C_Mem_Read(&hi2c1, BMP_ADDR, PWR_MGMT_1,1, bcom+1, 1, HAL_MAX_DELAY); //bcom = addresse de bcom[0]
	if(result != HAL_OK)
	{
		printf("Error I2C Mem Read Init_IMU_10_DOF Reset BMP\r\n");
		printf("result = %d\r\n", result);
		Error_Handler();
	}
	printf("reset value BMP = 0x%x\r\n",bcom[1]);


	HAL_Delay(100); //laisse le temps d'effacer tous les registres
	printf("\nfin reset value\r\n\n");


}

/**
 * @brief Fonction qui permet de lire la temperature depuis les registres puis de la convertir en une valeur exploitable en °C
 *
 * @param I2C_HandleTypeDef* i2c_handle, double* temp
 *
 * @retval None
 */
void Measure_T(I2C_HandleTypeDef* i2c_handle, double* temp)
{
	uint8_t tab_temp[2]; //car valeures non signées

	HAL_I2C_Mem_Read(&hi2c1, MPU_ADDR, TEMP_OUT_H, 1, tab_temp, 2, HAL_MAX_DELAY);
	*temp = (((tab_temp[0]<<8)+tab_temp[1]) - 21.0)/333.87 + 21.0;
	printf("temp = %1.1f\r\n", *temp);
}

void Measure_A(I2C_HandleTypeDef* i2c_handle, double* accel)
{
	uint8_t tab_accel[6]; //car valeures signées

	HAL_I2C_Mem_Read(i2c_handle, MPU_ADDR, ACCEL_XOUT_H, 1, tab_accel, 6, HAL_MAX_DELAY);

	int16_t Xbrut = ((tab_accel[0]<<8) + tab_accel[1]) ;
	int16_t Ybrut = ((tab_accel[2]<<8) + tab_accel[3]) ;
	int16_t Zbrut = ((tab_accel[4]<<8) + tab_accel[5]) ;

	float X = Xbrut / ACCEL_FS_SEL_2G;
	float Y = Ybrut / ACCEL_FS_SEL_2G;
	float Z = Zbrut / ACCEL_FS_SEL_2G;

	float G = sqrt(pow(Xbrut, 2)+pow(Ybrut, 2)+pow(Zbrut, 2))/ ACCEL_FS_SEL_2G;

	printf("X = %f		Y = %f		Z = %f		G = %f\r\n", X,Y,Z,G);
}

void Measure_G(I2C_HandleTypeDef* i2c_handle, double* gyro)
{
	uint8_t tab_gyro[6]; //car valeures signées

	HAL_I2C_Mem_Read(i2c_handle, MPU_ADDR, GYRO_XOUT_H, 1, tab_gyro, 6, HAL_MAX_DELAY);

	int16_t Xbrut = ((tab_gyro[0]<<8) + tab_gyro[1]) ;
	int16_t Ybrut = ((tab_gyro[2]<<8) + tab_gyro[3]) ;
	int16_t Zbrut = ((tab_gyro[4]<<8) + tab_gyro[5]) ;

	float X = Xbrut / SENSITIVITY_SCALE_FACTOR;
	float Y = Ybrut / SENSITIVITY_SCALE_FACTOR;
	float Z = Zbrut / SENSITIVITY_SCALE_FACTOR;

	float V = sqrt(pow(Xbrut, 2)+pow(Ybrut, 2)+pow(Zbrut, 2))/ SENSITIVITY_SCALE_FACTOR;

	printf("Vx = %f		Vy = %f		Vz = %f		V = %f\r\n", X,Y,Z,V);

}
