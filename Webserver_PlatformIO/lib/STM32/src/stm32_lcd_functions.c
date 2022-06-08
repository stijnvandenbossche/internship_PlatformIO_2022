#include "stm32_lcd_functions.h"
#include "lcd_elements.h"
#include <stdint.h>



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
extern char* chat_messages[];


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
  return;
}

void drawChatBox(){

  return;
}


void clearBGFG(){
    /*clear background*/
  BSP_LCD_SelectLayer(0);
  BSP_LCD_Clear(background_color);

  /*clear foreground*/
  BSP_LCD_SelectLayer(1);
  BSP_LCD_Clear(background_color);
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

  /*chat box*/
  if(chat_box.base_element.isDisplayed){
    drawChatBox();
  }  

  return;
}
