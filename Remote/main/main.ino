#include <M5Stack.h>
/*
    Edited from AOSLogo.ino from M5Samples_Arduino
    https://github.com/titech-aos/M5Samples_Arduino.git
*/
// Taken from "Free_Fonts.h" in "Free_Font_Demo"
#define GFXFF 1
#define FSS24 &FreeSans24pt7b
#define FF6 &FreeMonoBold12pt7b
#define FSI12 &FreeSerifItalic12pt7b

#define ALEN(a) (sizeof (a) / sizeof (a)[0])

const String text1[] = { "Team Noob", "UI", "Demo" };
const String text2[] = { "Font Style:", "FreeMonoBold", 
                         "",
                         "Font Size:", "12pt",
                         "",
                         "Color:", "Black"};

const String text3[] = { "Font Style:", "FreeSerifItalic",
                         "",
                         "Font Size:", "12pt",
                         "",
                         "Color:", "Yellow" };

void showText(const String text[], int text_len,
             uint16_t text_color = TFT_WHITE, uint16_t bg_color = TFT_BLACK,
             const GFXfont *text_font = FSS24) {
    M5.Lcd.fillScreen(bg_color);
    M5.Lcd.setFreeFont(text_font);
    M5.Lcd.setTextColor(text_color);
    int fh = M5.Lcd.fontHeight(GFXFF) * 0.9;
    int x = M5.Lcd.width() / 2;
    int y = M5.Lcd.height() / 2 - fh * text_len / 2;
    for (int i = 0; i < text_len; i++) {
        M5.Lcd.setTextDatum(TC_DATUM);
        M5.Lcd.drawString(text[i], x, y);
        y += fh;
    }
}

void setup() {
    M5.begin();
    showText(text1, ALEN(text1), TFT_CYAN);
}

void loop() {
    if (M5.BtnA.wasPressed())
        showText(text1, ALEN(text1), TFT_CYAN);
    if (M5.BtnB.wasPressed())
        showText(text2, ALEN(text2), TFT_BLACK, TFT_LIGHTGREY, FF6);
    if (M5.BtnC.wasPressed())
        showText(text3, ALEN(text3), TFT_YELLOW, TFT_DARKCYAN, FSI12);
    M5.update();
}
