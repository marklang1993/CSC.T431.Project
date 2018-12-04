#include <Arduino.h>
#include <WiFi.h>
#include "ipc.h"
#include "comm.h"

// Queues for ipc
extern QueueHandle_t sendQueueDrive;
extern QueueHandle_t recvQueueDrive;
extern QueueHandle_t sendQueueScan;
extern QueueHandle_t recvQueueScan;

// WIFI credential
static const char* ssid = "TP-LINK_D9D564";
static const char* password = "advanced123";

// Private functions
static void sendrecv(WiFiClient* pClient);
static int collectData(void);
static bool sendCommand(struct msgCommand* pMsgComm);

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
        delay(500);
        Serial.print(".");
    }

    // WIFI connected, print config info.
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void comm(void)
{
    static WiFiServer server(80);
    static WiFiClient client;

    if (client) {
        // If client is existed
        if (client.connected()) {
            // If client is connected
            Serial.println("Connected to client");
            sendrecv(&client);
            delay(500);
        }
    }
    else {
        // If client does not existed
        do {
            // Wait for the client

            // Serial.print("Wait for client");

            int test = collectData();
            if (test % 200 == 0) {
                struct msgCommand msgComm;
                msgComm.type = IPC_R_SETRELAY;
                msgComm.value = 1;
                while (!sendCommand(&msgComm));
            } else if (test % 100 == 0) {
                struct msgCommand msgComm;
                msgComm.type = IPC_R_SETRELAY;
                msgComm.value = 0;
                while (!sendCommand(&msgComm));
            }

            client = server.available();
            delay(100);
        } while(!client);
    }
}

static void sendrecv(WiFiClient* pClient)
{
    char sendData[256];
    char recvData[128];

    pClient->write(sendData);
}

static int collectData(void)
{
    static struct msgDrive msgDriveStatus;
    static struct msgScan msgScanStatus;

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
    return msgScanStatus.voltage;
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