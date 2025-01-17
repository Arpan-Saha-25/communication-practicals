#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME "/server_queue"
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define SERVER_MSG_SIZE (MAX_MSG_SIZE + 10)

typedef struct {
    long priority;
    char message[MAX_MSG_SIZE];
} msg_data;

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {

    mqd_t server_queue;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(msg_data);

    server_queue = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (server_queue == -1)
        error("Server: mq_open");

    printf("Server  started...\n");

    while (1) {
        msg_data msg;
        if (mq_receive(server_queue, (char *) &msg, sizeof(msg_data), NULL) == -1)
            error("Server: mq_receive");

        printf("Received message from client  %ld: %s\n", msg.priority, msg.message);


        printf("Message: %s\n", msg.message);
    }

    mq_close(server_queue);
    mq_unlink(SERVER_QUEUE_NAME);

    return 0;
}