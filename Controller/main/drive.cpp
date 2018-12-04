#include <Arduino.h>
#include "drive.h"
#include "port.h"
#include "taskDelay.h"
#include "ipc.h"

// motor controlling status
static int motorStatus = DRIVE_STOP; // Current motor status
static int motorStatusLast = DRIVE_STOP;
static int pwmPercentage = 0; // 0 ~ DRIVE_MAX_COUNT
static int pwmPercentageTarget = 0;
static int busyCount = 0;

// Message Queues
QueueHandle_t sendQueueDrive = NULL;
QueueHandle_t recvQueueDrive = NULL;

// Private functions
static void changeMotorStatus(int newStatus);

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

    // Init. queues for ipc
    sendQueueDrive = xQueueCreate(IPC_QUEUE_SIZE, sizeof(struct msgDrive));
    recvQueueDrive = xQueueCreate(IPC_QUEUE_SIZE, sizeof(struct msgCommand));
}

static void changeMotorStatus(int newStatus)
{
    motorStatusLast = motorStatus;
    motorStatus = newStatus;
}

static inline void checkSpeedChange(void)
{
    if (pwmPercentage < pwmPercentageTarget) {
        // Accelarating is required
        if (busyCount < DRIVE_MAX_BUSY_COUNT) 
        {
            ++busyCount;
        } else {
            busyCount = 0;
            ++pwmPercentage;
        }

    } else if (pwmPercentage > pwmPercentageTarget) {
        // Decelarating is required
        if (busyCount < DRIVE_MAX_BUSY_COUNT) 
        {
            ++busyCount;
        } else {
            busyCount = 0;
            --pwmPercentage;
        }
    }
}

static void setPWM(void)
{
    // Set percentage of PWM
    switch(motorStatus)
    {
        case DRIVE_FORWARD:
            // Forward
            if (motorStatusLast == DRIVE_BACKWARD) {
                // Operate in an opposite direction
                if (pwmPercentage != 0) {
                    // Motor does not stop completely
                    if (busyCount < DRIVE_MAX_BUSY_COUNT)
                    {
                        ++busyCount;
                    } else {
                        busyCount = 0;
                        --pwmPercentage;
                    }
                } else {
                    // Motor stop completely
                    changeMotorStatus(DRIVE_FORWARD);   // Change again
                }
            }
            if (motorStatusLast != DRIVE_BACKWARD) {
                // Operate in the same direction OR start from stopping
                digitalWrite(IN1_PORT, HIGH);
                digitalWrite(IN2_PORT, LOW);

                checkSpeedChange();
                // Otherwise, keep the same speed
            }
            break;

        case DRIVE_BACKWARD:
            // Backward
            if (motorStatusLast == DRIVE_FORWARD) {
                // Operate in an opposite direction
                if (pwmPercentage != 0) {
                    // Motor does not stop completely
                    if (busyCount < DRIVE_MAX_BUSY_COUNT)
                    {
                        ++busyCount;
                    } else {
                        busyCount = 0;
                        --pwmPercentage;
                    }
                } else {
                    // Motor stop completely
                    changeMotorStatus(DRIVE_BACKWARD);   // Change again
                }
            }
            if (motorStatusLast != DRIVE_FORWARD) {
                // Operate in the same direction OR start from stopping
                digitalWrite(IN1_PORT, LOW);
                digitalWrite(IN2_PORT, HIGH);

                checkSpeedChange();
                // Otherwise, keep the same speed
            }
            break;

        default:
            // Stop
            digitalWrite(IN1_PORT, LOW);
            digitalWrite(IN2_PORT, LOW);
            // Do some possible cleaning
            pwmPercentage = 0;
            busyCount = 0;

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
    // Report status of the motor
    struct msgDrive msgStatus;
    if (uxQueueSpacesAvailable(sendQueueDrive) == IPC_QUEUE_SIZE)
    {
        // Sending queue is empty -> collect status
        msgStatus.ratio = pwmPercentage * 10;
        msgStatus.motorStatus = motorStatus;
        // Send
        xQueueSend(sendQueueDrive, (void *)&msgStatus, (TickType_t)0);
    }
    
    // Drive
    setPWM();
    outputPWM();

    // Message receiving
    struct msgCommand msgCommand;
    if (uxQueueSpacesAvailable(recvQueueDrive) != IPC_QUEUE_SIZE)
    {
        // Receiving queue is not empty -> get command
        xQueueReceive(recvQueueDrive, (void *)&msgCommand, (TickType_t)0);
        // Execute the command
        int commandType = msgCommand.type;
        int commandValue = msgCommand.value;
        switch(commandType)
        {
            case IPC_R_STOP:
                changeMotorStatus(DRIVE_STOP);
                break;

            case IPC_R_FORWARD:
                if (motorStatus == DRIVE_BACKWARD &&
                    motorStatusLast == DRIVE_FORWARD) {
                    // When the motor is changing from FORWARD to BACKWARD
                    // Busy!
                    break;
                }
                // NOT busy.
                if (motorStatus == DRIVE_BACKWARD) {
                    pwmPercentageTarget = pwmPercentage;
                }
                changeMotorStatus(DRIVE_FORWARD);
                break;

            case IPC_R_BACKWARD:
                if (motorStatus == DRIVE_BACKWARD &&
                    motorStatusLast == DRIVE_FORWARD) {
                    // When the motor is changing from BACKWARD to FORWARD
                    // Busy!
                    break;
                }
                // NOT busy.
                if (motorStatus == DRIVE_FORWARD) {
                    pwmPercentageTarget = pwmPercentage;
                }
                changeMotorStatus(DRIVE_BACKWARD);
                break;

            case IPC_R_SETRATIO:
                if (commandValue >= 0 && commandValue <= DRIVE_MAX_COUNT * 10) {
                    pwmPercentageTarget = commandValue / 10;
                }
                break;

            default:
                // NULL command
                break;
        }
    }

    delay(DELAY_DRIVE);
}