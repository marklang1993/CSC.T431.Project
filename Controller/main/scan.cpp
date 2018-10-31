#include <Arduino.h>
#include "scan.h"


void scanTask(void *pvParameters)
{
    scanInit();
    while(1){ scan(); }
}

void scanInit(void)
{
  ;
}

void scan(void)
{
  ;
}