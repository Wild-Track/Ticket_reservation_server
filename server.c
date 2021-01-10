#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>
#include<netinet/in.h>
#include"fct_ticket.c"

#define PORT 1337
#define MAIN_LOOP 1
#define LEN_BUFFER 4096

void *handle_client(void *arg);
void rcv_message(int socket_cli, char *message);
void consult_ticket(int socket_cli);
void reserved_ticket(struct Data_thread *data_thread);
void cancel_ticket(int socket_cli);

struct Data_thread
{
    struct Node_ticket *node_ticket;
    struct Node_reservation *node_reservation;
    int fdsocket_cli;
};

static struct Node_ticket node_ticket;
static struct Node_reservation node_reservation;

pthread_mutex_t lock;

int main()
{
    init_ticket(&node_ticket);

    struct sockaddr_in server, client;
    int len_addr_serv, len_addr_cli;
    pthread_t thread;
    
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    
    if (listener == -1)
    {
        puts("The socket was not created \n");
        exit(0);
    }
    
    len_addr_serv = sizeof(struct sockaddr_in);
    // memset(&server, 0x00, len_addr_serv);
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);
    
    
    if(bind(listener, (struct sockaddr*)&server, len_addr_serv) == -1)
    {
        puts("bind failed \n");
        exit(0);
    }
    
    if(listen(listener, 10) == -1)
    {
        puts("listen error \n");
        exit(0);
    }
    
    len_addr_cli = sizeof(client);
    
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        puts("mutex init failed \n");
        exit(0);
    }
    
    while(MAIN_LOOP)
    {
        int fdsocket_cli = accept(listener, (struct sockaddr *)&client, (socklen_t *)&len_addr_cli);
        if(fdsocket_cli == -1)
        {
            puts("accept failed \n");
            exit(0);
        }
        
        struct Data_thread *data_thread;
        
        data_thread = (struct Data_thread*) malloc(sizeof(struct Data_thread));
        
        data_thread->fdsocket_cli = fdsocket_cli;
        data_thread->node_ticket = &node_ticket;
        data_thread->node_reservation = &node_reservation;
        
        // pthread_create return 0 (=false) if thread is created
        if(pthread_create(&thread, NULL, handle_client, data_thread))
        {
            puts("thread not create \n");
            exit(0);
        }
    }
    
    close(listener);
    
    pthread_mutex_destroy(&lock);
    
    return 0;
}


void *handle_client(void *data_thread_server)
{
    pthread_mutex_lock(&lock);
    
    struct Data_thread data_thread = *(struct Data_thread*)data_thread_server;
    int socket_cli = data_thread.fdsocket_cli;
    struct Node_ticket *node_ticket = data_thread.node_ticket;
    struct Node_ticket nt = *node_ticket;
    struct Node_reservation *node_reservation = data_thread.node_reservation;
    struct Node_reservation nr = *node_reservation;
    
    pthread_mutex_unlock(&lock);
    
    char message[500];
    int not_exit = 1;
    

    while(not_exit)
    {
        rcv_message(socket_cli, message);
        
        if(strcmp(message, "consult") == 0)
        {
            consult_ticket(socket_cli);
        }
        else if(strcmp(message, "reserve") == 0)
        {
            reserved_ticket(&data_thread);
        }
        else if(strcmp(message, "cancel") == 0)
        {
            // cancel_ticket();
        }
        else if(strcmp(message, "quit") == 0)
        {
            not_exit = 0;
        }
        message[0] = 0;
    }
    
    pthread_exit(NULL);
    
}


void rcv_message(int socket_cli, char *message)
{
    int not_finish = 1, len_message;
    char buffer[LEN_BUFFER];
    
    while(not_finish)
    {
        len_message += recv(socket_cli, buffer, LEN_BUFFER, 0);
        if(buffer[len_message - 1] == '\n')
        {
            buffer[len_message - 1] = 0;
            not_finish = 0;
        }
        strcat(message, buffer);
        
    }
}

void consult_ticket(int socket_cli)
{
    char tickets[300];
    tickets[0] = 0;
    char response[50];
    
    list_ticket(&node_ticket, tickets);
    if(send(socket_cli, tickets, strlen(tickets), 0) != sizeof(tickets))
    {
        puts("Send doesn't work properly");
    }
    
}


void reserved_ticket(struct Data_thread *data_thread)
{
    char response[500];
    char delimiter[] = ",";
    char last_name[50], first_name[50], num_ticket[10];
    int number_ticket, not_found = 1, end = 1;

    rcv_message(data_thread->fdsocket_cli, response);
    
    char *ptr = strtok(response, delimiter);
    
    strcpy(last_name, ptr);
    ptr = strtok(NULL, delimiter);
    strcpy(first_name, ptr);
    ptr = strtok(NULL, delimiter);
    strcpy(num_ticket, delimiter);
    
    number_ticket = atoi(num_ticket);

    pthread_mutex_lock(&lock);

    int socket_cli = data_thread->fdsocket_cli;
    struct Node_ticket *node_ticket = data_thread->node_ticket;
    struct Node_ticket nt = *node_ticket;
    struct Node_reservation *node_reservation = data_thread->node_reservation;
    struct Node_reservation nr = *node_reservation;
    
    struct Node_ticket *p = &nr;

    while(not_found || end)
    {
        if(p != NULL)
        {
            if (p->current->number != number_ticket)
            {
                p = p->next;    
            }
            else
            {
                not_found = 0;
                add_in_tail_reservation(last_name, first_name, &nr, p->current);
                set_reservation_ticket(p->current, true);
            }
        }
        else
        {
            end = 0;
        }
    }

    pthread_mutex_unlock(&lock);
    
    if(end == 0)
    {
        char *message = "this place is already reserved";
        send(socket_cli, message, strlen(message), 0);
    }
    else
    {
        char *message = "you're reservation has been correctly realized";
        send(socket_cli, message, strlen(message), 0);
    }
    
}


