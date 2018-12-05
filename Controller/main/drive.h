#ifndef _DRIVE_H_
#define _DRIVE_H_

// status of motor
#define DRIVE_STOP              0
#define DRIVE_FORWARD           1
#define DRIVE_BACKWARD          2

// some constants
#define DRIVE_MAX_COUNT         10
#define DRIVE_MAX_BUSY_COUNT    250

// Functions
void driveTask(void *pvParameters);

void driveInit(void);
void drive(void);


#endif