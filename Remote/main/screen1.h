#define FSS12 &FreeSans12pt7b

const String vars[6] = {"Voltage", "Current", "Relay", "Motor", "Power", "Temperature"};
const int data[6] = {1234, 1234, 1234, 1, 1234, 1234};
String unitArr[6] = {"V", "A", "", "", "%", "C"};

void showText()
{
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setFreeFont(FSS12);
    for (int i = 0; i < 6; i++)
    {
        M5.Lcd.setCursor(0, i * 30 + 30);
        M5.Lcd.setTextColor(TFT_CYAN);
        M5.Lcd.print(vars[i] + ": ");
        if (i == 2)
        {
            M5.Lcd.setTextColor(TFT_MAGENTA);
            M5.Lcd.print(data[i] == 0 ? "Close" : "Open");
        }

        else if (i == 3)
        {
            M5.Lcd.setTextColor(TFT_MAGENTA);
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
            M5.Lcd.setTextColor(TFT_MAGENTA);
            int f = data[i] / 100;
            M5.Lcd.print(String(f) + '.' + (data[i] - f * 100) + " " + unitArr[i]);
        }
    }
}
void startRender()
{
    showText();
}
