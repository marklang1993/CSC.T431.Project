#include <M5Stack.h>
#include "./screen1.h"
/*
    Edited from AOSLogo.ino from M5Samples_Arduino
    https://github.com/titech-aos/M5Samples_Arduino.git
*/
// Taken from "Free_Fonts.h" in "Free_Font_Demo"

void setup()
{
    M5.begin();
    startRender();
}

void loop()
{
    // if (M5.BtnA.wasPressed())
    //     showText(text1, ALEN(text1), TFT_CYAN);
    // if (M5.BtnB.wasPressed())
    //     showText(text2, ALEN(text2), TFT_BLACK, TFT_LIGHTGREY, FF6);
    // if (M5.BtnC.wasPressed())
    //     showText(text3, ALEN(text3), TFT_YELLOW, TFT_DARKCYAN, FSI12);
    // M5.update();
}
