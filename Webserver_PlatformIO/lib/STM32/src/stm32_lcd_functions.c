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
extern COMPILATION_CHART compilation_chart;
extern LCD_ELEMENT build_info;

extern char chat_messages[][MAX_LENGTH_CHAT_MESSAGE];
extern char compilation_chart_stm[][COMPILATION_TABLE_MAX_MSG_LENGTH];
extern char compilation_chart_esp[][COMPILATION_TABLE_MAX_MSG_LENGTH];

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

  BSP_LCD_SetBackColor(background_color);
  BSP_LCD_SetFont(&BG_OPTIONS_TITLE_FONT);
  BSP_LCD_DisplayStringAt(BG_OPTIONS_TITLE_LOCATION_X,BG_OPTIONS_TITLE_LOCATION_Y,(uint8_t*)"Select background color",LEFT_MODE);

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
  BSP_LCD_SetTextColor(chat_box.base_element.color);
  BSP_LCD_SetBackColor(background_color);
  BSP_LCD_SetFont(&CHAT_BOX_FONT);

  int i=0;
  int amountMsg = chat_box.amountmessages;
  while(i<amountMsg){
    BSP_LCD_DisplayStringAt(chat_box.base_element.locationX, chat_box.base_element.locationY + (chat_box.messageHeight * i),(uint8_t*)(chat_messages[i]),LEFT_MODE);
    i++;
  }
  return;
}

void drawCompilationChart(){
  BSP_LCD_SetTextColor(compilation_chart.base_element.color);
  BSP_LCD_SetBackColor(background_color);
  
  BSP_LCD_SetFont(&COMPILATION_TABLE_TITLE_FONT);

  BSP_LCD_DisplayStringAt(COMPILATION_TABLE_LOCATION_X,COMP_TABLE_HEADER_LOC_Y,(uint8_t*)"Compilation times (s)",LEFT_MODE);
  /*displaying table title*/
  if(compilation_chart.selectedTable==STM_TABLE){ 
    BSP_LCD_DisplayStringAt(compilation_chart.base_element.locationX, compilation_chart.base_element.locationY,(uint8_t*)"STM32",LEFT_MODE);
  } else if(compilation_chart.selectedTable==ESP_TABLE){
    BSP_LCD_DisplayStringAt(compilation_chart.base_element.locationX, compilation_chart.base_element.locationY,(uint8_t*)"ESP32",LEFT_MODE);
  }

  BSP_LCD_SetFont(&COMPILATION_TABLE_FONT);

  /*looping over all content to be displayed*/
  for(int row=0;row<compilation_chart.amountRows;row++){
    for(int col=0;col<compilation_chart.amountColumns;col++){
      if(compilation_chart.selectedTable == STM_TABLE){
        BSP_LCD_DisplayStringAt( compilation_chart.base_element.locationX + col * COMPILATION_TABLE_CELL_WIDTH, compilation_chart.titleHeight + compilation_chart.base_element.locationY + row * COMPILATION_TABLE_CELL_HEIGHT, (uint8_t*) compilation_chart_stm[ row * compilation_chart.amountColumns + col], LEFT_MODE);
      }else if(compilation_chart.selectedTable == ESP_TABLE){
        BSP_LCD_DisplayStringAt( compilation_chart.base_element.locationX + col * COMPILATION_TABLE_CELL_WIDTH, compilation_chart.titleHeight + compilation_chart.base_element.locationY + row * COMPILATION_TABLE_CELL_HEIGHT, (uint8_t*) compilation_chart_esp[ row * compilation_chart.amountColumns + col], LEFT_MODE);
      }
    }
  }

 

  return;
}


void drawBuildInfo(){
  BSP_LCD_SetTextColor(build_info.color);
  BSP_LCD_SetBackColor(background_color);
  
  BSP_LCD_SetFont(&BUILD_INFO_FONT);
  char string1[80] = "Compiled on ";
  strcat(string1,__DATE__);
  strcat(string1,",");

  char string2[50] = "at ";
  strcat(string2, __TIME__);
  strcat(string2,".");

  BSP_LCD_DisplayStringAt(build_info.locationX,build_info.locationY,(uint8_t*)string1,LEFT_MODE);
  BSP_LCD_DisplayStringAt(build_info.locationX,build_info.locationY + 30,(uint8_t*)string2,LEFT_MODE);

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

  /*compilation chart*/
  if(compilation_chart.base_element.isDisplayed){
    drawCompilationChart();
  }  

  if(build_info.isDisplayed){
    drawBuildInfo();
  }
  return;
}


void processChatMessage(char* msg){
  char temp[MAX_LENGTH_CHAT_MESSAGE];
  uri_decode(msg,strlen(msg),temp);

  if(strlen(temp)>=2*MAX_LENGTH_CHAT_MESSAGE){
    strcpy(temp,"The message is too long");
  }
  
  addMessage(temp);
  

  renderFrame();
}

void addMessage(char* msg){

  int amountMsg = chat_box.amountmessages;        
  if(amountMsg<MAX_AMOUNT_CHAT_MESSAGES){
    strcpy(chat_messages[amountMsg],msg);
    chat_box.amountmessages = chat_box.amountmessages+1;
  }
  /* j has index of first available spot in array, if none available, j=10 */
  else if(amountMsg>=MAX_AMOUNT_CHAT_MESSAGES){
    
    /*shifting all over one*/
    for(int k=0; k < (MAX_AMOUNT_CHAT_MESSAGES-1) ; k++){
      strcpy(chat_messages[k],chat_messages[k+1]);
    }
    /*writing message to last open spot*/
    strcpy(chat_messages[MAX_AMOUNT_CHAT_MESSAGES-1],msg);
  }

  return;
}