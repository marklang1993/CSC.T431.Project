#include <Arduino.h>
#include "drive.h"
#include "port.h"
#include "taskDelay.h"

// motor status
static int motorStatus = DRIVE_STOP;
static int pwmPercentage = 0; // 0 ~ DRIVE_MAX_COUNT

void driveTask(void *pvParameters)
{
    driveInit();
    while(1){ drive(); }
}

void driveInit(void)
{
    // init. all ports that control the motor
    pinMode(IN1_PORT, OUTPUT);
    pinMode(IN2_PORT, OUTPUT);
    pinMode(PWM_PORT, OUTPUT);

    // turn off all outputs
    digitalWrite(IN1_PORT, LOW);
    digitalWrite(IN2_PORT, LOW);
    digitalWrite(PWM_PORT, LOW);

    // set motor status for TESTING
    motorStatus = DRIVE_FORWARD;
    pwmPercentage = 1;
}

static void setPWM(void)
{
    // Set percentage of PWM
    switch(motorStatus)
    {
        case DRIVE_FORWARD:
            // Forward
            digitalWrite(IN1_PORT, HIGH);
            digitalWrite(IN2_PORT, LOW);
            break;

        case DRIVE_BACKWARD:
            // Backward
            digitalWrite(IN1_PORT, LOW);
            digitalWrite(IN2_PORT, HIGH);
            break;

        default:
            // Stop
            digitalWrite(IN1_PORT, LOW);
            digitalWrite(IN2_PORT, LOW);
            break;
    }
}

static void outputPWM(void)
{
    static int counter = 0;

    // Output PWM signal
    if (counter < pwmPercentage) {
        // on
        digitalWrite(PWM_PORT, HIGH);

    } else {
        // off
        digitalWrite(PWM_PORT, LOW);
    }
    ++counter;

    // reset counter
    if (counter >= DRIVE_MAX_COUNT) {
      counter = 0;
    }
}

void drive(void)
{
    setPWM();
    outputPWM();

    delay(DELAY_DRIVE);
}