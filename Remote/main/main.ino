#include <M5Stack.h>
#include "./display.h"

void setup()
{
    xTaskCreatePinnedToCore(
        displayTask,
        "Display",
        16384,
        NULL,
        2,
        NULL,
        0
    );
}

void loop()
{
    vTaskDelete(NULL);
}
