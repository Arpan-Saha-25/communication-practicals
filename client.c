#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>


#define SERVER_QUEUE_NAME "/server_queue"
#define MAX_MSG_SIZE 256



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
    msg_data msg;

    char client_queue_name[256];


    sprintf(client_queue_name, "/client_%d_queue", getpid());

    server_queue = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if (server_queue == -1)
        error("Client: mq_open");

    printf("Enter message to send: ");
    fgets(msg.message, MAX_MSG_SIZE, stdin);
    msg.message[strcspn(msg.message, "\n")] = 0;
    msg.priority = 1;



    if (mq_send(server_queue, (char *) &msg, sizeof(msg_data), msg.priority) == -1)
        error("Client: mq_send");

    printf("Message sent to server.\n");

    mq_close(server_queue);

    return 0;
}