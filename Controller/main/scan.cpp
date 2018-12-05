#include <Arduino.h>
#include "scan.h"
#include "port.h"
#include "taskDelay.h"
#include "ipc.h"

#define READ_ADC(x) ((((float)analogRead(x)) / ((float)SCAN_ADC_MAX)) * 3.51f * 1000.f)

// Relay controlling status
static bool relayStatus = true;

// Message Queues
QueueHandle_t sendQueueScan = NULL;
QueueHandle_t recvQueueScan = NULL;

void scanTask(void *pvParameters)
{
    scanInit();
    while(1){ scan(); }
}

void scanInit(void)
{
    /* 
      * SHUT UP THE SPEAKER.
      * Otherwise, the speaker will keep beeping when analogRead()
    */
    dacWrite(SPEAKER_PORT, 0);

    // init. RELAY_PORT as OUTPUT to control relay
    pinMode(RELAY_PORT, OUTPUT);
    digitalWrite(RELAY_PORT, LOW); // turn off relay until everything is set up

    // init. VM & CM ports for voltage and current measurement
    pinMode(VM_PORT, INPUT);
    pinMode(CM_PORT, INPUT);

    // Init. queues for ipc
    sendQueueScan = xQueueCreate(IPC_QUEUE_SIZE, sizeof(struct msgScan));
    recvQueueScan = xQueueCreate(IPC_QUEUE_SIZE, sizeof(struct msgCommand));
}

void scan(void)
{
    float voltage1, voltage2 = 0.0f;
    float voltageDiff;
    int voltage;
    int current;

    // Read 2 ADC ports
    voltage1 = READ_ADC(VM_PORT);
    voltage2 = READ_ADC(CM_PORT);

    // Calculate voltage
    voltage = (int)(voltage1 * 6.0f);
    
    // Calculate current
    voltageDiff = voltage1 - voltage2;
    if (voltageDiff < 0.0f) voltageDiff = 0.0f;
    current = (int)voltageDiff;

    // # Message receiving
    struct msgCommand msgCommand;
    if (uxQueueSpacesAvailable(recvQueueScan) != IPC_QUEUE_SIZE)
    {
        // Receiving queue is not empty -> get command
        xQueueReceive(recvQueueScan, (void *)&msgCommand, (TickType_t)0);
        // Execute the command
        int commandType = msgCommand.type;
        int commandValue = msgCommand.value;
        switch(commandType)
        {
            case IPC_R_SETRELAY:
                relayStatus = commandValue == 1 ? true : false;
                break;

            default:
                // NULL command
                break;
        }
    } else {
        // Receiving queue is empty -> NO command
        if (relayStatus == true)
        {
            // PROTECT FUNCTIONALITY -> Cut off the relay
            if (voltage < SCAN_V_MIN || voltage > SCAN_V_MAX) {
                // Voltage is out of range [SCAN_V_MIN, SCAN_V_MAX]
                relayStatus = false;
            } else if (current > SCAN_A_MAX) {
                // Current is more than SCAN_A_MAX
                relayStatus = false;
            } else {
                // Voltage and current is in the right range
                relayStatus = true;
            }
        }
        // Since the relay is turned off due to some reasons of safety,
        // it should not be automatically turned on.
    }

    // Set the relay
    digitalWrite(RELAY_PORT, relayStatus ? HIGH : LOW);

    // # Report status of the circuit
    struct msgScan msgStatus;
    if (uxQueueSpacesAvailable(sendQueueScan) == IPC_QUEUE_SIZE)
    {
        // Sending queue is empty -> collect status
        msgStatus.voltage = voltage;
        msgStatus.current = current;
        msgStatus.relayStatus = relayStatus ? 1 : 0;
        // Send
        xQueueSend(sendQueueScan, (void *)&msgStatus, (TickType_t)0);
    }

    delay(DELAY_SCAN);
}