#ifndef STM32_LCD_FUNCTIONS_H_
#define STM32_LCD_FUNCTIONS_H_

void drawMenuButton(void);
void drawBgColorOptions(void);
void drawIPButton(void);
void drawIPAddress(void);
void drawMenu(void);
void drawPIOLogo(void);
void drawChatBox(void);
void drawCompilationChart(void);
void clearBGFG(void);

void processChatMessage(char*);
void addMessage(char*);

void renderFrame(void);



#endif