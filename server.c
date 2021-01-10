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
void consult_ticket();
void reserved_ticket();
void cancel_ticket();


int main()
{
    puts("hgfd");
    
    struct sockaddr_in server, client;
    int len_addr_serv, len_addr_cli;
    pthread_t thread;
    struct Node_ticket *node_ticket = init_ticket();
    
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
    
    while(MAIN_LOOP)
    {
        int fdsocket_cli = accept(listener, (struct sockaddr *)&client, (socklen_t *)&len_addr_cli);
        if(fdsocket_cli == -1)
        {
            puts("accept failed \n");
            exit(0);
        }
        
        // pthread_create return 0 (=false) if thread is created
        if(pthread_create(&thread, NULL, handle_client, &fdsocket_cli))
        {
            puts("thread not create \n");
            exit(0);
        }
    }
    
    close(listener);
    
    return 0;
}


void *handle_client(void *fdsocket_cli)
{
    int socket_cli = *(int*)fdsocket_cli;
    char message[500];
    int not_exit = 1;
    
    rcv_message(socket_cli, message);

    puts(message);
    while(not_exit)
    {
        if(strcmp(message, "consult") == 0)
        {
            consult_ticket();
        }
        else if(strcmp(message, "reserve") == 0)
        {
            // reserved_ticket();
        }
        else if(strcmp(message, "cancel") == 0)
        {
            // cancel_ticket();
        }
        else if(strcmp(message, "quit") == 0)
        {
            not_exit = 0;
        }
    }
    
    pthread_exit(NULL);
    
}


void rcv_message(int socket_cli, char *message)
{
    int not_finish = 1, len_message;
    char buffer[LEN_BUFFER];
    
    while(not_finish)
    {
        len_message = recv(socket_cli, buffer, LEN_BUFFER, 0);
        if(buffer[len_message - 1] == '\n')
        {
            buffer[len_message - 1] = 0;
            not_finish = 0;
        }
        strcat(message, buffer);
        
    }
}

void consult_ticket()
{
    
}