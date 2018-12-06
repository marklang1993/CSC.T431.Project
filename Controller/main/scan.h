#ifndef _SCAN_H_
#define _SCAN_H_

#define SCAN_ADC_MAX    4095    // This means 3.51V

#define SCAN_V_MAX      13000   // This means 13V
#define SCAN_V_MIN      8000    // This means 8V
#define SCAN_A_MAX      2000    // This means 2A


void scanTask(void *pvParameters);

void scanInit(void);
void scan(void);


#endif