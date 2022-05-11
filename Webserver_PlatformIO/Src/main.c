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
#include <sys/times.h>
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
#define AMOUNT_SSI_TAGS 3

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
char* ssiTags[AMOUNT_SSI_TAGS]={"DATE","TIME","LWIPVERS"};

extern unsigned short PLATFORMIO_LOGO_DATA[];

extern uint32_t background_color;
extern bgcolors_array[];
extern MENU_BUTTON menu_button;
extern IP_BUTTON ip_button;
extern LCD_ELEMENT lcd_element_ip_address;
extern MENU menu;
extern PICTURE pio_logo;
extern BG_COLORS bg_colors;
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
  /*checking if uri is /cgi, this is an empty file just for cgi handling*/
  if(strncmp(uri,"/options.html",strlen("/options.html"))==0){
    for(int i=0;i<iNumParams;i++){
      if(strncmp(pcParam[i],"bgcolor",strlen("bgcolor"))==0){
        /*checking selected color*/
        if(strncmp(pcValue[i],"red",strlen("red"))==0){
          background_color = LCD_COLOR_RED;
        } else if(strncmp(pcValue[i],"green",strlen("green"))==0){
          background_color = LCD_COLOR_GREEN;
        } else if(strncmp(pcValue[i],"magenta",strlen("magenta"))==0){
          background_color = LCD_COLOR_MAGENTA;
        } else if(strncmp(pcValue[i],"blue",strlen("blue"))==0){
          background_color = LCD_COLOR_BLUE;
        }


        /*re-rendering frame to display color*/
        renderFrame();
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
  return 0;
}
#endif




//code for touch
#ifdef BOARD_ESP32

#define TOUCH_BUTTON_NUM 6
#define PAD_DEINIT 404


static QueueHandle_t que_touch = NULL;
typedef struct touch_msg {
    touch_pad_intr_mask_t intr_mask;
    uint32_t pad_num;
    uint32_t pad_status;
    uint32_t pad_val;
} touch_event_t;

static const touch_pad_t button[TOUCH_BUTTON_NUM] = {
    TOUCH_BUTTON_PHOTO,      /*!< 'PHOTO' button */
    TOUCH_BUTTON_PLAY,       /*!< 'PLAY/PAUSE' button */
    TOUCH_BUTTON_NETWORK,    /*!< 'NETWORK' button */
    TOUCH_BUTTON_RECORD,     /*!< 'RECORD' button */
    TOUCH_BUTTON_VOLUP,      /*!< 'VOL_UP' button */
    TOUCH_BUTTON_VOLDOWN,    /*!< 'VOL_DOWN' button */
    TOUCH_BUTTON_GUARD,      /*!< Guard ring for waterproof design. */
    /*!< If this pad be touched, other pads no response. */
};

/*!<
 * Touch threshold. The threshold determines the sensitivity of the touch.
 * This threshold is derived by testing changes in readings from different touch channels.
 * If (raw_data - baseline) > baseline * threshold, the pad be activated.
 * If (raw_data - baseline) < baseline * threshold, the pad be inactivated.
 */
static const float button_threshold[TOUCH_BUTTON_NUM] = {
    0.01,     /*!< threshold = 1% */
    0.01,
    0.01,
    0.01,
    0.01,
    0.01,
    0.01,
};

static void tp_example_set_thresholds(void)
{
    uint32_t touch_value;
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++) {
        /*!< read benchmark value */
        touch_pad_read_benchmark(button[i], &touch_value);
        /*!< set interrupt threshold. */
        touch_pad_set_thresh(button[i], touch_value * button_threshold[i]);
        ESP_LOGI(TAG, "touch pad [%d] base %d, thresh %d", \
                 button[i], touch_value, (uint32_t)(touch_value * button_threshold[i]));
    }
}

esp_err_t example_touch_init(void)
{

    if (que_touch == NULL) {
        que_touch = xQueueCreate(TOUCH_BUTTON_NUM, sizeof(touch_event_t));
    }

    /*!< Initialize touch pad peripheral, it will start a timer to run a filter */
    ESP_LOGI(TAG, "Initializing touch pad");
    /*!< Initialize touch pad peripheral. */
    touch_pad_init();

    for (int i = 0; i < TOUCH_BUTTON_NUM; i++) {
        touch_pad_config(button[i]);
    }

#if TOUCH_CHANGE_CONFIG
    /*!< If you want change the touch sensor default setting, please write here(after initialize). There are examples: */
    touch_pad_set_meas_time(TOUCH_PAD_SLEEP_CYCLE_DEFAULT, TOUCH_PAD_SLEEP_CYCLE_DEFAULT);
    touch_pad_set_voltage(TOUCH_PAD_HIGH_VOLTAGE_THRESHOLD, TOUCH_PAD_LOW_VOLTAGE_THRESHOLD, TOUCH_PAD_ATTEN_VOLTAGE_THRESHOLD);
    touch_pad_set_idle_channel_connect(TOUCH_PAD_IDLE_CH_CONNECT_DEFAULT);
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++) {
        touch_pad_set_cnt_mode(i, TOUCH_PAD_SLOPE_DEFAULT, TOUCH_PAD_TIE_OPT_DEFAULT);
    }
#endif

#if TOUCH_BUTTON_DENOISE_ENABLE
    /*!< Denoise setting at TouchSensor 0. */
    touch_pad_denoise_t denoise = {
        /*!< The bits to be cancelled are determined according to the noise level. */
        .grade     = TOUCH_PAD_DENOISE_BIT4,
        .cap_level = TOUCH_PAD_DENOISE_CAP_L4,
    };
    touch_pad_denoise_set_config(&denoise);
    touch_pad_denoise_enable();
    ESP_LOGI(TAG, "Denoise function init");
#endif

#if TOUCH_BUTTON_WATERPROOF_ENABLE
    /*!< Waterproof function */
    touch_pad_waterproof_t waterproof = {
        .guard_ring_pad = TOUCH_BUTTON_GUARD,       /*!< If no ring pad, set 0; */
        /*!< It depends on the number of the parasitic capacitance of the shield pad. */
        .shield_driver  = TOUCH_PAD_SHIELD_DRV_L2,  /*!< 40pf */
    };
    touch_pad_waterproof_set_config(&waterproof);
    touch_pad_waterproof_enable();
    ESP_LOGI(TAG, "touch pad waterproof init");
#endif

    /*!< Filter setting */
    //touchsensor_filter_set(TOUCH_PAD_FILTER_IIR_16);
    //touch_pad_timeout_set(true, SOC_TOUCH_PAD_THRESHOLD_MAX);
    /*!< Register touch interrupt ISR, enable intr type. */
    //touch_pad_isr_register(touchsensor_interrupt_cb, NULL, TOUCH_PAD_INTR_MASK_ALL);
    //touch_pad_intr_enable(TOUCH_PAD_INTR_MASK_ACTIVE | TOUCH_PAD_INTR_MASK_INACTIVE | TOUCH_PAD_INTR_MASK_TIMEOUT);

    /*!< Enable touch sensor clock. Work mode is "timer trigger". */
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_fsm_start();

    return ESP_OK;
}



void touch_pad_read_task(void *pvParameter)
{
    touch_event_t evt = {0};

    /*!< Wait touch sensor init done */
    vTaskDelay(100 / portTICK_RATE_MS);
    tp_example_set_thresholds();

    while (1) {
        int ret = xQueueReceive(que_touch, &evt, (portTickType)portMAX_DELAY);
        if (evt.pad_num == PAD_DEINIT)
        {
            break;
        }

        if (ret != pdTRUE || (evt.intr_mask & TOUCH_PAD_INTR_MASK_ACTIVE) == false) {
            continue;
        }

        /*!< if guard pad be touched, other pads no response. */
        switch (evt.pad_num) {
            case TOUCH_BUTTON_PHOTO:
                ESP_LOGI(TAG, "photo    -> set the red light");
                break;

            case TOUCH_BUTTON_PLAY:
                ESP_LOGI(TAG, "play     -> set the green light");
                break;

            case TOUCH_BUTTON_NETWORK:
                ESP_LOGI(TAG, "network  -> set the blue light");
                break;

            case TOUCH_BUTTON_RECORD:
                ESP_LOGI(TAG, "record   -> shut down the light");
                break;

            case TOUCH_BUTTON_VOLUP:
                
                    

                ESP_LOGI(TAG, "vol_up   -> make the light brighter:");
                    
                

                break;

            case TOUCH_BUTTON_VOLDOWN:
                

                ESP_LOGI(TAG, "vol_down -> make the light darker:");
                    

                break;

            default:
                ESP_LOGI(TAG, "ERROR\n");
                break;
        }


    }
    ESP_LOGI(TAG, "touch_pad_read_task:exit the task\n");
    //example_touch_deinit();
    vTaskDelete(NULL);
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

    /*!< Initialize the touch pad */
    #ifdef BOARD_ESP32
    ESP_LOGI(TAG,"render ok");
    ESP_ERROR_CHECK(example_touch_init());

    ESP_LOGI(TAG,"start touch task");
    /*!< Start a task to show what pads have been touched */
    xTaskCreate(&touch_pad_read_task, "touch_pad_read_task", 2048, NULL, 5, NULL);
    ESP_LOGI(TAG,"TOUCH START ok");
    #endif
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
