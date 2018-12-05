#include <M5Stack.h>
#include "display.h"
#include "comm.h"

void setup()
{
    // Display
    xTaskCreatePinnedToCore(
        displayTask,
        "Display",
        16384,
        NULL,
        2,
        NULL,
        0
    );

    // Communication
    xTaskCreatePinnedToCore(
        commTask,
        "Communication",
        16384,
        NULL,
        1,
        NULL,
        0
    );
}

void loop()
{
    vTaskDelete(NULL);
}
