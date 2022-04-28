/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#ifdef BOARD_STM32
#include "stm32f7xx_hal.h"
#endif


//Common includes
#include "lcd_elements.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */




void clearBGFG(void);
void handleTouch(uint16_t , uint16_t);
void renderFrame(void);
void clearElements(void);
void drawMenuButton(void);
void drawBgColorOptions(void);
void drawIPButton(void);
void drawIPAddress(void);
void drawMenu(void);
void drawPIOLogo(void);
void goToPage(int page);
int touchInBoundary(uint16_t, uint16_t, LCD_ELEMENT);
int touchInBoundaryCoords(uint16_t, uint16_t, int, int, int, int);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_DISPLAY_Pin GPIO_PIN_12
#define LCD_DISPLAY_GPIO_Port GPIOI
/* USER CODE BEGIN Private defines */

#define USER_BUTTON GPIO_PIN_11
#define BUTTON_GPIO_PORT GPIOI

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
