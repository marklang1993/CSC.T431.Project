const String commands[5] = {"Stop", "Forward", "Backward", "SetDutyRatio", "SetRelay"};
int selected = 0;
int percentage = 0;
bool isDataSelecting = false;

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
    M5.Lcd.setFreeFont(FSS18);
    if (selected < 3)
    {
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
    else
    {
        for (int i = 0; i < 3; i++)
        {
            M5.Lcd.setTextColor(TEXT_LIGHT);
            if (selected - 2 == i)
            {
                M5.Lcd.fillRect(0, 70 * i, 320, 65, BG_DARK);
                M5.Lcd.setTextColor(FRONT_LIGHT);
            }
            M5.Lcd.setCursor(20, i * 70 + 45);
            M5.Lcd.print(commands[i + 2]);
        }
    }
}
void startRender2()
{
    update2(selected);
}
void updateDataSelection(int per)
{
    M5.Lcd.clear();
    drawBackground();
    drawButtonBar();
    M5.Lcd.setTextColor(TEXT_LIGHT);
    M5.Lcd.setFreeFont(FSS18);
    M5.Lcd.drawString(String(per) + "%", 100, 100);
    M5.Lcd.setFreeFont(FSS9);
    M5.Lcd.setCursor(35, 226);
    M5.Lcd.print("Status");
    M5.Lcd.setCursor(140, 226);
    M5.Lcd.print("10%");
    M5.Lcd.setCursor(240, 226);
    M5.Lcd.print("OK");
}
void next()
{
    if (isDataSelecting)
    {
        if (percentage == 100)
        {
            percentage = 0;
        }
        else
        {
            percentage += 10;
        }
        updateDataSelection(percentage);
    }
    else
    {
        if (selected < 4)
        {
            selected++;
        }
        else
        {
            selected = 0;
        }
        update2(selected);
    }
}
void ok()
{
    if (isDataSelecting)
    {
        //send data;
        update2(selected);
        isDataSelecting = false;
    }
    else
    {
        if (selected < 3)
            //send command
            ;
        else
        {
            percentage = 0;
            isDataSelecting = true;
            updateDataSelection(percentage);
        }
    }
}
