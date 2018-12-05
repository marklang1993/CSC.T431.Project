#ifndef _IPC_H_
#define _IPC_H_

// Size of Message queue
#define IPC_QUEUE_SIZE  10

// Message type for sending
#define IPC_S_EMPTY     10
#define IPC_S_STOP      20
#define IPC_S_FORWARD   30
#define IPC_S_BACKWARD  40
#define IPC_S_SETRATIO  50
#define IPC_S_SETRELAY  60

struct msgDisplay {
        int voltage;
        int current;
        int relayStatus;
        int motorStatus;
        int ratio;
        int temperature;
};

struct msgCommand {
        int type;
        int value;
};

#endif