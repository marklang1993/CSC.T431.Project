#ifndef _COMM_H_
#define _COMM_H_

#define COMM_SENDBUFF_SIZE      256
#define COMM_RECVBUFF_SIZE      256

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