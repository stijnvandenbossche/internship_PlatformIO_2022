/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <errno.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef BOARD_STM32
#include "stm32_includes.h"
#endif

#ifdef BOARD_ESP32
#include "esp32_includes.h"
#endif




//#include "common_includes.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */




#ifdef BOARD_STM32
#include "platformio_logo.h"
#endif 

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */




/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/*SSI values*/
#define AMOUNT_SSI_TAGS 13

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#ifdef BOARD_STM32
CRC_HandleTypeDef hcrc;
DMA2D_HandleTypeDef hdma2d;
I2C_HandleTypeDef hi2c3;
LTDC_HandleTypeDef hltdc;
SD_HandleTypeDef hsd1;
HAL_SD_CardInfoTypeDef SDCardInfo1;
DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
SDRAM_HandleTypeDef hsdram1;

UART_HandleTypeDef huart1;
#endif


/*SSI TAGS*/
#ifdef BOARD_STM32
char* ssiTags[AMOUNT_SSI_TAGS]={"DATE","TIME","LWIPVERS","MSG01","MSG02","MSG03","MSG04","MSG05","MSG06","MSG07","MSG08","MSG09","MSG10"};

extern unsigned short PLATFORMIO_LOGO_DATA[];

extern uint32_t background_color;
extern bgcolors_array[];
extern MENU_BUTTON menu_button;
extern IP_BUTTON ip_button;
extern LCD_ELEMENT lcd_element_ip_address;
extern MENU menu;
extern PICTURE pio_logo;
extern BG_COLORS bg_colors;
extern CHAT_BOX chat_box;
extern char chat_messages[][MAX_LENGTH_CHAT_MESSAGE];
#endif

#ifdef BOARD_ESP32
uint16_t* data_buf;
uint16_t background_color_esp;
sFONT defaultFont;
#endif


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
#ifdef BOARD_STM32
void SystemClock_Config(void);
#endif
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*printing over USART*/

#ifdef BOARD_STM32
int _write(int file, char *ptr, int len) {
    HAL_StatusTypeDef xStatus;
    switch (file) {
    case STDOUT_FILENO: /*stdout*/
		xStatus = HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
		if (xStatus != HAL_OK) {
			errno = EIO;
			return -1;
		}
        break;
    case STDERR_FILENO: /* stderr */
		xStatus = HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
		if (xStatus != HAL_OK) {
			errno = EIO;
			return -1;
		}
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}
#endif
#ifdef BOARD_ESP32
static const char *TAG = "main";
#endif




/*Handlers for CGI & SSI*/

/*parameters are in pcParam, values in pcValue*/

#ifdef BOARD_STM32
void httpd_cgi_handler(struct fs_file *file, const char* uri, int iNumParams,char **pcParam, char **pcValue){
  
  if(strncmp(uri,"/options.html",strlen("/options.html"))==0){
    for(int i=0;i<iNumParams;i++){
      if(strncmp(pcParam[i],"bgcolor",strlen("bgcolor"))==0){
        /*checking selected color*/
        if(strncmp(pcValue[i],"darkred",strlen("darkred"))==0){
          background_color = LCD_COLOR_DARKRED;
        } else if(strncmp(pcValue[i],"red",strlen("red"))==0){
          background_color = LCD_COLOR_RED;
        } else if(strncmp(pcValue[i],"lightred",strlen("lightred"))==0){
          background_color = LCD_COLOR_LIGHTRED;
        } else if(strncmp(pcValue[i],"orange",strlen("orange"))==0){
          background_color = LCD_COLOR_ORANGE;
        } else if(strncmp(pcValue[i],"darkyellow",strlen("darkyellow"))==0){
          background_color = LCD_COLOR_DARKYELLOW;
        } 
        
        else if(strncmp(pcValue[i],"yellow",strlen("yellow"))==0){
          background_color = LCD_COLOR_YELLOW;
        } else if(strncmp(pcValue[i],"lightyellow",strlen("lightyellow"))==0){
          background_color = LCD_COLOR_LIGHTYELLOW;
        } else if(strncmp(pcValue[i],"lightgreen",strlen("lightgreen"))==0){
          background_color = LCD_COLOR_LIGHTGREEN;
        } else if(strncmp(pcValue[i],"green",strlen("green"))==0){
          background_color = LCD_COLOR_GREEN;
        } else if(strncmp(pcValue[i],"darkgreen",strlen("darkgreen"))==0){
          background_color = LCD_COLOR_DARKGREEN;
        } 

        else if(strncmp(pcValue[i],"darkcyan",strlen("darkcyan"))==0){
          background_color = LCD_COLOR_DARKCYAN;
        } else if(strncmp(pcValue[i],"cyan",strlen("cyan"))==0){
          background_color = LCD_COLOR_CYAN;
        } else if(strncmp(pcValue[i],"lightblue",strlen("lightblue"))==0){
          background_color = LCD_COLOR_LIGHTBLUE;
        } else if(strncmp(pcValue[i],"blue",strlen("blue"))==0){
          background_color = LCD_COLOR_BLUE;
        } else if(strncmp(pcValue[i],"darkblue",strlen("darkblue"))==0){
          background_color = LCD_COLOR_DARKBLUE;
        }

        else if(strncmp(pcValue[i],"darkmagenta",strlen("darkmagenta"))==0){
          background_color = LCD_COLOR_DARKMAGENTA;
        } else if(strncmp(pcValue[i],"magenta",strlen("magenta"))==0){
          background_color = LCD_COLOR_MAGENTA;
        } else if(strncmp(pcValue[i],"lightmagenta",strlen("lightmagenta"))==0){
          background_color = LCD_COLOR_LIGHTMAGENTA;
        } else if(strncmp(pcValue[i],"lightgray",strlen("lightgray"))==0){
          background_color = LCD_COLOR_LIGHTGRAY;
        } else if(strncmp(pcValue[i],"white",strlen("white"))==0){
          background_color = LCD_COLOR_WHITE;
        }

        /*re-rendering frame to display color*/
        renderFrame();
      }
    }
  }

  /*checking for chat messages*/
  if(strncmp(uri,"/chat.shtml",strlen("/chat.shtml"))==0){
    for(int i=0;i<iNumParams;i++){
      if(strncmp(pcParam[i],"chatmessage",strlen("chatmessage"))==0){
        /* Storing message in chat_messages array, loop to store in first available place.
           When everything is full, shift all messages and lose the oldest one
        */

        processChatMessage(pcValue[i]);
               
      }
    }
  }

  return;
}

uint16_t mySsiHandler(const char* ssi_tag_name, char* pcInsert, int iInsertLen){
  
  /*checking for DATE*/
  if(strncmp(ssi_tag_name,ssiTags[0],strlen(ssiTags[0]))==0){
    strncpy(pcInsert,__DATE__,iInsertLen);
    return strlen(__DATE__);
  }
  /*checking for TIME*/
  else if(strncmp(ssi_tag_name,ssiTags[1],strlen(ssiTags[1]))==0){
    strncpy(pcInsert,__TIME__,iInsertLen);
    return strlen(__TIME__);
  }
  /*checking for LWIPVERS*/
  else if(strncmp(ssi_tag_name,ssiTags[2],strlen(ssiTags[2]))==0){
    strncpy(pcInsert,LWIP_VERSION_STRING,iInsertLen);
    return strlen(LWIP_VERSION_STRING);
  }
  

  /*checking for chat messages*/
  else{
    for(int i=0;i<(chat_box.amountmessages);i++){
      /*to be checked*/
      if(strncmp(ssi_tag_name,ssiTags[3+i],strlen(ssiTags[3+i]))==0){
        strncpy(pcInsert,chat_messages[i], iInsertLen);
        return strlen(chat_messages[i]);
      }
    }
  }

  return 0;
}
#endif




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  
  /* USER CODE BEGIN 2 */

  #ifdef BOARD_STM32
  http_set_ssi_handler(mySsiHandler, ssiTags,AMOUNT_SSI_TAGS);
  init_stm32();

  /*Touch Initialisations*/
  TS_StateTypeDef touchstate;
  BSP_TS_Init(480,272);
  #endif
  //vTaskDelay(500);

  #ifdef BOARD_ESP32
  lcd_config_t lcd_config = {
  #ifdef CONFIG_LCD_ST7789
        .clk_fre         = 80 * 1000 * 1000, /*!< ILI9341 Stable frequency configuration */
  #endif
  #ifdef CONFIG_LCD_ILI9341
        .clk_fre         = 40 * 1000 * 1000, /*!< ILI9341 Stable frequency configuration */
  #endif
        .pin_clk         = LCD_CLK,
        .pin_mosi        = LCD_MOSI,
        .pin_dc          = LCD_DC,
        .pin_cs          = LCD_CS,
        .pin_rst         = LCD_RST,
        .pin_bk          = -1, //changed to be able to use audio board for buttons
        .max_buffer_size = 2 * 1024,
        .horizontal      = 2, /*!< 2: UP, 3: DOWN */
        .swap_data       = 1,
    };

    lcd_init(&lcd_config);
    init_ESP_LCD_Buffer();
    /*data_buf = (uint16_t *)heap_caps_calloc(SCREEN_XSIZE * SCREEN_YSIZE, sizeof(uint16_t), MALLOC_CAP_8BIT);
    
    if(data_buf != NULL){
        ESP_LOGI(TAG,"LCD calloc ok");
    }else{
        ESP_LOGI(TAG,"LCD calloc not ok");   
    }*/

    //ESP_LOGI(TAG,"databuf: %d",(int)data_buf); 
    background_color_esp = LCD_COLOR_LIGHTBLUE;
    defaultFont = Font16;
    fillBackground();
  #endif

  



  renderFrame();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    #ifdef BOARD_STM32
    MX_LWIP_Process();       

    /*Polling touchscreen*/
    BSP_TS_GetState(&touchstate);
    if(touchstate.touchDetected>=1){
      /*Function to handle touches*/  
      handleTouch( *(touchstate.touchX) , *(touchstate.touchY) );
      HAL_Delay(100);
    }
    #endif

    #ifdef BOARD_ESP32
    //vTaskDelay(100);
    
    
    #endif
  }
  /* USER CODE END 3 */
}

#ifdef BOARD_ESP32
void app_main(){
  main();
}
#endif









//extra functions for stm
#ifdef BOARD_STM32
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {

  }
  /* USER CODE END Error_Handler_Debug */
}

#endif

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
