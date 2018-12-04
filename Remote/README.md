# Advanced System Software UI

Take a look at [Cheat Sheet](https://github.com/Kongduino/M5CheatSheet).
The colour style is from [Flat UI](http://www.bootcss.com/p/flat-ui/).
## Code Structures
`colorscheme.h` : Constants for defining colours and fonts.

`ui.h` : Functions for drawing general UI components.
## Fonts
Fonts defined are from “Free_Fonts.h” in “Free_Font_Demo” from M5Stack library.
### setTextColor()
`Lcd.setTextColor(FOREGROUND, BACKGROUND)`
#### Description
Set the colour of printing text. Background can be omitted. 

TFT preset colours are: 
[image:77143965-D99A-4CB4-B75C-6E6007C8BED4-662-00001CBB5A18E9AF/CCDCC328-64B4-48C8-B936-A883A95BE6CD.png]

You can also use custom colours, but remember to convert the hex value from **RGB888** to **RGB565**.
[Online Conversion Tool](http://www.barth-dev.de/online/rgb565-color-picker/)


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
#define FSB9 &FreeSerifBold9pt7b
M5.Lcd.setFreeFont(FSB9);             // Select the font as Free Serif Bold 9pt
```

## Print
There are two ways of printing text, using `print()`method or using `drawString()`method. With `drawString()`, you can specify a background colour of texts. 
###  print()
`Lcd.print(‘text’`/`Lcd.println(‘text’)`
#### Description
Print  the text at cursor location.
#### Parameters
**(string) text:** the string need to print.
**(int) x:** the x coordinate of destination.
**(int) y:** the y coordinate of destination.
#### Example
```c
lcd.print('this is a print text function')
```

### drawString()
`Lcd.drawString(‘text’, x, y)`
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
