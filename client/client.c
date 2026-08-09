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

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock_fd);
        return 1;
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sock_fd);
        return 1;
    }
    
    printf("Connected to server %s:%d\n", SERVER_IP, PORT);  // MOVED HERE

    char ReadBuffer[1024];
    char WriteBuffer[1024];

    while (1) {
        printf("Client: ");
        if (fgets(WriteBuffer, sizeof(WriteBuffer), stdin) == NULL)
            continue;
            
        // Check for exit from client side
        if (strcmp(WriteBuffer, "exit\n") == 0) {
            send(sock_fd, WriteBuffer, strlen(WriteBuffer), 0);
            break;
        }
        
        send(sock_fd, WriteBuffer, strlen(WriteBuffer), 0);

        int bytes_received = recv(sock_fd, ReadBuffer, sizeof(ReadBuffer) - 1, 0);
        if (bytes_received > 0) {
            ReadBuffer[bytes_received] = '\0';
            printf("Server: %s", ReadBuffer);
        } else if (bytes_received == 0) {
            printf("Server disconnected\n");
            break;
        } else {
            perror("recv");
            break;
        }
    }

    close(sock_fd);
    return 0;
}