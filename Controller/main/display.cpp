#include <Arduino.h>
#include "display.h"


void displayTask(void *pvParameters)
{
    displayInit();
    while(1){ display(); }
}


void displayInit(void)
{
  ;
}

void display(void)
{
  ;
}