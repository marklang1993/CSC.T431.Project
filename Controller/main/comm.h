#ifndef _COMM_H_
#define _COMM_H_

#define COMM_SENDBUFF_SIZE      256
#define COMM_RECVBUFF_SIZE      256

#define COMM_EMPTY              0
#define COMM_STOP               1
#define COMM_FORWARD            2
#define COMM_BACKWARD           3
#define COMM_SETRATIO           4
#define COMM_SETRELAY           5

struct commSendData {
        int voltage;
        int current;
        int relayStatus;
        int ratio;
        int motorStatus;
        int temperature;
};
struct commRecvData {
        int command;
        int subCommand;
};

void commTask(void *pvParameters);

void commInit(void);
void comm(void);

#endif