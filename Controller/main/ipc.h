#ifndef _IPC_H_
#define _IPC_H_

// Size of Message queue
#define IPC_QUEUE_SIZE  10

// Message type for sending
#define IPC_S_VOLTAGE   1
#define IPC_S_CURRENT   2
#define IPC_S_RELAY     3
#define IPC_S_RATIO     4
#define IPC_S_MOTOR     5

// Message type for receiving
#define IPC_R_EMPTY     10
#define IPC_R_STOP      20
#define IPC_R_FORWARD   30
#define IPC_R_BACKWARD  40
#define IPC_R_SETRATIO  50
#define IPC_R_SETRELAY  60

struct msgDrive {
        int ratio;
        int motorStatus;
};

struct msgScan {
        int voltage;
        int current;
        int relayStatus;
};

struct msgI2C {
        int temperature;
};

struct msgCommand {
        int type;
        int value;
};

#endif