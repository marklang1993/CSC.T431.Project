#include <Arduino.h>
#include "led.h"

#define LED_PORT 21


void ledTask(void *pvParameters)
{
    ledInit();
    while(1){ led(); }
}

void ledInit(void)
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_PORT, OUTPUT);
}


void led(void)
{
    digitalWrite(LED_PORT, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                    // wait for a second
    digitalWrite(LED_PORT, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                    // wait for a second
}