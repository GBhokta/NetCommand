#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main()
{

    int sock_fd;

    struct sockaddr_in server_addr;

    sock_fd = socket(AF_INET,SOCK_STREAM,0);

    if(sock_fd<0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    printf("Socket created successfully\n");

    server_addr.sin_family = AF_INET; //for IPv4
    server_addr.sin_port = htons(PORT); //convert port number to network byte order
     if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sock_fd);
        return 1;
    }
    

    int temp;
    temp = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (temp < 0)
    {
        perror("connect failed");
        close(sock_fd);
        return 1;
    }

    char message[1024];

    while (1)
    {
        printf("Enter message to send to server: ");
        if (fgets(message, sizeof(message), stdin) == NULL)
            break;

        send(sock_fd, message, strlen(message), 0);

        printf("Message sent to server: %s", message);
        if (strcmp(message, "exit\n") == 0)
        {
            break;
        }
    }

    printf("Connected to server %s:%d\n", SERVER_IP, PORT);
    close(sock_fd);
    return 0;
}