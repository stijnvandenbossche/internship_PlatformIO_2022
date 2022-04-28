#include "lcd_elements.h"

/*global defs*/ 
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

/*initialize bg_colors drawing*/

BG_COLORS bg_colors = {
                        {.locationX = BG_OPTIONS_LOCATION_X, .locationY = BG_OPTIONS_LOCATION_Y, .height= BG_OPTIONS_HEIGHT, .width = BG_OPTIONS_WIDTH, .color = BG_OPTIONS_COLOR, .isDisplayed = BG_OPTIONS_ISDISPLAYED}, 
                        .amountRows = BACKGROUND_COLOR_AMOUNT_ROWS, .colorsPerRow = BACKGROUND_COLORS_PER_ROW, .elementHeight = BG_OPTIONS_ELEMENT_HEIGHT, .elementWidth = BG_OPTIONS_ELEMENT_WIDTH
                      };




