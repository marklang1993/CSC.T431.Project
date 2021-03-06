#include "DHT12.h"  // External library

#include "i2c.h"
#include "ipc.h"
#include "taskDelay.h"

// Message Queues
QueueHandle_t sendQueueI2C = NULL;

// DHT12 Module
static DHT12 dht12;

void i2cTask(void *pvParameters)
{
    i2cInit();
    while(1){ i2c(); }
}

void i2cInit(void)
{
    // init. I2C
    Wire.begin();

    // Init. queues for ipc
    sendQueueI2C = xQueueCreate(IPC_QUEUE_SIZE, sizeof(struct msgI2C));
}

void i2c(void)
{
    float rawTemperature;
    int temperature = 0;

    // Read raw temperature and convert it to temperature
    rawTemperature = dht12.readTemperature(CELSIUS);
    temperature = (int)(rawTemperature * 100.0f);

    // # Report data from I2C
    struct msgI2C msgStatus;
    if (uxQueueSpacesAvailable(sendQueueI2C) == IPC_QUEUE_SIZE)
    {
        // Sending queue is empty -> collect data
        msgStatus.temperature = temperature;
        // Send
        xQueueSend(sendQueueI2C, (void *)&msgStatus, (TickType_t)0);
    }

    delay(DELAY_I2C);
}
