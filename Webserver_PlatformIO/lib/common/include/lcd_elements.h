#ifndef LCD_ELEMENTS_H_
#define LCD_ELEMENTS_H_


#ifdef BOARD_STM32
#include "stm32746g_discovery_lcd.h"
#endif

#ifdef BOARD_STM32
#include "platformio_logo.h"
#endif

#include <stdint.h>

#ifdef BOARD_STM32
#define SCREEN_XSIZE 480
#define SCREEN_YSIZE 272
#define X_CONV(x) x
#define Y_CONV(y) y
#endif

#ifdef BOARD_ESP32
#define SCREEN_XSIZE 320
#define SCREEN_YSIZE 240
#define X_CONV(x) (x*2)/3
#define Y_CONV(y) (y*15)/17
#endif




/*default background color*/
#define BACKGROUND_COLOR LCD_COLOR_WHITE
/*Default values for Menu button*/
#define MENU_BUTTON_LOCATION_X X_CONV(10)
#define MENU_BUTTON_LOCATION_Y Y_CONV(10)
#define MENU_BUTTON_HEIGHT Y_CONV(35)
#define MENU_BUTTON_WIDTH X_CONV(35)
#define MENU_BUTTON_AMOUNT_STRIPES 3
#define MENU_BUTTON_STRIPE_WIDTH MENU_BUTTON_HEIGHT/(MENU_BUTTON_AMOUNT_STRIPES*2-1)
#define MENU_BUTTON_COLOR LCD_COLOR_BLACK
#define MENU_BUTTON_IS_ACTIVE 1 //0
/*by default is displayed*/
#define MENU_BUTTON_ISDISPLAYED 1

/*Default values for IP button*/
#define IP_BUTTON_HEIGHT Y_CONV(35)
#define IP_BUTTON_WIDTH X_CONV(35)
#define IP_BUTTON_LOCATION_X SCREEN_XSIZE - IP_BUTTON_WIDTH - X_CONV(10)
#define IP_BUTTON_LOCATION_Y Y_CONV(10)
#define IP_BUTTON_COLOR LCD_COLOR_BLACK
#ifdef BOARD_STM32
#define IP_BUTTON_FONT Font20
#endif
#ifdef BOARD_ESP32
#define IP_BUTTON_FONT Font16
#endif
#define IP_BUTTON_PADDING_Y Y_CONV(10)
#define IP_BUTTON_PADDING_X X_CONV(4)
/*by default is displayed*/
#define IP_BUTTON_ISDISPLAYED 1

/*Default values for IP Address*/
#define IP_ADDRESS_HEIGHT Y_CONV(35)
#define IP_ADDRESS_WIDTH X_CONV(150)
#define IP_ADDRESS_LOCATION_X X_CONV(140)
#define IP_ADDRESS_LOCATION_Y Y_CONV(10)
#define IP_ADDRESS_COLOR LCD_COLOR_BLACK
#ifdef BOARD_STM32
#define IP_ADDRESS_FONT Font16
#endif
#ifdef BOARD_ESP32
#define IP_ADDRESS_FONT Font12
#endif
#define IP_ADDRESS_TEXT "IP address: %d.%d.%d.%d"
/*by default is not displayed*/
#define IP_ADDRESS_ISDISPLAYED 0

/*Default values for Menu*/
#define MENU_LOCATION_X 1
#define MENU_LOCATION_Y 1
#define MENU_AMOUNT_ITEMS 5 /* Home - Options - windows vs linux - info build - chat*/

#define MENU_ITEM_HEIGHT Y_CONV(30)
#define MENU_HEIGHT MENU_AMOUNT_ITEMS*MENU_ITEM_HEIGHT + MENU_BUTTON_LOCATION_Y + MENU_BUTTON_HEIGHT + Y_CONV(10)

#define MENU_COLOR LCD_COLOR_BLACK
#ifdef BOARD_STM32
#define MENU_FONT Font12
#endif
#ifdef BOARD_ESP32
#define MENU_FONT Font8
#endif
#define MENU_WIDTH X_CONV(135)
/*by default isn't displayed*/
#define MENU_ISDISPLAYED 1 //0

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
#define MENU_ITEMS_TOP_PADDING Y_CONV(20)
#define MENU_ITEM_LOCATION_X X_CONV(10)
#define MENU_ITEM_LOCATION_Y(itemnumber) MENU_BUTTON_LOCATION_Y + MENU_BUTTON_HEIGHT + MENU_ITEMS_TOP_PADDING + (itemnumber-1)*MENU_ITEM_HEIGHT

/*Values for platformio logo*/
//#define PIO_LOGO_DATA PLATFORMIO_LOGO_DATA

#ifdef BOARD_STM32
#define PIO_LOGO_LOCATION_X 160
#define PIO_LOGO_LOCATION_Y 40
#define PIO_LOGO_FORMAT PLATFORMIO_LOGO_DATA_FORMAT
#define PIO_LOGO_COLOR LCD_COLOR_WHITE /*Not used*/
#define PIO_LOGO_WIDTH PLATFORMIO_LOGO_DATA_X_PIXEL
#define PIO_LOGO_HEIGHT PLATFORMIO_LOGO_DATA_Y_PIXEL
#define PIO_LOGO_DATA PLATFORMIO_LOGO_DATA
#define PIO_LOGO_ISDISPLAYED 1
#endif

#ifdef BOARD_ESP32
#define PIO_LOGO_LOCATION_X MENU_WIDTH + 10
#define PIO_LOGO_LOCATION_Y Y_CONV(40) + 10
//#define PIO_LOGO_FORMAT PLATFORMIO_LOGO_DATA_FORMAT
#define PIO_LOGO_COLOR LCD_COLOR_WHITE /*Not used*/
#define PIO_LOGO_WIDTH PLATFORMIO_LOGO_ESP_DATA_X_PIXEL
#define PIO_LOGO_HEIGHT PLATFORMIO_LOGO_ESP_DATA_Y_PIXEL
#define PIO_LOGO_DATA PLATFORMIO_LOGO_ESP_DATA
#define PIO_LOGO_ISDISPLAYED 1
#endif

/*Values for background colors*/
#define AMOUNT_BACKGROUND_COLORS 20
#define BACKGROUND_COLORS_PER_ROW 5
#define BACKGROUND_COLOR_AMOUNT_ROWS AMOUNT_BACKGROUND_COLORS/BACKGROUND_COLORS_PER_ROW

/*Values for background color options*/
#define BG_OPTIONS_LOCATION_X X_CONV(160)
#define BG_OPTIONS_LOCATION_Y Y_CONV(100)
#define BG_OPTIONS_COLOR LCD_COLOR_BLACK
#define BG_OPTIONS_ELEMENT_WIDTH X_CONV(35)
#define BG_OPTIONS_ELEMENT_HEIGHT Y_CONV(35)
#define BG_OPTIONS_HEIGHT BG_OPTIONS_ELEMENT_WIDTH*BACKGROUND_COLOR_AMOUNT_ROWS
#define BG_OPTIONS_WIDTH BG_OPTIONS_ELEMENT_HEIGHT*BACKGROUND_COLORS_PER_ROW
#define BG_OPTIONS_ISDISPLAYED 0

#define BG_OPTIONS_TITLE_LOCATION_X X_CONV(160)
#define BG_OPTIONS_TITLE_LOCATION_Y Y_CONV(50)
#ifdef BOARD_STM32
#define BG_OPTIONS_TITLE_FONT Font16
#endif
#ifdef BOARD_ESP32
#define BG_OPTIONS_TITLE_FONT Font12
#endif


/*Values for chat message array*/
#define MAX_AMOUNT_CHAT_MESSAGES 10
#define MAX_LENGTH_CHAT_MESSAGE 45

#define CHAT_BOX_LOCATION_X X_CONV(160)
#define CHAT_BOX_LOCATION_Y Y_CONV(50)
#define CHAT_BOX_COLOR LCD_COLOR_BLACK
#define CHAT_BOX_HEIGHT (SCREEN_YSIZE - CHAT_BOX_LOCATION_Y)
#define CHAT_BOX_WIDTH SCREEN_XSIZE - CHAT_BOX_LOCATION_X
#define CHAT_BOX_MESSAGE_HEIGHT CHAT_BOX_HEIGHT / MAX_AMOUNT_CHAT_MESSAGES
#ifdef BOARD_STM32
#define CHAT_BOX_FONT Font12
#endif
#ifdef BOARD_ESP32
#define CHAT_BOX_FONT Font12
#endif
#define CHAT_BOX_ISDISPLAYED 0

#define COMP_TABLE_HEADER_LOC_Y Y_CONV(50)

#define COMPILATION_TABLE_LOCATION_X X_CONV(160)
#define COMPILATION_TABLE_LOCATION_Y Y_CONV(80)
#define COMPILATION_TABLE_HEIGHT (SCREEN_YSIZE - COMPILATION_TABLE_LOCATION_Y - 50)
#define COMPILATION_TABLE_WIDTH (SCREEN_XSIZE - COMPILATION_TABLE_LOCATION_X)
#ifdef BOARD_STM32
#define COMPILATION_TABLE_FONT Font12
#define COMPILATION_TABLE_TITLE_FONT Font16
#endif
#ifdef BOARD_ESP32
#define COMPILATION_TABLE_FONT Font8
#define COMPILATION_TABLE_TITLE_FONT Font12
#endif
#define COMPILATION_TABLE_COLOR LCD_COLOR_BLACK
#define COMPILATION_TABLE_MAX_MSG_LENGTH 15
#define COMPILATION_TABLE_ISDISPLAYED 0

#define ESP_TABLE 2
#define STM_TABLE 1

#define COMPILATION_TABLE_AMOUNT_COLUMNS 3
#define COMPILATION_TABLE_AMOUNT_ROWS 6 + 1

#define COMPILATION_TABLE_TITLE_HEIGHT X_CONV(20)
#define COMPILATION_TABLE_TITLE_WIDTH COMPILATION_TABLE_WIDTH

#define COMPILATION_TABLE_CELL_HEIGHT COMPILATION_TABLE_HEIGHT / COMPILATION_TABLE_AMOUNT_ROWS
#define COMPILATION_TABLE_CELL_WIDTH COMPILATION_TABLE_WIDTH / COMPILATION_TABLE_AMOUNT_COLUMNS




#define BUILD_INFO_LOCATION_X X_CONV(160)
#define BUILD_INFO_LOCATION_Y Y_CONV(60)
#ifdef BOARD_STM32
#define BUILD_INFO_FONT Font16
#endif
#ifdef BOARD_ESP32
#define BUILD_INFO_FONT Font12
#endif
#define BUILD_INFO_COLOR LCD_COLOR_BLACK

#define BUILD_INFO_ISDISPLAYED 0


/* USER CODE BEGIN EFP */
typedef struct LCD_ELEMENT{
  /*Location is top left corner of element*/
  int locationX;
  int locationY;
  int height;
  int width;
  uint32_t color;
  int isDisplayed;
} LCD_ELEMENT;

typedef struct MENU_BUTTON{
  LCD_ELEMENT base_element;  
  int amountStripes;
  int stripeWidth;
  int isActive;
} MENU_BUTTON;

typedef struct IP_BUTTON{
  LCD_ELEMENT base_element;
  int paddingX;
  int paddingY;
} IP_BUTTON;

typedef struct MENU{
  LCD_ELEMENT base_element;
  int amountElements;
  int elementHeight;
  LCD_ELEMENT elementHome;
  LCD_ELEMENT elementOptions;
  LCD_ELEMENT elementBuildinfo;
  LCD_ELEMENT elementChat;
  LCD_ELEMENT elementDifferences;
} MENU;

typedef struct PICTURE{
  LCD_ELEMENT base_element;
  uint32_t format;
  unsigned short* data;
} PICTURE;

typedef struct BG_COLORS{
  LCD_ELEMENT base_element;
  int colorsPerRow;
  int amountRows;
  int elementWidth;
  int elementHeight;
}BG_COLORS;

typedef struct CHAT_BOX{
  LCD_ELEMENT base_element;
  int amountmessages;
  int messageHeight;
}CHAT_BOX;

typedef struct COMPILATION_CHART{
  LCD_ELEMENT base_element;
  int selectedTable;
  int amountColumns;
  int amountRows;
  int titleHeight;
  int titleWidth;
} COMPILATION_CHART;


void handleTouch(uint16_t , uint16_t);
void clearElements(void);
void goToPage(int page);
int touchInBoundary(uint16_t, uint16_t, LCD_ELEMENT);
int touchInBoundaryCoords(uint16_t, uint16_t, int, int, int, int);


#endif