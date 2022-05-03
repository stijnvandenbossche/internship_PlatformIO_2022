#ifndef LCD_ELEMENTS_H_
#define LCD_ELEMENTS_H_


#ifdef BOARD_STM32
#include "stm32746g_discovery_lcd.h"
#endif

#ifdef BOARD_STM32
#include "platformio_logo.h"
#endif

#include <stdint.h>


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


void handleTouch(uint16_t , uint16_t);
void renderFrame(void);
void clearElements(void);
void goToPage(int page);
int touchInBoundary(uint16_t, uint16_t, LCD_ELEMENT);
int touchInBoundaryCoords(uint16_t, uint16_t, int, int, int, int);


#endif