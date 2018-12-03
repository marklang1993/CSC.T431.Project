#include <Arduino.h>
#include <WiFi.h>
#include "ipc.h"
#include "comm.h"


// Queues for ipc
extern QueueHandle_t sendQueueDrive;
extern QueueHandle_t recvQueueDrive;

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

    // TESTING PURPOSE
    struct msgCommand msgComm;
    msgComm.type = IPC_R_SETRATIO;
    msgComm.value = 100;
    while (!sendCommand(&msgComm));

    msgComm.type = IPC_R_FORWARD;
    while (!sendCommand(&msgComm));
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

            // TESTING PURPOSE
            if (collectData() == 100)
            {
                struct msgCommand msgComm;
                msgComm.type = IPC_R_BACKWARD;
                while (!sendCommand(&msgComm));
            }

            client = server.available();
            delay(10);
        } while(!client);
    }
}

static void sendrecv(WiFiClient* pClient)
{
    char sendData[256];
    char recvData[256];

    pClient->write(sendData);
}

static int collectData(void)
{
    struct msgDrive msgDriveStatus;

    // Gather info. from Drive Task
    if (sendQueueDrive != NULL) {
        if (uxQueueSpacesAvailable(sendQueueDrive) < IPC_QUEUE_SIZE) {
            // Some message are ready to pick up
            xQueueReceive(sendQueueDrive, (void *)&msgDriveStatus, (TickType_t)0);
            Serial.print("Ratio: ");
            Serial.println(msgDriveStatus.ratio);
            Serial.print("Motor Status: ");
            Serial.println(msgDriveStatus.motorStatus);

            return msgDriveStatus.ratio;
        }
    }
    // TODO: Gather info. from Scan Task
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
        // TODO
    }


    return false;
}