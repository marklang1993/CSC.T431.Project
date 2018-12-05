#include <M5stack.h>

#include "ui.h"
#include "screen2.h"
#include "colorscheme.h"
#include "comm.h"
#include "ipc.h"

extern QueueHandle_t sendQueueDisplay;
extern bool isRelay;

static String commands[5] = {"Stop", "Forward", "Backward", "SetPower", "SetRelay"};
static int selected = 0;
static int percentage = 0;
static bool isDataSelecting = false;

static void sendCommand(int command, int value);

void updateRelay()
{
    Serial.print("updateRelay: ");
    Serial.println(isRelay);
    if (isRelay)
    {
        drawRadioOn(200, 160);
    }
    else
    {
        drawRadioOff(200, 160);
    }
}

void update2(int selected)
{
    // Draw static UI
    M5.Lcd.clear();
    drawBackground();
    drawButtonBar();
    M5.Lcd.fillRect(0, 65, 320, 5, BG_DARK);
    M5.Lcd.fillRect(0, 135, 320, 5, BG_DARK);
    M5.Lcd.setTextColor(TEXT_LIGHT);
    M5.Lcd.setFreeFont(FSS9);
    M5.Lcd.setCursor(35, 226);
    M5.Lcd.print("Status");
    M5.Lcd.setCursor(140, 226);
    M5.Lcd.print("Next");
    M5.Lcd.setCursor(240, 226);
    M5.Lcd.print("OK");
    M5.Lcd.setFreeFont(FSS18);
    if (selected < 3)
    {
        for (int i = 0; i < 3; i++)
        {
            M5.Lcd.setTextColor(TEXT_LIGHT);
            if (selected == i)
            {
                M5.Lcd.fillRect(0, 70 * i, 320, 65, BG_DARK);
                M5.Lcd.setTextColor(FRONT_LIGHT);
            }
            M5.Lcd.setCursor(20, i * 70 + 45);
            M5.Lcd.print(commands[i]);
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            M5.Lcd.setTextColor(TEXT_LIGHT);
            if (selected - 2 == i)
            {
                M5.Lcd.fillRect(0, 70 * i, 320, 65, BG_DARK);
                M5.Lcd.setTextColor(FRONT_LIGHT);
            }
            M5.Lcd.setCursor(20, i * 70 + 45);
            M5.Lcd.print(commands[i + 2]);
        }
        updateRelay();
    }
}

void startRender2()
{
    update2(selected);
}

void updateDataSelection(int per)
{
    M5.Lcd.clear();
    drawBackground();
    drawButtonBar();
    M5.Lcd.setTextColor(TEXT_LIGHT);
    M5.Lcd.setFreeFont(FSS18);
    M5.Lcd.drawString(String(per) + "%", 100, 100);
    M5.Lcd.setFreeFont(FSS9);
    M5.Lcd.setCursor(35, 226);
    M5.Lcd.print("Status");
    M5.Lcd.setCursor(140, 226);
    M5.Lcd.print("10%");
    M5.Lcd.setCursor(240, 226);
    M5.Lcd.print("OK");
}

void next()
{
    if (isDataSelecting)
    {
        if (percentage == 100)
        {
            percentage = 0;
        }
        else
        {
            percentage += 10;
        }
        updateDataSelection(percentage);
    }

    else
    {
        if (selected < 4)
        {
            selected++;
        }
        else
        {
            selected = 0;
        }
        update2(selected);
    }
}

void ok()
{
    if (isDataSelecting)
    {
        update2(selected);
        isDataSelecting = false;

        // SEND COMMAND
        // DutyRatio
        sendCommand(COMM_SETRATIO, percentage);
    }
    else
    {
        if (selected < 3)
        {
            // SEND COMMAND
            // Stop(0), Forward(1), Backward(2)
            switch (selected)
            {
                case 0:
                    sendCommand(COMM_STOP, 0);
                    break;

                case 1:
                    sendCommand(COMM_FORWARD, 0);
                    break;
                
                case 2:
                    sendCommand(COMM_BACKWARD, 0);
                    break;
                
                default:
                    break;
            }
        }
        else if (selected == 3)
        {
            isDataSelecting = true;
            updateDataSelection(percentage);
        }
        else if (selected == 4)
        {
            isRelay = !isRelay;
            updateRelay();
            // SEND COMMAND
            // Relay
            sendCommand(COMM_SETRELAY, isRelay ? 1 : 0);            
        }
    }
}

static void sendCommand(int command, int value)
{
    struct msgCommand msgComm;

    if (sendQueueDisplay != NULL)
    {
        // Queue is ready
        if (uxQueueSpacesAvailable(sendQueueDisplay) == IPC_QUEUE_SIZE) {
            // Ready to send command
            msgComm.type = command;
            msgComm.value = value;
            // Send
            xQueueSend(sendQueueDisplay, (void *)&msgComm, (TickType_t)0);
        }
    }
}
