#include <M5Stack.h>

#include "display.h"
#include "screen1.h"
#include "screen2.h"
#include "taskDelay.h"
#include "ipc.h"

static bool isScreen1;

QueueHandle_t sendQueueDisplay = NULL;
QueueHandle_t recvQueueDisplay = NULL;

static void displayInit()
{
    M5.begin();
    startRender1();
    isScreen1 = true;

    // Init. queues for ipc
    sendQueueDisplay = xQueueCreate(IPC_QUEUE_SIZE, sizeof(struct msgCommand));
    recvQueueDisplay = xQueueCreate(IPC_QUEUE_SIZE, sizeof(struct msgDisplay));
}

static void displayLoop()
{
    // Render
    if (isScreen1)
        startRender1();

    if (M5.BtnA.wasPressed())
    {
        isScreen1 = !isScreen1;
        if (!isScreen1)
            startRender2();
    }

    if (M5.BtnB.wasPressed())
    {
        if (isScreen1)
            ;
        else
            next();
    }
    if (M5.BtnC.wasPressed())
    {
        if (isScreen1)
            ;
        else
            ok();
    }

    M5.update();
}

void displayTask(void *pvParameters)
{
    displayInit();
    while (1)
    {
        displayLoop();
        delay(DELAY_DISPLAY);
    }
}