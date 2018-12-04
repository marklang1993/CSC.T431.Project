const String commands[6] = {"Empty", "Stop", "Forward", "Backward", "SetDutyRatio", "SetRelay"};
int selected = 0;

void update2(int selected)
{
    // Draw static UI
    M5.Lcd.clear();
    drawBackground();
    drawButtonBar();
    M5.Lcd.fillRect(0, 65, 320, 5, BG_DARK);
    M5.Lcd.fillRect(0, 135, 320, 5, BG_DARK);
    M5.Lcd.setTextColor(TEXT_LIGHT);
    M5.Lcd.setFreeFont(FSS9);
    M5.Lcd.setCursor(35, 226);
    M5.Lcd.print("Status");
    M5.Lcd.setCursor(140, 226);
    M5.Lcd.print("Next");
    M5.Lcd.setCursor(240, 226);
    M5.Lcd.print("OK");
    if (selected <= 3)
    {
        M5.Lcd.setFreeFont(FSS18);
        for (int i = 0; i < 3; i++)
        {
            M5.Lcd.setTextColor(TEXT_LIGHT);
            if (selected == i)
            {
                M5.Lcd.fillRect(0, 70 * i, 320, 65, BG_DARK);
                M5.Lcd.setTextColor(FRONT_LIGHT);
            }
            M5.Lcd.setCursor(20, i * 70 + 45);
            M5.Lcd.print(commands[i]);
        }
    }
}
void startRender2()
{
    update2(selected);
}
void next()
{
    if (selected < 2)
    {
        selected++;
    }
    else
    {
        selected = 0;
    }
    update2(selected);
}