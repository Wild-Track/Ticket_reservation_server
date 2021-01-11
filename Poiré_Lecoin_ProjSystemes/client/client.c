#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 1337
#define LEN_BUFFER 4096
#define IP "127.0.0.1"

void consult_ticket(int listener);
void reserved_ticket(int listener);
void cancel_ticket(int listener);
void quit(int listener);

int main()
{
    int listener, response, main_loop = 1;
    struct sockaddr_in server;

    listener = socket(AF_INET, SOCK_STREAM, 0);

    if (listener == -1)
    {
        puts("The socket was not created \n");
        exit(0);
    }

    server.sin_family = AF_INET;
    inet_aton(IP, &server.sin_addr);
    server.sin_port = htons(PORT);

    if (connect(listener, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        puts(" failed to connect \n");
        exit(0);
    }

    while (main_loop)
    {
        printf("What do you want to do ? \n -see available places (1) \n -Book a seat (2) \n -Cancel a reservation (3) \n -Quit (4) \n");
        scanf("%d", &response);
        switch (response)
        {
        case 1:
            consult_ticket(listener);
            break;

        case 2:
            reserved_ticket(listener);
            break;

        case 3:
            cancel_ticket(listener);
            break;

        case 4:
            main_loop = 0;
            quit(listener);
            break;

        default:
            printf("Please choose among the answers 1 to 4 \n");
            break;
        }
    }

    return 0;
}

void consult_ticket(int listener)
{
    char *msg = "consult\n", response[LEN_BUFFER];
    send(listener, msg, strlen(msg), 0);

    recv(listener, response, LEN_BUFFER, 0);
    printf("Here are the places available : \n %s", response);
}

void reserved_ticket(int listener)
{
    char *msg = "reserve\n";
    send(listener, msg, strlen(msg), 0);

    char last_name[50], first_name[50], num_ticket[10];
    char message[120], response[LEN_BUFFER];

    printf("last_name :\n");
    scanf("%s", &last_name);
    printf("first_name :\n");
    scanf("%s", first_name);
    printf("Number of reserved place :\n");
    scanf("%s", num_ticket);

    sprintf(message, "%s,%s,%s\n\0", last_name, first_name, num_ticket);

    send(listener, message, strlen(message), 0);

    recv(listener, response, LEN_BUFFER, 0);

    printf("%s", response);
}

void cancel_ticket(int listener)
{
    char *msg = "cancel\n";
    send(listener, msg, strlen(msg), 0);

    char file_number[20], last_name[50], message[100];

    printf("Please provide your file number");
    scanf("%s", &file_number);
    printf("Please enter your last_name");
    scanf("%s", &last_name);

    sprintf(message, "%s,%s\n\0", file_number, last_name);

    send(listener, message, strlen(message), 0);
}

void quit(int listener)
{
    char *msg = "quit\n";
    send(listener, msg, strlen(msg), 0);
}