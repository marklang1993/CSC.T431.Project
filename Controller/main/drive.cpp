#include <Arduino.h>
#include "drive.h"


void driveTask(void *pvParameters)
{
    driveInit();
    while(1){ drive(); }
}

void driveInit(void)
{
  ;
}

void drive(void)
{
  ;
}