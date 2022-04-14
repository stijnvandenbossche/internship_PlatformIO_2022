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
#include "crc.h"
#include "dma2d.h"
#include "fatfs.h"
#include "i2c.h"
#include "ltdc.h"
#include "lwip.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "init.h"
#include "httpd.h"
#include <errno.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/times.h>
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_lcd.h"

#include "platformio_logo.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */




/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/*SSI values*/
#define AMOUNT_SSI_TAGS 3





/*default background color*/
#define BACKGROUND_COLOR LCD_COLOR_WHITE
/*Default values for Menu button*/
#define MENU_BUTTON_LOCATION_X 10
#define MENU_BUTTON_LOCATION_Y 10
#define MENU_BUTTON_HEIGHT 35
#define MENU_BUTTON_WIDTH 35
#define MENU_BUTTON_AMOUNT_STRIPES 3
#define MENU_BUTTON_STRIPE_WIDTH MENU_BUTTON_HEIGHT/(MENU_BUTTON_AMOUNT_STRIPES*2-1)
#define MENU_BUTTON_COLOR LCD_COLOR_BLACK
#define MENU_BUTTON_IS_ACTIVE 0
/*by default is displayed*/
#define MENU_BUTTON_ISDISPLAYED 1

/*Default values for IP button*/
#define IP_BUTTON_HEIGHT 35
#define IP_BUTTON_WIDTH 35
#define IP_BUTTON_LOCATION_X 480 - IP_BUTTON_WIDTH - 10
#define IP_BUTTON_LOCATION_Y 10
#define IP_BUTTON_COLOR LCD_COLOR_BLACK
#define IP_BUTTON_FONT Font20
#define IP_BUTTON_PADDING_Y 10
#define IP_BUTTON_PADDING_X 4
/*by default is displayed*/
#define IP_BUTTON_ISDISPLAYED 1

/*Default values for IP Address*/
#define IP_ADDRESS_HEIGHT 35
#define IP_ADDRESS_WIDTH 150
#define IP_ADDRESS_LOCATION_X 140
#define IP_ADDRESS_LOCATION_Y 10
#define IP_ADDRESS_COLOR LCD_COLOR_BLACK
#define IP_ADDRESS_FONT Font16
#define IP_ADDRESS_TEXT "IP address: %d.%d.%d.%d"
/*by default is not displayed*/
#define IP_ADDRESS_ISDISPLAYED 0

/*Default values for Menu*/
#define MENU_LOCATION_X 1
#define MENU_LOCATION_Y 1
#define MENU_AMOUNT_ITEMS 5 /* Home - Options - windows vs linux - info build - chat*/

#define MENU_ITEM_HEIGHT 30
#define MENU_HEIGHT MENU_AMOUNT_ITEMS*MENU_ITEM_HEIGHT + MENU_BUTTON_LOCATION_Y + MENU_BUTTON_HEIGHT + 10

#define MENU_COLOR LCD_COLOR_BLACK
#define MENU_FONT Font12
#define MENU_WIDTH 135
/*by default isn't displayed*/
#define MENU_ISDISPLAYED 0

/*Default values for menu items*/
#define MENU_ITEM_HOME 1
#define MENU_ITEM_OPTIONS 2 
#define MENU_ITEM_BUILDINFO 3 
#define MENU_ITEM_DIFFERENCES 4 
#define MENU_ITEM_CHAT 5

/*Page values*/
#define PAGE_HOME MENU_ITEM_HOME
#define PAGE_OPTIONS MENU_ITEM_OPTIONS 
#define PAGE_BUILDINFO MENU_ITEM_BUILDINFO
#define PAGE_DIFFERENCES MENU_ITEM_DIFFERENCES
#define PAGE_CHAT MENU_ITEM_CHAT

/*Menu item values*/
#define MENU_ITEMS_TOP_PADDING 20
#define MENU_ITEM_LOCATION_X 10
#define MENU_ITEM_LOCATION_Y(itemnumber) MENU_BUTTON_LOCATION_Y + MENU_BUTTON_HEIGHT + MENU_ITEMS_TOP_PADDING + (itemnumber-1)*MENU_ITEM_HEIGHT

/*Values for platformio logo*/
//#define PIO_LOGO_DATA PLATFORMIO_LOGO_DATA
#define PIO_LOGO_LOCATION_X 160
#define PIO_LOGO_LOCATION_Y 40
#define PIO_LOGO_FORMAT PLATFORMIO_LOGO_DATA_FORMAT
#define PIO_LOGO_COLOR LCD_COLOR_WHITE /*Not used*/
#define PIO_LOGO_WIDTH PLATFORMIO_LOGO_DATA_X_PIXEL
#define PIO_LOGO_HEIGHT PLATFORMIO_LOGO_DATA_Y_PIXEL
#define PIO_LOGO_ISDISPLAYED 1

/*Values for background colors*/
#define AMOUNT_BACKGROUND_COLORS 20
#define BACKGROUND_COLORS_PER_ROW 5
#define BACKGROUND_COLOR_AMOUNT_ROWS AMOUNT_BACKGROUND_COLORS/BACKGROUND_COLORS_PER_ROW

/*Values for background color options*/
#define BG_OPTIONS_LOCATION_X 160
#define BG_OPTIONS_LOCATION_Y 50
#define BG_OPTIONS_COLOR LCD_COLOR_BLACK
#define BG_OPTIONS_ELEMENT_WIDTH 35
#define BG_OPTIONS_ELEMENT_HEIGHT 35
#define BG_OPTIONS_HEIGHT BG_OPTIONS_ELEMENT_WIDTH*BACKGROUND_COLOR_AMOUNT_ROWS
#define BG_OPTIONS_WIDTH BG_OPTIONS_ELEMENT_HEIGHT*BACKGROUND_COLORS_PER_ROW
#define BG_OPTIONS_ISDISPLAYED 0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
CRC_HandleTypeDef hcrc;
DMA2D_HandleTypeDef hdma2d;
I2C_HandleTypeDef hi2c3;
LTDC_HandleTypeDef hltdc;
SD_HandleTypeDef hsd1;
HAL_SD_CardInfoTypeDef SDCardInfo1;
DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
SDRAM_HandleTypeDef hsdram1;

UART_HandleTypeDef huart1;
int cnt = 0;

/*SSI TAGS*/
char* ssiTags[AMOUNT_SSI_TAGS]={"DATE","TIME","LWIPVERS"};



/*LCD elements*/

/*background color*/
uint32_t background_color = BACKGROUND_COLOR;

/*array with available background colors*/
uint32_t bgcolors_array[AMOUNT_BACKGROUND_COLORS]={
  LCD_COLOR_DARKRED     , LCD_COLOR_RED         , LCD_COLOR_LIGHTRED    , LCD_COLOR_ORANGE      , LCD_COLOR_DARKYELLOW  , 
  LCD_COLOR_YELLOW      , LCD_COLOR_LIGHTYELLOW , LCD_COLOR_LIGHTGREEN  , LCD_COLOR_GREEN       , LCD_COLOR_DARKGREEN   , 
  LCD_COLOR_DARKCYAN    , LCD_COLOR_CYAN        , LCD_COLOR_LIGHTBLUE   , LCD_COLOR_BLUE        , LCD_COLOR_DARKBLUE    , 
  LCD_COLOR_DARKMAGENTA , LCD_COLOR_MAGENTA     , LCD_COLOR_LIGHTMAGENTA, LCD_COLOR_LIGHTGRAY   , LCD_COLOR_WHITE 
};

/*initialize menu_button*/
MENU_BUTTON menu_button = {.base_element = {.locationX = MENU_BUTTON_LOCATION_X, .locationY = MENU_BUTTON_LOCATION_Y, .height=MENU_BUTTON_HEIGHT, .width = MENU_BUTTON_WIDTH, .color = MENU_BUTTON_COLOR, .isDisplayed = MENU_BUTTON_ISDISPLAYED}, .amountStripes = MENU_BUTTON_AMOUNT_STRIPES, .stripeWidth = MENU_BUTTON_STRIPE_WIDTH, .isActive = MENU_BUTTON_IS_ACTIVE };

/*initialize IP_button*/
IP_BUTTON ip_button = {.base_element = {.locationX = IP_BUTTON_LOCATION_X, .locationY = IP_BUTTON_LOCATION_Y, .height=IP_BUTTON_HEIGHT, .width = IP_BUTTON_WIDTH, .color = IP_BUTTON_COLOR, .isDisplayed = IP_BUTTON_ISDISPLAYED}, .paddingX = IP_BUTTON_PADDING_X, .paddingY = IP_BUTTON_PADDING_Y};

/*initialize ip_address*/
LCD_ELEMENT lcd_element_ip_address = {.locationX = IP_ADDRESS_LOCATION_X, .locationY = IP_ADDRESS_LOCATION_Y, .height=IP_ADDRESS_HEIGHT, .width = IP_ADDRESS_WIDTH, .color = IP_ADDRESS_COLOR, .isDisplayed = IP_ADDRESS_ISDISPLAYED};

/*initialize menu*/
MENU menu = {
  .base_element = {.locationX = MENU_LOCATION_X, .locationY = MENU_LOCATION_Y, .height=MENU_HEIGHT, .width = MENU_WIDTH, .color = MENU_COLOR, .isDisplayed = MENU_ISDISPLAYED},
  .amountElements = MENU_AMOUNT_ITEMS,
  .elementHeight = MENU_ITEM_HEIGHT,
  .elementHome = {.locationX = MENU_ITEM_LOCATION_X, .locationY = MENU_ITEM_LOCATION_Y(MENU_ITEM_HOME), .height=MENU_ITEM_HEIGHT, .width = MENU_WIDTH, .color = MENU_COLOR, .isDisplayed = MENU_ISDISPLAYED},
  .elementOptions = {.locationX = MENU_ITEM_LOCATION_X, .locationY = MENU_ITEM_LOCATION_Y(MENU_ITEM_OPTIONS), .height=MENU_ITEM_HEIGHT, .width = MENU_WIDTH, .color = MENU_COLOR, .isDisplayed = MENU_ISDISPLAYED},
  .elementBuildinfo = {.locationX = MENU_ITEM_LOCATION_X, .locationY = MENU_ITEM_LOCATION_Y(MENU_ITEM_BUILDINFO), .height=MENU_ITEM_HEIGHT, .width = MENU_WIDTH, .color = MENU_COLOR, .isDisplayed = MENU_ISDISPLAYED},
  .elementDifferences = {.locationX = MENU_ITEM_LOCATION_X, .locationY = MENU_ITEM_LOCATION_Y(MENU_ITEM_DIFFERENCES), .height=MENU_ITEM_HEIGHT, .width = MENU_WIDTH, .color = MENU_COLOR, .isDisplayed = MENU_ISDISPLAYED},
  .elementChat = {.locationX = MENU_ITEM_LOCATION_X, .locationY = MENU_ITEM_LOCATION_Y(MENU_ITEM_CHAT), .height=MENU_ITEM_HEIGHT, .width = MENU_WIDTH, .color = MENU_COLOR, .isDisplayed = MENU_ISDISPLAYED},
  };

/*initialize pio_logo*/

PICTURE pio_logo = {{.locationX = PIO_LOGO_LOCATION_X, .locationY = PIO_LOGO_LOCATION_Y, .height= PIO_LOGO_HEIGHT, .width = PIO_LOGO_WIDTH, .color = PIO_LOGO_COLOR, .isDisplayed = PIO_LOGO_ISDISPLAYED},.format = PIO_LOGO_FORMAT, .data = PLATFORMIO_LOGO_DATA};

/*initialize bg_colors drawing*/

BG_COLORS bg_colors = {
                        {.locationX = BG_OPTIONS_LOCATION_X, .locationY = BG_OPTIONS_LOCATION_Y, .height= BG_OPTIONS_HEIGHT, .width = BG_OPTIONS_WIDTH, .color = BG_OPTIONS_COLOR, .isDisplayed = BG_OPTIONS_ISDISPLAYED}, 
                        .amountRows = BACKGROUND_COLOR_AMOUNT_ROWS, .colorsPerRow = BACKGROUND_COLORS_PER_ROW, .elementHeight = BG_OPTIONS_ELEMENT_HEIGHT, .elementWidth = BG_OPTIONS_ELEMENT_WIDTH
                      };



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*printing over USART*/
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


/*checking if a touch is within boundaries of an element*/
int touchInBoundary(uint16_t touchX, uint16_t touchY, LCD_ELEMENT base_element){
  return ( base_element.locationX <= touchX && touchX <= base_element.locationX + base_element.width && base_element.locationY <= touchY && touchY <= base_element.locationY + base_element.height );
}

int touchInBoundaryCoords(uint16_t touchX, uint16_t touchY, int posX, int posY, int width, int height){
  return (posX <= touchX && touchX <= posX + width && posY <= touchY && touchY <= posY + height);
}

void handleTouch(uint16_t x, uint16_t y){
  /*Checking if touch is on MENU BUTTON*/
  if(menu_button.base_element.isDisplayed){
    if(touchInBoundary(x,y,menu_button.base_element)){
      if(!menu_button.isActive){

        /*display menu + menu elements*/
        menu.base_element.isDisplayed = 1;
        menu.elementHome.isDisplayed = 1;
        menu.elementOptions.isDisplayed = 1;
        menu.elementBuildinfo.isDisplayed = 1;
        menu.elementDifferences.isDisplayed = 1;
        menu.elementChat.isDisplayed = 1;
        /*change visual of menu button*/
        menu_button.isActive = 1;

      }else if(menu_button.isActive){
        /*display menu + menu elements*/
        menu.base_element.isDisplayed = 0;
        menu.elementHome.isDisplayed = 0;
        menu.elementOptions.isDisplayed = 0;
        menu.elementBuildinfo.isDisplayed = 0;
        menu.elementDifferences.isDisplayed = 0;
        menu.elementChat.isDisplayed = 0;
        /*change visual of menu button*/
        menu_button.isActive = 0;
      }

      printf("DRAW MENU\r\n");
    }
  }

  /*Checking if touch is on IP Button*/
  if(ip_button.base_element.isDisplayed){
    if(touchInBoundary(x,y,ip_button.base_element)){

      if(lcd_element_ip_address.isDisplayed){
        lcd_element_ip_address.isDisplayed=0;
      }else if(!lcd_element_ip_address.isDisplayed){
        lcd_element_ip_address.isDisplayed=1;
      }
      printf("Display IP\r\n");
    }
  }


  /*Checking menu items*/
  if(menu.base_element.isDisplayed){
    /*element 1 - Home*/
    if(menu.elementHome.isDisplayed){
      if(touchInBoundary(x,y,menu.elementHome)){
        goToPage(PAGE_HOME);
        printf("Home pressed\r\n");
      }
    }

    /*element 2 - Options*/
    if(menu.elementOptions.isDisplayed){
      if(touchInBoundary(x,y,menu.elementOptions)){
        goToPage(PAGE_OPTIONS);
        printf("Options pressed\r\n");
      }
    }

    /*element 3 - Build info*/
    if(menu.elementBuildinfo.isDisplayed){
      if(touchInBoundary(x,y,menu.elementBuildinfo)){
        goToPage(PAGE_BUILDINFO);
        printf("Build Info pressed\r\n");
      }
    }
  
    /*element 4 - windows vs linux*/
    if(menu.elementDifferences.isDisplayed){
      if(touchInBoundary(x,y,menu.elementDifferences)){
        goToPage(PAGE_DIFFERENCES);
        printf("windows vs linux pressed\r\n");
      }
    }  

    /*element 5 - chat*/
    if(menu.elementChat.isDisplayed){
      if(touchInBoundary(x,y,menu.elementChat)){
        goToPage(PAGE_CHAT);
        printf("Chat pressed\r\n");
      }
    }
  }

  /*Checking BG color option items*/
  if(bg_colors.base_element.isDisplayed){
    if(touchInBoundary(x,y,bg_colors.base_element)){
      for(int i=0; i<(bg_colors.amountRows)*(bg_colors.colorsPerRow);i++){
        if(touchInBoundaryCoords(
                                x,
                                y,
                                bg_colors.base_element.locationX + (i % bg_colors.colorsPerRow)*bg_colors.elementWidth,
                                bg_colors.base_element.locationY + (i / bg_colors.colorsPerRow)*bg_colors.elementHeight,
                                bg_colors.elementWidth,
                                bg_colors.elementHeight
                                ))
        {
          printf("Color pressed: #%d\r\n",i);
          background_color = bgcolors_array[i];
          break;
        }
      }
    }
  }

  renderFrame();
  return;
}


/*Clearing all elements displayed, then re-enabling all common ones over all pages*/
void clearElements(){
  /*Clearing all*/
  menu_button.base_element.isDisplayed=0;
  ip_button.base_element.isDisplayed = 0;
  lcd_element_ip_address.isDisplayed = 0;
  pio_logo.base_element.isDisplayed = 0;
  bg_colors.base_element.isDisplayed = 0;

  /*Re-enabling common ones*/
  ip_button.base_element.isDisplayed = 1;
  menu_button.base_element.isDisplayed = 1;
}

/*Navigating to another 'page' and drawing all necessary components for that page*/
void goToPage(int page){
  clearElements();

  switch (page){
    case PAGE_HOME:
      pio_logo.base_element.isDisplayed = 1;
      break;
    case PAGE_OPTIONS:
      bg_colors.base_element.isDisplayed = 1;
      break;
    case PAGE_BUILDINFO:

      break;
    case PAGE_DIFFERENCES:

      break;
    case PAGE_CHAT:

      break;
  }
}

/*Checking all elements if they need to be displayed*/
void renderFrame(){
  /*clear background*/
  BSP_LCD_SelectLayer(0);
  BSP_LCD_Clear(background_color);

  /*clear foreground*/
  BSP_LCD_SelectLayer(1);
  BSP_LCD_Clear(background_color);

  /*Menu button*/
  if(menu_button.base_element.isDisplayed){
    drawMenuButton();
  }

  /*IP button*/
  if(ip_button.base_element.isDisplayed){
    drawIPButton();
  }

  /*IP Address*/
  if(lcd_element_ip_address.isDisplayed){
    drawIPAddress();
  }

  if(menu.base_element.isDisplayed){
    drawMenu();
  }

  /*pio logo*/
  if(pio_logo.base_element.isDisplayed){
    drawPIOLogo();
  }

  /*BG color block options*/
  if(bg_colors.base_element.isDisplayed){
    drawBgColorOptions();
  }

  return;
}


/*Functions to render elements*/

void drawMenuButton(){
  BSP_LCD_SelectLayer(1);
  BSP_LCD_SetTextColor(menu_button.base_element.color);
  for(int i=0;i<menu_button.amountStripes;i++){
    //Checking if button is active. If it is, draw hollow rectangles, otherwise draw filled ones
    if(!menu_button.isActive){
      BSP_LCD_FillRect(menu_button.base_element.locationX, menu_button.base_element.locationY + (menu_button.stripeWidth*2*i) , menu_button.base_element.width , menu_button.stripeWidth );
    }else if(menu_button.isActive){
      BSP_LCD_DrawRect(menu_button.base_element.locationX, menu_button.base_element.locationY + (menu_button.stripeWidth*2*i) , menu_button.base_element.width , menu_button.stripeWidth );
    }
  }
  
  return;
}

void drawBgColorOptions(){
  BSP_LCD_SelectLayer(1);

  int posX;
  int posY;
  /*Looping over all color blocks*/
  for(int i=0;i<(bg_colors.colorsPerRow)*(bg_colors.amountRows);i++){
    posX =  bg_colors.base_element.locationX + (i % bg_colors.colorsPerRow)*bg_colors.elementWidth; 
    posY =  bg_colors.base_element.locationY + (i / bg_colors.colorsPerRow)*bg_colors.elementHeight;
    /*Selecting corresponding color*/
    BSP_LCD_SetTextColor(bgcolors_array[i]);
    BSP_LCD_FillRect(posX,posY,bg_colors.elementWidth,bg_colors.elementHeight);
    /*Drawing border around*/
    BSP_LCD_SetTextColor(bg_colors.base_element.color);
    BSP_LCD_DrawRect(posX,posY,bg_colors.elementWidth,bg_colors.elementHeight);
  }
  return;
}

void drawIPButton(){
  BSP_LCD_SelectLayer(1);
  BSP_LCD_SetTextColor(ip_button.base_element.color);
  BSP_LCD_SetBackColor(background_color);

  /*drawing border*/
  BSP_LCD_DrawRect(ip_button.base_element.locationX,ip_button.base_element.locationY,ip_button.base_element.width,ip_button.base_element.height);
  /*Writing 'IP'*/
  BSP_LCD_SetFont(&IP_BUTTON_FONT);
  BSP_LCD_DisplayStringAt(ip_button.base_element.locationX + ip_button.paddingX , ip_button.base_element.locationY + ip_button.paddingY, (uint8_t*)"IP",LEFT_MODE);
  return;
}

void drawIPAddress(){
  uint32_t ip = getIpAddr();
  char* ip_addr_str;
  sprintf(ip_addr_str,IP_ADDRESS_TEXT, (uint8_t)(ip & 0x000000ff), (uint8_t)((ip & 0x0000ff00)>>8), (uint8_t) ((ip & 0x00ff0000)>>16), (uint8_t)((ip & 0xff000000)>>24));
        
  /* Displaying IP Address on LCD*/
  BSP_LCD_SelectLayer(1);
  BSP_LCD_SetTextColor(lcd_element_ip_address.color);
  BSP_LCD_SetBackColor(background_color);
  BSP_LCD_SetFont(&IP_ADDRESS_FONT);
  BSP_LCD_DisplayStringAt(lcd_element_ip_address.locationX,lcd_element_ip_address.locationY,(uint8_t*)ip_addr_str,LEFT_MODE);

  return;
}

void drawMenu(){
  BSP_LCD_SelectLayer(1);
  BSP_LCD_SetTextColor(menu.base_element.color);
  BSP_LCD_SetBackColor(background_color);

  /*Drawing border*/
  BSP_LCD_DrawRect(menu.base_element.locationX,menu.base_element.locationY,menu.base_element.width,menu.base_element.height);

  /*Drawing elements*/
  /*element 1 - Home*/
  BSP_LCD_SetTextColor(menu.elementHome.color);
  BSP_LCD_SetBackColor(background_color);
  BSP_LCD_SetFont(&MENU_FONT);
  BSP_LCD_DisplayStringAt(menu.elementHome.locationX,menu.elementHome.locationY,(uint8_t*)"Home",LEFT_MODE);

  /*element 2 - Options*/
  BSP_LCD_SetTextColor(menu.elementOptions.color);
  BSP_LCD_SetBackColor(background_color);
  BSP_LCD_SetFont(&MENU_FONT);
  BSP_LCD_DisplayStringAt(menu.elementOptions.locationX,menu.elementOptions.locationY,(uint8_t*)"Options",LEFT_MODE);

  /*element 3 - build info*/
  BSP_LCD_SetTextColor(menu.elementBuildinfo.color);
  BSP_LCD_SetBackColor(background_color);
  BSP_LCD_SetFont(&MENU_FONT);
  BSP_LCD_DisplayStringAt(menu.elementBuildinfo.locationX,menu.elementBuildinfo.locationY,(uint8_t*)"Build info",LEFT_MODE);

  /*element 4 - differences*/
  BSP_LCD_SetTextColor(menu.elementDifferences.color);
  BSP_LCD_SetBackColor(background_color);
  BSP_LCD_SetFont(&MENU_FONT);
  BSP_LCD_DisplayStringAt(menu.elementDifferences.locationX,menu.elementDifferences.locationY,(uint8_t*)"Windows vs. Linux",LEFT_MODE);

  /*element 5 - Chat*/
  BSP_LCD_SetTextColor(menu.elementChat.color);
  BSP_LCD_SetBackColor(background_color);
  BSP_LCD_SetFont(&MENU_FONT);
  BSP_LCD_DisplayStringAt(menu.elementChat.locationX,menu.elementChat.locationY,(uint8_t*)"Chat",LEFT_MODE);


  return;
}

void drawPIOLogo(){
  /*Calling custom draw function that uses DMA2D*/
  drawBitmapImage(pio_logo.data,pio_logo.base_element.locationX,pio_logo.base_element.locationY,pio_logo.base_element.width,pio_logo.base_element.height,pio_logo.format);
  //WDA_LCD_DrawBitmap(pio_logo.data,pio_logo.base_element.locationX,pio_logo.base_element.locationY,pio_logo.base_element.width,pio_logo.base_element.height,pio_logo.format);
  return;
}






/*Handlers for CGI & SSI*/

/*parameters are in pcParam, values in pcValue*/
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
}



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
  /* USER CODE BEGIN 2 */
  /*setting ssi handler*/
  http_set_ssi_handler(mySsiHandler, ssiTags,AMOUNT_SSI_TAGS);
  
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

  /*Touch Initialisations*/
  TS_StateTypeDef touchstate;
  BSP_TS_Init(480,272);



  printf("Initialised\r\n");
  
  renderFrame();
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    MX_LWIP_Process();   
    

    /*Polling touchscreen*/
    BSP_TS_GetState(&touchstate);
    if(touchstate.touchDetected>=1){
      /*Function to handle touches*/  
      handleTouch( *(touchstate.touchX) , *(touchstate.touchY) );
      HAL_Delay(100);
    }
  }
  /* USER CODE END 3 */
}

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
