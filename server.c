#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>
#include<netinet/in.h>

#define PORT 6000
#define MAIN_LOOP 1

enum Reserved{true, false};

struct Ticket
{
    int number;
    enum Reserved reserved;
    Ticket *p;
};

Ticket *init_ticket();
void add_in_tail(Ticket *&ticket);
void *handle_client(void *arg);

int main()
{
    
    struct sockaddr_in server, client;
    int len_addr_serv, len_addr_cli;
    pthread_t thread;
    int value;
    Ticket *ticket = init_ticket();
    
    int fdsocket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (fdsocket == -1)
    {
        printf("The socket was not created \n");
        exit(0);
    }
    
    len_addr_serv = sizeof(struct sockaddr_in);
    memset(&server, 0x00, len_addr_serv);
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htonl(PORT);
    
    
    if(bind(fdsocket, (struct sockaddr*)&server, len_addr_serv) == -1)
    {
        printf("bind failed \n");
        exit(0);
    }
    
    if(listen(fdsocket, 10) == -1)
    {
        printf("listen error \n");
        exit(0);
    }
    
    len_addr_cli = sizeof(client);
    
    while(MAIN_LOOP)
    {
        int fdsocket_cli = accept(fdsocket, (struct sockaddr *)&client, &len_addr_cli);
        if(fdsocket_cli == -1)
        {
            printf("accept failed \n");
            exit(0);
        }
        
        // pthread_create return 0 (=false) if thread is created
        if(pthread_create(&thread, NULL, handle_client, &value))
        {
            printf("thread not create \n");
            exit(0);
        }
        
        
        
    }
    
}


void add_in_tail(int number, Ticket *&ticket)
{
    if(!ticket)
    {
        Ticket *t;
        t = (Ticket*) malloc(sizeof(Ticket));
        
        t->number = number;
        t->reserved = false;
        t->p = NULL;
        
        ticket = t;
    }
    else 
    {
        add_in_tail(number, ticket->p);
    }
}


Ticket *init_ticket()
{
    Ticket *ticket;
    
    for(int i = 1; i <= 100; i++)
    {
        add_in_tail(i, ticket);
    }
    
    return ticket;
}


void *handle_client(void *arg)
{
    
    
    
    
    pthread_exit(NULL);
}