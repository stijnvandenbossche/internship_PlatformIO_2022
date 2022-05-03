#ifndef ESP32_LCD_FUNCTIONS_H_
#define ESP32_LCD_FUNCTIONS_H_

/*defines for colors*/
#define IMAGE_MAX_SIZE (100 * 1024)/**< The maximum size of a single picture in the boot animation */
#define IMAGE_WIDTH    320 /*!< width of jpeg file */
#define IMAGE_HIGHT    240 /*!< height of jpeg file */

//colors
#define LCD_COLOR_BLUE ((uint16_t) 0b0000000000011111)
#define LCD_COLOR_GREEN ((uint16_t) 0b0000011111100000)
#define LCD_COLOR_RED ((uint16_t) 0b1111100000000000)
#define LCD_COLOR_CYAN ((uint16_t) 0b0000011111111111)
#define LCD_COLOR_MAGENTA ((uint16_t) 0b1111100000011111)
#define LCD_COLOR_YELLOW ((uint16_t) 0b1111111111100000)
#define LCD_COLOR_LIGHTBLUE ((uint16_t) 0b1000010000011111)
#define LCD_COLOR_LIGHTGREEN ((uint16_t) 0b1000011111110000)
#define LCD_COLOR_LIGHTRED ((uint16_t) 0xFAAA)
#define LCD_COLOR_LIGHTCYAN ((uint16_t) 0b1000011111111111)
#define LCD_COLOR_LIGHTMAGENTA ((uint16_t) 0b1111110000011111)
#define LCD_COLOR_LIGHTYELLOW ((uint16_t) 0b1111111111110000)
#define LCD_COLOR_DARKBLUE ((uint16_t) 0xA638)
#define LCD_COLOR_DARKGREEN ((uint16_t) 0b0000010000000000)
#define LCD_COLOR_DARKYELLOW ((uint16_t) 0xFDE3)
#define LCD_COLOR_WHITE ((uint16_t) 0b1111111111111111)
#define LCD_COLOR_DARKGRAY ((uint16_t) 0b0110001100001100)
#define LCD_COLOR_GRAY ((uint16_t) 0b1000010000010000)
#define LCD_COLOR_LIGHTGRAY ((uint16_t) 0b0010000110000100)
#define LCD_COLOR_BLACK ((uint16_t) 0b0000000000000000)
#define LCD_COLOR_BROWN ((uint16_t) 0x82A5)
#define LCD_COLOR_ORANGE ((uint16_t) 0b1111101110100000)

//extra defines to ensure compatibility, to be adjusted
#define LCD_COLOR_DARKRED ((uint16_t) 0b1111100000000000)
#define LCD_COLOR_DARKCYAN ((uint16_t) 0b0000011111111111)
#define LCD_COLOR_DARKMAGENTA ((uint16_t) 0b1111100000011111)

#define CENTER_MODE 1
#define LEFT_MODE 2
#define RIGHT_MODE 3



void drawMenuButton(void);
void drawBgColorOptions(void);
void drawIPButton(void);
void drawIPAddress(void);
void drawMenu(void);
void drawPIOLogo(void);
void clearBGFG(void);


#endif