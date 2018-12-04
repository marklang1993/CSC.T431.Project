#include "./ui.h"
#include "./colorscheme.h"

const String vars[6] = {"Voltage", "Current", "Relay", "Motor", "Power", "Temperature"};
const int data[6] = {1234, 1234, 1234, 1, 1234, 1234};
String unitArr[6] = {"V", "A", "", "", "%", "C"};

void showText()
{
    for (int i = 0; i < 6; i++)
    {
        M5.Lcd.setCursor(20, i * 33 + 30);
        M5.Lcd.setFreeFont(FSS12);
        M5.Lcd.setTextColor(TEXT_DARK);
        M5.Lcd.print(vars[i] + ": ");
        if (i == 2)
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            M5.Lcd.print(data[i] == 0 ? "Close" : "Open");
        }

        else if (i == 3)
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
        }
        else
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            int f = data[i] / 100;
            M5.Lcd.print(String(f) + '.' + (data[i] - f * 100) + " " + unitArr[i]);
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
void startRender()
{
    M5.Lcd.fillScreen(BG_LIGHT);
    drawButtonBar();
    showText();
}