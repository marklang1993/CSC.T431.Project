#include <M5Stack.h>

#include "./display.h"
#include "./screen1.h"
#include "./screen2.h"

static bool isScreen1;

static void displayInit()
{
    M5.begin();
    startRender1();
    isScreen1 = true;
}

static void displayLoop()
{
    if (M5.BtnA.wasPressed())
    {
        isScreen1 = !isScreen1;
        if (isScreen1)
            startRender1();
        else
            startRender2();
    }

    if (M5.BtnB.wasPressed())
    {
        if (isScreen1)
            ;
        else
            next();
    }
    if (M5.BtnC.wasPressed())
    {
        if (isScreen1)
            ;
        else
            ok();
    }

    M5.update();
}

void displayTask(void *pvParameters)
{
    displayInit();
    while (1)
    {
        displayLoop();
        delay(10);
    }
}