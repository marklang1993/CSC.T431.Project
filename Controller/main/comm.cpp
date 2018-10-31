#include <Arduino.h>
#include <WiFi.h>

#include "comm.h"

// WIFI credential
static const char* ssid = "TP-LINK_D9D564";
static const char* password = "advanced123";

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
    Serial.print("WIFI Idle at: ");
    Serial.println(millis());
    delay(5000);
}