#include <M5stack.h>

#include "screen1.h"
#include "ui.h"
#include "colorscheme.h"
#include "comm.h"
#include "ipc.h"

extern QueueHandle_t recvQueueDisplay;

bool isRelay = false;
static String vars[6] = {"Voltage", "Current", "Relay", "Motor", "Power", "Temperature"};
static String unitArr[] = {"V", "A", "C"};

static void receiveInfo(int* pData);

void showText()
{
    // Get data from IPC
    static int data[6] = {0, 0, 0, 0, 0, 0};
    receiveInfo(data);

    for (int i = 0; i < 6; i++)
    {
        char param;
        M5.Lcd.setCursor(20, i * 33 + 30);
        M5.Lcd.setFreeFont(FSS12);
        M5.Lcd.setTextColor(TEXT_DARK);
        M5.Lcd.print(vars[i] + ": ");
        switch (i)
        {
        case 0:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            int f = data[i] / 1000;
            M5.Lcd.print(String(f) + '.' + (data[i] - f * 1000) + " " + unitArr[i]);
            break;
        }
        case 1:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            int f = data[i] / 1000;
            M5.Lcd.print(String(f) + '.' + (data[i] - f * 1000) + " " + unitArr[i]);
            break;
        }
        case 2:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            isRelay = data[i] == 0;
            M5.Lcd.print(isRelay ? "Close" : "Open");
            break;
        }
        case 3:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            switch (data[i])
            {
            case 0:
                M5.Lcd.print("Stop");
                break;
            case 1:
                M5.Lcd.print("Forward");
                break;
            case 2:
                M5.Lcd.print("Backword");
                break;
            default:
                M5.Lcd.print("Error");
                break;
            }
            break;
        }
        case 5:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            int f = data[i] / 100;
            M5.Lcd.print(String(f) + '.' + (data[i] - f * 100) + " " + unitArr[i - 3]);
            break;
        }
        case 4:
        {
            M5.Lcd.setFreeFont(FSSO12);
            M5.Lcd.setTextColor(TEXT_LIGHT);
            M5.Lcd.print(String(data[i]) + "%");
        }
        default:
        {
            break;
        }
        }
    }
    M5.Lcd.setTextColor(TEXT_LIGHT);
    M5.Lcd.setFreeFont(FSS9);
    M5.Lcd.setCursor(13, 226);
    M5.Lcd.print("Commands");

    for (int i = 0; i < 5; i++)
    {
        M5.Lcd.fillRect(0, i * 33 + 38, 320, 2, BG_DARK);
    }
}

void startRender1()
{
    drawBackground();
    drawButtonBar();
    showText();
    drawRadioOn(160, 75);
}

static void receiveInfo(int* pData)
{
    struct msgDisplay msgComm;

    if (recvQueueDisplay != NULL)
    {
        // Queue is ready
        if (uxQueueSpacesAvailable(recvQueueDisplay) < IPC_QUEUE_SIZE) {
            // Pick up message from "Comm"
            xQueueReceive(recvQueueDisplay, (void *)&msgComm, (TickType_t)0);

            // Put value
            *pData = msgComm.voltage;
            *(pData + 1) = msgComm.current;
            *(pData + 2) = msgComm.relayStatus;
            *(pData + 3) = msgComm.motorStatus;
            *(pData + 4) = msgComm.ratio;
            *(pData + 5) = msgComm.temperature;
        }
    }
}