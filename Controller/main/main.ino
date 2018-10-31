#include "led.h"
#include "display.h"
#include "comm.h"
#include "scan.h"
#include "drive.h"


void taskInit() {
    // All tasks should be initialized here
    xTaskCreatePinnedToCore(
        ledTask,
        "LED",
        4096,
        NULL,
        1,
        NULL,
        0
    ); // LED

    xTaskCreatePinnedToCore(
        commTask,
        "Communication",
        4096,
        NULL,
        2,
        NULL,
        0
    ); // Communication
}

void otherInit() {
    // Other objects initialization

    // Serial Port
    Serial.begin(115200);
    delay(10);
}


void setup() {
    // Initialization
    otherInit();

    // Initialize tasks
    taskInit();
}

void loop() {
    // This task will not be used any more
    vTaskDelete(NULL);
}
