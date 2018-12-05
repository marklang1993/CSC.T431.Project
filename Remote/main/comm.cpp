#include <M5Stack.h>
#include <WiFi.h>

#include "taskDelay.h"
#include "comm.h"
#include "ipc.h"
#include "taskDelay.h"

extern QueueHandle_t sendQueueDisplay;
extern QueueHandle_t recvQueueDisplay;

// WIFI credential
static const char* ssid = "TP-LINK_D9D564";
static const char* password = "advanced123";
static const char* host = "192.168.1.101";
static const int port = 80;

static void split(String input, int* pData);

static void commInit()
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
}

static void commLoop()
{
    WiFiClient client;
    // Try to connect to the server
    while (!client.connect(host, port)) {
        Serial.println("Connection failed");
        delay(DELAY_COMM_CONNECT);
    }

    // // Check the client is available
    // if (client.available() == 0)
    // {
    //     Serial.println("Server is not available");
    //     delay(DELAY_COMM_CONNECT);
    //     return;
    // }

    // Receive data from server
    String stringReceived = client.readStringUntil('\n');
    // TEST
    Serial.print("recvData: ");
    Serial.println(stringReceived);

    // Send system infomation to recvQueueDisplay
    if (recvQueueDisplay != NULL)
    {
        // Queue is ready
        if (uxQueueSpacesAvailable(recvQueueDisplay) == IPC_QUEUE_SIZE) {
            // Ready to send system information
            struct msgDisplay msgSend;
            int data[6];
            split(stringReceived, data);        
            // Put data
            msgSend.voltage = data[0];
            msgSend.current = data[1];
            msgSend.relayStatus = data[2];
            msgSend.motorStatus = data[4];
            msgSend.ratio = data[3];
            msgSend.temperature = data[5];
            // Send to UI - Screen 1
            xQueueSend(recvQueueDisplay, (void *)&msgSend, (TickType_t)0);
        }
    }

    // Check is there any command to send
    struct msgCommand msgRecv;
    if (sendQueueDisplay != NULL)
    {
        // Queue is ready
        if (uxQueueSpacesAvailable(sendQueueDisplay) < IPC_QUEUE_SIZE) {
            // Pick up message from UI - Screen 2
            xQueueReceive(sendQueueDisplay, (void *)&msgRecv, (TickType_t)0);
            // Send to Controller via WIFI
            client.println(String(msgRecv.type) + "," + String(msgRecv.value));
        } else {
            // Send EMPTY command
            client.println("0,0");
        }
    } else {
        // Send EMPTY command
        client.println("0,0");
    }
    delay(DELAY_COMM_SENDRECV);
}

void commTask(void *pvParameters)
{
    commInit();
    while (1) {
        commLoop();
    }
}

static void split(String input, int* pData)
{
    String tmp = "";
    int j = 0;

    for (int i = 0; i < input.length(); ++i)
    {
        if (input[i] == ',') {
            // Find splitor
            *(pData + j) = tmp.toInt();
            j++;
            tmp = "";
            continue;

        } else {
            tmp.concat(input[i]);
        }
    }
    // Convert the last token
    *(pData + j) = tmp.toInt();
}