#include <Arduino.h>
#include <WiFi.h>

#include "comm.h"

// WIFI credential
static const char* ssid = "TP-LINK_D9D564";
static const char* password = "advanced123";

// Private functions
static void sendrecv(WiFiClient* pClient);

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
            Serial.print("Wait for client");
            client = server.available();
            delay(2000);
        } while(!client);
    }
}

static void sendrecv(WiFiClient* pClient)
{
    char sendData[256];
    char recvData[256];

    pClient->write(data);
}