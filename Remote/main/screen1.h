#include "./ui.h"
#include "./colorscheme.h"

const String vars[6] = {"Voltage", "Current", "Relay", "Motor", "Power", "Temperature"};
const int data[6] = {12345, 12345, 1, 1, 123, 1234};
String unitArr[2] = {"V", "A"};

void showText()
{
    for (int i = 0; i < 6; i++)
    {
        char param;
        M5.Lcd.setCursor(20, i * 33 + 30);
        M5.Lcd.setFreeFont(FSS12);
        M5.Lcd.setTextColor(TEXT_DARK);
        M5.Lcd.print(vars[i] + ": ");
        switch (i)
        {
        case 0:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            int f = data[i] / 1000;
            M5.Lcd.print(String(f) + '.' + (data[i] - f * 1000) + " " + unitArr[i]);
            break;
        }
        case 1:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            int f = data[i] / 1000;
            M5.Lcd.print(String(f) + '.' + (data[i] - f * 1000) + " " + unitArr[i]);
            break;
        }
        case 2:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            M5.Lcd.print(data[i] == 0 ? "Close" : "Open");
            break;
        }
        case 3:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            switch (data[i])
            {
            case 0:
                M5.Lcd.print("Stop");
                break;
            case 1:
                M5.Lcd.print("Forward");
                break;
            case 2:
                M5.Lcd.print("Backword");
                break;
            default:
                M5.Lcd.print("Error");
                break;
            }
            break;
        }
        case 5:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            int f = data[i] / 100;
            M5.Lcd.print(String(f) + '.' + (data[i] - f * 100) + " C");
            break;
        }
        case 4:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            M5.Lcd.print(String(data[i]) + "%");
        }
        default:
        {
            break;
        }
        }
    }
    M5.Lcd.setTextColor(TEXT_LIGHT);
    M5.Lcd.setFreeFont(FSS9);
    M5.Lcd.setCursor(13, 226);
    M5.Lcd.print("Commands");

    for (int i = 0; i < 5; i++)
    {
        M5.Lcd.fillRect(0, i * 33 + 38, 320, 2, BG_DARK);
    }
}

void startRender1()
{
    drawBackground();
    drawButtonBar();
    showText();
    drawRadioOn(160, 75);
}
