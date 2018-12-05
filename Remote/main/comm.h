#ifndef _COMM_H_
#define _COMM_H_

#define COMM_EMPTY              0
#define COMM_STOP               1
#define COMM_FORWARD            2
#define COMM_BACKWARD           3
#define COMM_SETRATIO           4
#define COMM_SETRELAY           5

void commTask(void *pvParameters);

#endif