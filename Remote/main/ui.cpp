
#include <M5Stack.h>
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

void drawRadioOff(int xpos, int ypos)
{
    M5.Lcd.fillRoundRect(xpos, ypos, 85, 25, 13, TEXT_DARK);
    M5.Lcd.fillCircle(xpos + 17, ypos + 12, 10, GREY_LIGHT);
    M5.Lcd.setTextColor(TEXT_LIGHT);
    M5.Lcd.setFreeFont(FSS9);
    M5.Lcd.drawString("OFF", xpos + 37, ypos + 5);
}

void drawRadioOn(int xpos, int ypos)
{
    M5.Lcd.fillRoundRect(xpos, ypos, 85, 25, 13, BG_LIGHT);
    M5.Lcd.fillCircle(xpos + 65, ypos + 12, 10, FRONT_LIGHT);
    M5.Lcd.setTextColor(TEXT_LIGHT);
    M5.Lcd.setFreeFont(FSS9);
    M5.Lcd.drawString("ON", xpos + 7, ypos + 5);
}
