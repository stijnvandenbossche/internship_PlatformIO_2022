#include "stm32_hw_functions.h"

void init_stm32(){

    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_LTDC_Init();
    MX_USART1_UART_Init();
    MX_LWIP_Init();
    MX_CRC_Init();
    MX_DMA2D_Init();
    MX_FMC_Init();
    MX_I2C3_Init();
    MX_SDMMC1_SD_Init();
    MX_FATFS_Init();



    httpd_init();


    /* Initialisation of LCD*/
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS + (BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*4));
    
    BSP_LCD_DisplayOn();

    /*Clear background and set background to white*/
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    /*Clear foreground*/
    BSP_LCD_SelectLayer(1);
    BSP_LCD_Clear(LCD_COLOR_WHITE);

}