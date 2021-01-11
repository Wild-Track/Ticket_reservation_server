#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 1337
#define MAIN_LOOP 1
#define LEN_BUFFER 4096
#define IP "127.0.0.1"

int main()
{
    int listener;
    struct sockaddr_in server;
    char buffer[LEN_BUFFER];

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

    char *msg = "consult";
    send(listener, msg, strlen(msg), 0);

    return 0;
}
