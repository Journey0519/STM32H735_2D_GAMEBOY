/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FDCAN2_TX_Pin GPIO_PIN_6
#define FDCAN2_TX_GPIO_Port GPIOB
#define JTDO_Pin GPIO_PIN_3
#define JTDO_GPIO_Port GPIOB
#define OCSPI2_IO7_Pin GPIO_PIN_11
#define OCSPI2_IO7_GPIO_Port GPIOG
#define OCSPI1_IO6_Pin GPIO_PIN_9
#define OCSPI1_IO6_GPIO_Port GPIOG
#define Detectn_Pin GPIO_PIN_1
#define Detectn_GPIO_Port GPIOD
#define JTDI_Pin GPIO_PIN_15
#define JTDI_GPIO_Port GPIOA
#define JTCK_Pin GPIO_PIN_14
#define JTCK_GPIO_Port GPIOA
#define SAI4_D2_Pin GPIO_PIN_4
#define SAI4_D2_GPIO_Port GPIOE
#define ARD_D8_Pin GPIO_PIN_3
#define ARD_D8_GPIO_Port GPIOE
#define ARD_D9_Pin GPIO_PIN_7
#define ARD_D9_GPIO_Port GPIOB
#define NJTRST_Pin GPIO_PIN_4
#define NJTRST_GPIO_Port GPIOB
#define USB_FS_OVCR_Pin GPIO_PIN_13
#define USB_FS_OVCR_GPIO_Port GPIOG
#define OCSPI1_IO7_Pin GPIO_PIN_7
#define OCSPI1_IO7_GPIO_Port GPIOD
#define OCSPI1_IO5_Pin GPIO_PIN_5
#define OCSPI1_IO5_GPIO_Port GPIOD
#define SDIO1_CMD_Pin GPIO_PIN_2
#define SDIO1_CMD_GPIO_Port GPIOD
#define SDIO1_CK_Pin GPIO_PIN_12
#define SDIO1_CK_GPIO_Port GPIOC
#define JTMS_Pin GPIO_PIN_13
#define JTMS_GPIO_Port GPIOA
#define USB_FS_DP_Pin GPIO_PIN_12
#define USB_FS_DP_GPIO_Port GPIOA
#define Blue_button_B2_used_for_wakeup_Pin GPIO_PIN_13
#define Blue_button_B2_used_for_wakeup_GPIO_Port GPIOC
#define OCSPI1_IO2_Pin GPIO_PIN_2
#define OCSPI1_IO2_GPIO_Port GPIOE
#define FDCAN2_RX_Pin GPIO_PIN_5
#define FDCAN2_RX_GPIO_Port GPIOB
#define OCSPI2_IO6_Pin GPIO_PIN_10
#define OCSPI2_IO6_GPIO_Port GPIOG
#define OCSPI1_IO4_Pin GPIO_PIN_4
#define OCSPI1_IO4_GPIO_Port GPIOD
#define SDIO1_D3_Pin GPIO_PIN_11
#define SDIO1_D3_GPIO_Port GPIOC
#define SDIO1_D2_Pin GPIO_PIN_10
#define SDIO1_D2_GPIO_Port GPIOC
#define USB_FS_ID_Pin GPIO_PIN_10
#define USB_FS_ID_GPIO_Port GPIOA
#define USB_FS_DM_Pin GPIO_PIN_11
#define USB_FS_DM_GPIO_Port GPIOA
#define SAI4_CK2_Pin GPIO_PIN_5
#define SAI4_CK2_GPIO_Port GPIOE
#define LCD_BL_CTRL_Pin GPIO_PIN_15
#define LCD_BL_CTRL_GPIO_Port GPIOG
#define OCSPI2_NCS_Pin GPIO_PIN_12
#define OCSPI2_NCS_GPIO_Port GPIOG
#define USB_FS_VBUS_Pin GPIO_PIN_9
#define USB_FS_VBUS_GPIO_Port GPIOA
#define SDIO1_D0_Pin GPIO_PIN_8
#define SDIO1_D0_GPIO_Port GPIOC
#define SAI1_SD_A_Pin GPIO_PIN_6
#define SAI1_SD_A_GPIO_Port GPIOE
#define SDIO1_D1_Pin GPIO_PIN_9
#define SDIO1_D1_GPIO_Port GPIOC
#define OCSPI2_IO1_Pin GPIO_PIN_1
#define OCSPI2_IO1_GPIO_Port GPIOF
#define OCSPI2_IO0_Pin GPIO_PIN_0
#define OCSPI2_IO0_GPIO_Port GPIOF
#define OCSPI1_NCS_Pin GPIO_PIN_6
#define OCSPI1_NCS_GPIO_Port GPIOG
#define ARD_D7_Pin GPIO_PIN_5
#define ARD_D7_GPIO_Port GPIOG
#define OCSPI2_IO2_Pin GPIO_PIN_2
#define OCSPI2_IO2_GPIO_Port GPIOF
#define MEMS_LED_Pin GPIO_PIN_8
#define MEMS_LED_GPIO_Port GPIOG
#define ARD_D4_SHOT_Pin GPIO_PIN_4
#define ARD_D4_SHOT_GPIO_Port GPIOG
#define CTP_INT_Pin GPIO_PIN_2
#define CTP_INT_GPIO_Port GPIOG
#define SAI1_SD_B_Pin GPIO_PIN_6
#define SAI1_SD_B_GPIO_Port GPIOF
#define OCSPI2_CLK_Pin GPIO_PIN_4
#define OCSPI2_CLK_GPIO_Port GPIOF
#define OCSPI2_IO3_Pin GPIO_PIN_3
#define OCSPI2_IO3_GPIO_Port GPIOF
#define ARD_D2_JUMP_Pin GPIO_PIN_3
#define ARD_D2_JUMP_GPIO_Port GPIOG
#define OCSPI1_IO3_Pin GPIO_PIN_13
#define OCSPI1_IO3_GPIO_Port GPIOD
#define SAI1_SCK_B_Pin GPIO_PIN_8
#define SAI1_SCK_B_GPIO_Port GPIOF
#define SAI1_MCLK_B_Pin GPIO_PIN_7
#define SAI1_MCLK_B_GPIO_Port GPIOF
#define SAI1_FS_B_Pin GPIO_PIN_9
#define SAI1_FS_B_GPIO_Port GPIOF
#define OCSPI1_IO0_Pin GPIO_PIN_11
#define OCSPI1_IO0_GPIO_Port GPIOD
#define OCSPI1_IO1_Pin GPIO_PIN_12
#define OCSPI1_IO1_GPIO_Port GPIOD
#define OCSPI1_CLK_Pin GPIO_PIN_10
#define OCSPI1_CLK_GPIO_Port GPIOF
#define T_VCP_RX_Pin GPIO_PIN_9
#define T_VCP_RX_GPIO_Port GPIOD
#define ARD_D0_Pin GPIO_PIN_15
#define ARD_D0_GPIO_Port GPIOB
#define ARD_D1_Pin GPIO_PIN_14
#define ARD_D1_GPIO_Port GPIOB
#define ARD_A0_Pin GPIO_PIN_0
#define ARD_A0_GPIO_Port GPIOC
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define LCD_DISP_Pin GPIO_PIN_10
#define LCD_DISP_GPIO_Port GPIOD
#define T_VCP_TX_Pin GPIO_PIN_8
#define T_VCP_TX_GPIO_Port GPIOD
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define USER_LED2_Pin GPIO_PIN_2
#define USER_LED2_GPIO_Port GPIOC
#define USER_LED1_Pin GPIO_PIN_3
#define USER_LED1_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define SPI5_MISO_Pin GPIO_PIN_7
#define SPI5_MISO_GPIO_Port GPIOH
#define ARD_D5_Pin GPIO_PIN_14
#define ARD_D5_GPIO_Port GPIOE
#define ARD_A4_Pin GPIO_PIN_2
#define ARD_A4_GPIO_Port GPIOC
#define ARD_A5_Pin GPIO_PIN_3
#define ARD_A5_GPIO_Port GPIOC
#define ARD_A1_Pin GPIO_PIN_2
#define ARD_A1_GPIO_Port GPIOH
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define SPI5_MOSI_Pin GPIO_PIN_11
#define SPI5_MOSI_GPIO_Port GPIOF
#define Audio_Int_Pin GPIO_PIN_8
#define Audio_Int_GPIO_Port GPIOE
#define OCSPI2_IO5_Pin GPIO_PIN_1
#define OCSPI2_IO5_GPIO_Port GPIOG
#define I2C4_SDA_Pin GPIO_PIN_15
#define I2C4_SDA_GPIO_Port GPIOF
#define STMOD_20_Pin GPIO_PIN_13
#define STMOD_20_GPIO_Port GPIOF
#define RMII_RX_ER_Pin GPIO_PIN_10
#define RMII_RX_ER_GPIO_Port GPIOB
#define STMOD_11_INT_Pin GPIO_PIN_12
#define STMOD_11_INT_GPIO_Port GPIOH
#define ARD_D3_Pin GPIO_PIN_0
#define ARD_D3_GPIO_Port GPIOA
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define ARD_A3_Y_Pin GPIO_PIN_1
#define ARD_A3_Y_GPIO_Port GPIOA
#define DAC1_OUT2_Pin GPIO_PIN_5
#define DAC1_OUT2_GPIO_Port GPIOA
#define OCSPI1_DQS_Pin GPIO_PIN_2
#define OCSPI1_DQS_GPIO_Port GPIOB
#define OCSPI2_IO4_Pin GPIO_PIN_0
#define OCSPI2_IO4_GPIO_Port GPIOG
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOB
#define OCSPI2_DQS_Pin GPIO_PIN_12
#define OCSPI2_DQS_GPIO_Port GPIOF
#define LCD_RST_Pin GPIO_PIN_6
#define LCD_RST_GPIO_Port GPIOH
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define ARD_A2_X_Pin GPIO_PIN_0
#define ARD_A2_X_GPIO_Port GPIOA
#define USB_FS_PWR_EN_Pin GPIO_PIN_5
#define USB_FS_PWR_EN_GPIO_Port GPIOH
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define STMOD_19_Pin GPIO_PIN_10
#define STMOD_19_GPIO_Port GPIOE
#define I2C4_SCL_Pin GPIO_PIN_14
#define I2C4_SCL_GPIO_Port GPIOF
#define STMOD_18_Pin GPIO_PIN_9
#define STMOD_18_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
