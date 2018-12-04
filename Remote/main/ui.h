/*
This header file contains some functions drawing UI components that can be used in different windows.
*/
#include "./colorscheme.h"
void drawButtonBar()
{
    M5.Lcd.fillRoundRect(10, 205, 300, 40, 7, FRONT_LIGHT);
    M5.Lcd.fillRect(109, 205, 2, 40, FRONT_DARK);
    M5.Lcd.fillRect(209, 205, 2, 40, FRONT_DARK);
}
void drawBackground()
{
    M5.Lcd.fillScreen(BG_LIGHT);
}