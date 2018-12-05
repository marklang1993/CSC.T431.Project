#include "led.h"
#include "i2c.h"
#include "comm.h"
#include "scan.h"
#include "drive.h"

// Task Descriptor
typedef struct {
    TaskFunction_t pvTaskCode;
    const char *const pcName;
    const uint32_t usStackDepth;
    void *const pvParameters;
    UBaseType_t uxPriority;
    TaskHandle_t *const pvCreatedTask;
    const BaseType_t xCoreID;
} taskDescriptor;

// Task Descriptor Table
static taskDescriptor tdt[] {
    {ledTask, "LED", 4096, NULL, 1, NULL, 0}, // LED
    {commTask, "Communication", 16384, NULL, 2, NULL, 0}, // Communication
    {i2cTask, "I2C", 4096, NULL, 3, NULL, 0}, // I2C
    {scanTask, "Scan", 8192, NULL, 4, NULL, 0}, // Circuit Scan
    {driveTask, "Drive", 8192, NULL, 5, NULL, 0} // Motor drive
    
};

void taskInit() {
    // All tasks should be initialized here
    for (int i = 0; i < sizeof(tdt)/sizeof(taskDescriptor); ++i)
    {
        xTaskCreatePinnedToCore(
            tdt[i].pvTaskCode,
            tdt[i].pcName,
            tdt[i].usStackDepth,
            tdt[i].pvParameters,
            tdt[i].uxPriority,
            tdt[i].pvCreatedTask,
            tdt[i].xCoreID
        );
    }
}

void otherInit() {
    // Other objects should be initialzed here

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
