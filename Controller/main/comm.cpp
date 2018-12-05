#include <Arduino.h>
#include <WiFi.h>
#include "ipc.h"
#include "comm.h"
#include "taskDelay.h"

// Queues for ipc
extern QueueHandle_t sendQueueDrive;
extern QueueHandle_t recvQueueDrive;
extern QueueHandle_t sendQueueScan;
extern QueueHandle_t recvQueueScan;
extern QueueHandle_t sendQueueI2C;

// WIFI credential
static const char* ssid = "TP-LINK_D9D564";
static const char* password = "advanced123";

// Server & Client
static WiFiServer server(80);
static WiFiClient client;

// Private functions
static void sendrecv(WiFiClient* pClient);
static void collectData(struct commSendData* pCommSendData);
static bool sendCommand(struct msgCommand* pMsgComm);
static void sendCommandByCommunication(struct commRecvData* pCommRecvData);

void commTask(void *pvParameters)
{
    commInit();
    while(1){ comm(); }
}

void commInit(void)
{
    // Init. WIFI object
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.print(" .");

    WiFi.begin(ssid, password);

    // Wait for WIFI connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(DELAY_COMM_WIFI_CONNECT);
        Serial.print(".");
    }

    // WIFI connected, print config info.
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Start the server
    server.begin();
}

void comm(void)
{
    if (client) {
        // If client is existed
        if (client.connected()) {
            // If client is connected
            Serial.println("Connected to client");
            // Communicate
            sendrecv(&client);
            // Stop - FIN
            client.stop();
            delay(DELAY_COMM_SENDRECV);
        }
    }
    else {
        // If client does not existed
        do {
            // Wait for the client
            // Serial.println("Wait for client");

            client = server.available();
            delay(DELAY_COMM_CONNECT);
        } while(!client);
    }
}

static void sendrecv(WiFiClient* pClient)
{
    struct commSendData sendData;
    struct commRecvData recvData;

    // Collect all data from SCAN and DRIVE
    collectData(&sendData);

    // Send data
    pClient->println(
        String(sendData.voltage) + "," +
        String(sendData.current) + "," +
        String(sendData.relayStatus) + "," +
        String(sendData.ratio) + "," +
        String(sendData.motorStatus) + "," +
        String(sendData.temperature)
        );

    // Receive data
    String stringReceived = pClient->readStringUntil('\n');
    // TEST
    Serial.print("recvData: ");
    Serial.println(stringReceived);

    // Convert data from stringReceived to "recvData"
    int indexOfComma = stringReceived.indexOf(',');
    String commandString = stringReceived.substring(0, indexOfComma);
    String subCommandString = stringReceived.substring(indexOfComma + 1);
    recvData.command = commandString.toInt();
    recvData.subCommand = subCommandString.toInt();

    // TEST
    Serial.print("Command: ");
    Serial.println(recvData.command);
    Serial.print("SubCommand: ");
    Serial.println(recvData.subCommand);

    // Dispatch the received command
    sendCommandByCommunication(&recvData);
}

static void collectData(struct commSendData* pCommSendData)
{
    static struct msgDrive msgDriveStatus;
    static struct msgScan msgScanStatus;
    static struct msgI2C msgI2CStatus;

    // Gather info. from Drive Task
    if (sendQueueDrive != NULL) {
        if (uxQueueSpacesAvailable(sendQueueDrive) < IPC_QUEUE_SIZE) {
            // Some message are ready to pick up
            xQueueReceive(sendQueueDrive, (void *)&msgDriveStatus, (TickType_t)0);
        }
    }
    // Gather info. from Scan Task
    if (sendQueueScan != NULL) {
        if (uxQueueSpacesAvailable(sendQueueScan) < IPC_QUEUE_SIZE) {
            // Some message are ready to pick up
            xQueueReceive(sendQueueScan, (void *)&msgScanStatus, (TickType_t)0);
        }
    }

    // Gather info. from I2C Task
    if (sendQueueI2C != NULL) {
        if (uxQueueSpacesAvailable(sendQueueI2C) < IPC_QUEUE_SIZE) {
            // Some message are ready to pick up
            xQueueReceive(sendQueueI2C, (void *)&msgI2CStatus, (TickType_t)0);
        }
    }

    // Prepare sending data for communication
    pCommSendData->voltage = msgScanStatus.voltage;
    pCommSendData->current = msgScanStatus.current;
    pCommSendData->relayStatus = msgScanStatus.relayStatus;
    pCommSendData->ratio = msgDriveStatus.ratio;
    pCommSendData->motorStatus = msgDriveStatus.motorStatus;
    pCommSendData->temperature = msgI2CStatus.temperature;

    // TEST
    Serial.print("Voltage: ");
    Serial.println(msgScanStatus.voltage);
    Serial.print("Current: ");
    Serial.println(msgScanStatus.current);
    Serial.print("Relay: ");
    Serial.println(msgScanStatus.relayStatus);
    Serial.print("Ratio: ");
    Serial.println(msgDriveStatus.ratio);
    Serial.print("Motor Status: ");
    Serial.println(msgDriveStatus.motorStatus);
    Serial.print("Temperature: ");
    Serial.println(msgI2CStatus.temperature);
}

static bool sendCommand(struct msgCommand* pMsgComm)
{
    if (pMsgComm->type != IPC_R_SETRELAY) {
        // Command for Drive
        if (recvQueueDrive != NULL) {
            if (uxQueueSpacesAvailable(recvQueueDrive) == IPC_QUEUE_SIZE) {
                // Ready to send command
                xQueueSend(recvQueueDrive, (void *)pMsgComm, (TickType_t)0);
                return true;
            }
        }
    } else {
        // Command for Scan
        if (recvQueueScan != NULL) {
            if (uxQueueSpacesAvailable(recvQueueScan) == IPC_QUEUE_SIZE) {
                // Ready to send command
                xQueueSend(recvQueueScan, (void *)pMsgComm, (TickType_t)0);
                return true;
            }
        }
    }
    return false;
}

static void sendCommandByCommunication(struct commRecvData* pCommRecvData)
{
    struct msgCommand msgComm;

    switch(pCommRecvData->command)
    {
        case 0:     // Command: Empty
            break;
        case 1:     // Command: Stop
            msgComm.type = IPC_R_STOP;
            msgComm.value = 0;
            sendCommand(&msgComm);
            break;
        case 2:     // Command: Forward
            msgComm.type = IPC_R_FORWARD;
            msgComm.value = 0;
            sendCommand(&msgComm);
            break;
        case 3:     // Command: Backward
            msgComm.type = IPC_R_BACKWARD;
            msgComm.value = 0;
            sendCommand(&msgComm);
            break;
        case 4:     // Command: SetDutyRatio
            msgComm.type = IPC_R_SETRATIO;
            msgComm.value = pCommRecvData->subCommand;
            sendCommand(&msgComm);
            break;
        case 5:     // Command: SetRelay
            msgComm.type = IPC_R_SETRELAY;
            msgComm.value = pCommRecvData->subCommand;
            sendCommand(&msgComm);
            break;
        default:    // Unknown command
            break;
    }
}