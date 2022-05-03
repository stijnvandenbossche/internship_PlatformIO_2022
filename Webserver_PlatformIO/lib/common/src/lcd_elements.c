#include "lcd_elements.h"
#ifdef BOARD_STM32
#include "platformio_logo.h"
#endif

#ifdef BOARD_STM32
#include "stm32_lcd_functions.h"
#endif
#ifdef BOARD_ESP32
#include "esp32_lcd_functions.h"
#endif

#include <stdio.h>

/*LCD elements*/

extern unsigned short PLATFORMIO_LOGO_DATA[];

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

#ifdef BOARD_STM32
PICTURE pio_logo = {{.locationX = PIO_LOGO_LOCATION_X, .locationY = PIO_LOGO_LOCATION_Y, .height= PIO_LOGO_HEIGHT, .width = PIO_LOGO_WIDTH, .color = PIO_LOGO_COLOR, .isDisplayed = PIO_LOGO_ISDISPLAYED},.format = PIO_LOGO_FORMAT, .data = PLATFORMIO_LOGO_DATA};
#endif
#ifdef BOARD_ESP32
PICTURE pio_logo;
#endif

/*initialize bg_colors drawing*/

BG_COLORS bg_colors = {
                        {.locationX = BG_OPTIONS_LOCATION_X, .locationY = BG_OPTIONS_LOCATION_Y, .height= BG_OPTIONS_HEIGHT, .width = BG_OPTIONS_WIDTH, .color = BG_OPTIONS_COLOR, .isDisplayed = BG_OPTIONS_ISDISPLAYED}, 
                        .amountRows = BACKGROUND_COLOR_AMOUNT_ROWS, .colorsPerRow = BACKGROUND_COLORS_PER_ROW, .elementHeight = BG_OPTIONS_ELEMENT_HEIGHT, .elementWidth = BG_OPTIONS_ELEMENT_WIDTH
                      };




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
  clearBGFG();

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


                 