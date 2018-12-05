#include <Arduino.h>
#include "led.h"
#include "port.h"
#include "taskDelay.h"


void ledTask(void *pvParameters)
{
    ledInit();
    while(1){ led(); }
}

void ledInit(void)
{
    // initialize digital pin LED_PORT as an output.
    pinMode(LED_PORT, OUTPUT);
}


void led(void)
{
    digitalWrite(LED_PORT, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(DELAY_LED);               // wait for a second
    digitalWrite(LED_PORT, LOW);    // turn the LED off by making the voltage LOW
    delay(DELAY_LED);               // wait for a second
}