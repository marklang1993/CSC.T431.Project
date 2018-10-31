# Advanced System Software UI
The current code is edited from AOSLogo.ino from [M5Samples_Arduino](https://github.com/titech-aos/M5Samples_Arduino).

Below are the essence of [M5Stack/Free_Font_Demo.ino at master · m5stack/M5Stack · GitHub](https://github.com/m5stack/M5Stack/blob/master/examples/Advanced/Display/Free_Font_Demo/Free_Font_Demo.ino)* and [M5Stack Documentation](https://media.readthedocs.org/pdf/m5stack/latest/m5stack.pdf), take a look at them for more details.
## Fonts
Fonts defined are from “Free_Fonts.h” in “Free_Font_Demo” from M5Stack library.
### setTextColor()
`Lcd.setTextColor(FOREGROUND, BACKGROUND)`
#### Description
Set the colour of printing text. Background can be omitted.
#### Example
```c
M5.Lcd.setTextColor(TFT_YELLOW);
M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
```
### setFreeFont()
`Lcd.setFreeFont(FONT)`
#### Description
Set the font style of printing text. FONT is defined in [M5Stack/Free_Fonts.h at master · m5stack/M5Stack · GitHub](https://github.com/m5stack/M5Stack/blob/master/examples/Advanced/Display/Free_Font_Demo/Free_Fonts.h)
#### Example
> *in Free_Font.h,*  
```c
...
#define FSB9 &FreeSerifBold9pt7b
...
```
Hence in use,
```c
M5.Lcd.setFreeFont(FSB9);             // Select the font as Free Serif Bold 9pt
```

## Print
There are two ways of printing text, using `print()`method or using `drawString()`method. With `drawString()`, you can specify a background colour of texts. 
###  print()
`Lcd.print(‘text’, [x, y])`/`Lcd.println(‘text’, [x, y])`
#### Description
Print  the text at position (x, y).
#### Parameters
**(string) text:** the string need to print.

**(int) x:** the x coordinate of destination.

**(int) y:** the y coordinate of destination.
#### Example
```c
// For comaptibility with Adafruit_GFX library the text background is not plotted when using the print class even if we specify it.
lcd.print('this is a print text function', 80, 80)
```


### drawString()
`Lcd.drawString(‘text’, [x, y])`
#### Description
Print  the text at position (x, y).
#### Parameters
**(string) text:** the string need to print.

**(int) x:** the x coordinate of destination.

**(int) y:** the y coordinate of destination.
#### Example
```c
M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);

M5.Lcd.setTextDatum(TC_DATUM); // Centre text on x,y position

xpos = M5.Lcd.width() / 2; // Half the screen width
ypos = 50;

M5.Lcd.setFreeFont(FSB9);                              // Select the font
M5.Lcd.drawString("Serif Bold 9pt", xpos, ypos, GFXFF);  // Draw the text string in the selected GFX free font
ypos += M5.Lcd.fontHeight(GFXFF);                        // Get the font height and move ypos down
```
