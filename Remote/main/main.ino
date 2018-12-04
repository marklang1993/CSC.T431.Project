#include <M5Stack.h>
#include "./screen1.h"
#include "./screen2.h"

bool isScreen1 = true;
void setup()
{
    M5.begin();
    startRender1();
}

void loop()
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

    M5.update();
}
