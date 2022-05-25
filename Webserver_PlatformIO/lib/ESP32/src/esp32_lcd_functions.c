#include "esp32_lcd_functions.h"


extern uint16_t* data_buf;
extern uint16_t background_color_esp;
extern sFONT defaultFont;

extern uint32_t bgcolors_array[];
extern MENU_BUTTON menu_button;
extern IP_BUTTON ip_button;
extern LCD_ELEMENT lcd_element_ip_address;
extern MENU menu;
extern PICTURE pio_logo;
extern BG_COLORS bg_colors;

extern unsigned short PLATFORMIO_LOGO_ESP_DATA[];

static const char* TAG = "esp32_lcd_functions";

void drawMenuButton(){
    uint16_t color = (uint16_t)(menu_button.base_element.color);

    for(int i=0;i<menu_button.amountStripes;i++){
    //Checking if button is active. If it is, draw hollow rectangles, otherwise draw filled ones
    if(!menu_button.isActive){
      fillRect(menu_button.base_element.locationX, menu_button.base_element.locationY + (menu_button.stripeWidth*2*i) , menu_button.base_element.width , menu_button.stripeWidth,color);
    }else if(menu_button.isActive){
      fillRect(menu_button.base_element.locationX, menu_button.base_element.locationY + (menu_button.stripeWidth*2*i) , menu_button.base_element.width , menu_button.stripeWidth,color);
    }
  }
}

void drawBgColorOptions(){

  int posX;
  int posY;
  uint16_t color;
  /*Looping over all color blocks*/
  for(int i=0;i<(bg_colors.colorsPerRow)*(bg_colors.amountRows);i++){
    posX =  bg_colors.base_element.locationX + (i % bg_colors.colorsPerRow)*bg_colors.elementWidth; 
    posY =  bg_colors.base_element.locationY + (i / bg_colors.colorsPerRow)*bg_colors.elementHeight;
    /*Selecting corresponding color*/
    color = (uint16_t)(bgcolors_array[i]);
    fillRect(posX,posY,bg_colors.elementWidth,bg_colors.elementHeight,color);
    /*Drawing border around*/
    color = (uint16_t)(bg_colors.base_element.color);
    fillRect(posX,posY,bg_colors.elementWidth,bg_colors.elementHeight,color);
  }
  return;    
}

void drawIPButton(){
  uint16_t color = (uint16_t)(ip_button.base_element.color);

  /*drawing border*/
  drawRect(ip_button.base_element.locationX,ip_button.base_element.locationY,ip_button.base_element.width,ip_button.base_element.height,color);
  /*Writing 'IP'*/
  defaultFont = IP_BUTTON_FONT;
  displayStringAt(ip_button.base_element.locationX + ip_button.paddingX , ip_button.base_element.locationY + ip_button.paddingY, (uint8_t*)"IP",color,LEFT_MODE);
}

void drawIPAddress(){
  /*
  uint32_t ip = getIpAddr();
  char* ip_addr_str;
  sprintf(ip_addr_str,IP_ADDRESS_TEXT, (uint8_t)(ip & 0x000000ff), (uint8_t)((ip & 0x0000ff00)>>8), (uint8_t) ((ip & 0x00ff0000)>>16), (uint8_t)((ip & 0xff000000)>>24));
  */
  char* ip_addr_str = "0.0.0.0";

  /* Displaying IP Address on LCD*/
  uint16_t color = (uint16_t)(lcd_element_ip_address.color);
  defaultFont = IP_ADDRESS_FONT;
  displayStringAt(lcd_element_ip_address.locationX,lcd_element_ip_address.locationY,(uint8_t*)ip_addr_str,color,LEFT_MODE);

  return;
}

void drawMenu(){
  uint16_t color = (uint16_t)(menu.base_element.color);

  /*Drawing border*/
  drawRect(menu.base_element.locationX,menu.base_element.locationY,menu.base_element.width,menu.base_element.height,color);

  /*Drawing elements*/
  /*element 1 - Home*/
  color = (uint16_t)(menu.elementHome.color);
  defaultFont = MENU_FONT;
  displayStringAt(menu.elementHome.locationX,menu.elementHome.locationY,(uint8_t*)"Home",color,LEFT_MODE);

  /*element 2 - Options*/
  color = (uint16_t)(menu.elementOptions.color);
  displayStringAt(menu.elementOptions.locationX,menu.elementOptions.locationY,(uint8_t*)"Options",color,LEFT_MODE);

  /*element 3 - build info*/
  color = (uint16_t)(menu.elementBuildinfo.color);
  displayStringAt(menu.elementBuildinfo.locationX,menu.elementBuildinfo.locationY,(uint8_t*)"Build info",color,LEFT_MODE);

  /*element 4 - differences*/
  color = (uint16_t)(menu.elementDifferences.color);
  displayStringAt(menu.elementDifferences.locationX,menu.elementDifferences.locationY,(uint8_t*)"Windows vs. Linux",color,LEFT_MODE);

  /*element 5 - Chat*/
  color = (uint16_t)(menu.elementChat.color);
  displayStringAt(menu.elementChat.locationX,menu.elementChat.locationY,(uint8_t*)"Chat",color,LEFT_MODE);


  return;
}

void drawPIOLogo(){
  /*Calling custom draw function that uses DMA2D*/
  drawBitmap(PLATFORMIO_LOGO_ESP_DATA,PIO_LOGO_LOCATION_X,PIO_LOGO_LOCATION_Y,PLATFORMIO_LOGO_ESP_DATA_X_PIXEL,PLATFORMIO_LOGO_ESP_DATA_Y_PIXEL);
  return;
}


void clearBGFG(){
    fillBackground();
}






void init_ESP_LCD_Buffer(){
    data_buf = (uint16_t *)heap_caps_calloc(SCREEN_XSIZE * SCREEN_YSIZE, sizeof(uint16_t), MALLOC_CAP_8BIT);
    
    if(data_buf != NULL){
        ESP_LOGI(TAG,"LCD calloc ok");
    }else{
        ESP_LOGI(TAG,"LCD calloc not ok");   
    }
    
}


void fillBackground(){
    for(int x=0;x<SCREEN_XSIZE;x++){
        for(int y=0;y<SCREEN_XSIZE;y++){
            data_buf[y*SCREEN_XSIZE+x] = background_color_esp;
        }
    }
}

void drawHLine(int posx_start, int posy, int posx_end, uint16_t color){
    for(int x=posx_start;x<posx_end;x++){
        data_buf[posy*SCREEN_XSIZE+x] = color;
    }
}

void drawVLine(int posx, int posy_start,int posy_end, uint16_t color){
    for(int y=posy_start;y<posy_end;y++){
        data_buf[y*SCREEN_XSIZE+posx] = color;
    }
}


void drawRect(int xstart,  int ystart, int width, int height, uint16_t color){
    drawHLine(xstart,ystart,xstart + width,color);
    drawHLine(xstart,ystart + height,xstart + width,color);
    drawVLine(xstart,ystart,ystart + height,color);
    drawVLine(xstart + width,ystart,ystart + height,color);
}

void fillRect(int xstart, int ystart,int width, int height, uint16_t color){
    for(int x=xstart;x<xstart+width;x++){
        for(int y = ystart;y<ystart+height;y++){
            data_buf[y*SCREEN_XSIZE+x] = color;
        }
    }
}

//adapted from BSP library
void drawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c, uint16_t color)
{
  //ESP_LOGI(TAG,"LCD draw char");    
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t  offset;
  uint8_t  *pchar;
  uint32_t line;
  
  height = defaultFont.Height;
  width  = defaultFont.Width;
  
  offset =  8 *((width + 7)/8) -  width ;
  
  for(i = 0; i < height; i++)
  {
    //ESP_LOGI("LCD c = %d",(int)c);
    pchar = (c + (width + 7)/8 * i);
    //ESP_LOGI("LCD pchar = %d",(int)pchar);
    switch(((width + 7)/8))
    {
      
    case 1:
      line =  pchar[0];      
      break;
      
    case 2:
      line =  (pchar[0]<< 8) | pchar[1];      
      break;
      
    case 3:
    default:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];      
      break;
    } 
    
    for (j = 0; j < width; j++)
    {
      if(line & (1 << (width- j + offset- 1))) 
      { 
        data_buf[((Xpos + j) + Ypos*SCREEN_XSIZE)] = color;
      }
    }
    Ypos++;
  }
}

/*adapted from BSP*/
void displayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii,uint16_t color)
{
  /*ESP_LOGI("LCD display char %u",Ascii);
  ESP_LOGI("LCD display char table %d",defaultFont.table[(Ascii-' ') * defaultFont.Height * ((defaultFont.Width + 7) / 8)]);
  */
  drawChar(Xpos, Ypos, &(defaultFont.table[(Ascii-' ') * defaultFont.Height * ((defaultFont.Width + 7) / 8)]),color);
}


/*adapted from BSP*/
void displayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, uint16_t color, int textAlignMode)
{
  ESP_LOGI(TAG, "LCD display String at");
  uint16_t ref_column = 1, i = 0;
  uint32_t size = 0, xsize = 0; 
  uint8_t  *ptr = Text;
  
  /* Get the text size */
  while (*ptr++) size ++ ;
  
  /* Characters number per line */
  xsize = (SCREEN_XSIZE/defaultFont.Width);
  
  
  switch (textAlignMode)
  {
  case CENTER_MODE:
    {
      ref_column = Xpos + ((xsize - size)* defaultFont.Width) / 2;
      break;
    }
  case LEFT_MODE:
    {
      ref_column = Xpos;
      break;
    }
  case RIGHT_MODE:
    {
      ref_column = - Xpos + ((xsize - size)*defaultFont.Width);
      break;
    }    
  default:
    {
      ref_column = Xpos;
      break;
    }
  }    
  
  /* Check that the Start column is located in the screen */
  if ((ref_column < 1) || (ref_column >= 0x8000))
  {
    ref_column = 1;
  }

  /* Send the string character by character on LCD */
  while ((*Text != 0) & (((SCREEN_XSIZE - (i*defaultFont.Width)) & 0xFFFF) >= defaultFont.Width))
  {
    /* Display one character on LCD */
    displayChar(ref_column, Ypos, *Text,color);
    /* Decrement the column position by 16 */
    ref_column += defaultFont.Width;
    /* Point on the next character */
    Text++;
    i++;
  }  
}


void drawBitmap(uint16_t* picturedata, uint16_t startx, uint16_t starty, int picX, int picY){
  if(startx+picX > SCREEN_XSIZE || starty + picY > SCREEN_YSIZE){
    ESP_LOGI(TAG, "picture doesn't fit on screen");
  }
  else{
    for(int x=0;x<picX;x++){
      for(int y=0;y<picY;y++){
        //if BMP has 0xff7d, it means transparent (edited BMP manually)
        if(picturedata[x+y*picX]!=0xff7d){
          data_buf[(x+startx)+SCREEN_XSIZE*(y+starty)]=picturedata[x+y*picX];
        }
      }
    }
  }
}


/*Checking all elements if they need to be displayed*/
void renderFrame(){
  clearBGFG();

  /*Menu button*/
  if(menu_button.base_element.isDisplayed){
    ESP_LOGI(TAG,"drawing menu button:");
    drawMenuButton();
  }

  /*IP button*/
  if(ip_button.base_element.isDisplayed){
    ESP_LOGI(TAG,"drawing IP button:");
    drawIPButton();
  }

  /*IP Address*/
  if(lcd_element_ip_address.isDisplayed){
    ESP_LOGI(TAG,"drawing IP address:");
    drawIPAddress();
  }

  if(menu.base_element.isDisplayed){
    ESP_LOGI(TAG,"drawing menu:");
    drawMenu();
  }

  /*pio logo*/
  if(pio_logo.base_element.isDisplayed){
    ESP_LOGI(TAG,"drawing pio logo:");
    drawPIOLogo();
  }

  /*BG color block options*/
  if(bg_colors.base_element.isDisplayed){
    ESP_LOGI(TAG,"drawing bg color options:");
    drawBgColorOptions();
  }

  

  lcd_set_index(0,0, SCREEN_XSIZE-1,SCREEN_YSIZE-1);
  ESP_LOGI(TAG,"attempting data buf write at: %d",(uint8_t)data_buf);
  lcd_write_data((uint8_t*)data_buf,SCREEN_XSIZE*SCREEN_YSIZE * sizeof(uint16_t)); //sizeof uint16_t bc every pixel is in 565 format -> 16 bits
  ESP_LOGI(TAG,"data buf write ok");
  return;
}