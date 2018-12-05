#ifndef _SCAN_H_
#define _SCAN_H_

#define SCAN_ADC_MAX    4095    // This means 3.51V

#define SCAN_V_MAX      12500   // This means 12.5V
#define SCAN_V_MIN      8000    // This means 8V
#define SCAN_A_MAX      500     // This means 0.5A


void scanTask(void *pvParameters);

void scanInit(void);
void scan(void);


#endif