#include <Arduino.h>
#include "scan.h"
#include "port.h"
#include "taskDelay.h"


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
}

void scan(void)
{
    int voltage1, voltage2;
    int current;

    // Read 2 ADC ports
    voltage1 = analogRead(VM_PORT);
    voltage2 = analogRead(CM_PORT);
    Serial.print("Voltage: ");
    Serial.print(voltage1);
    Serial.print(", ");
    Serial.println(voltage2);

    // TODO: check the values of voltage and current
    digitalWrite(RELAY_PORT, HIGH);

    delay(DELAY_SCAN);
}