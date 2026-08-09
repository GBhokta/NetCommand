#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }
    printf("Server bound to port %d\n", PORT);

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }
    printf("Server listening...\n");

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return 1;
    }
    printf("Client connected\n");

    char Readbuffer[1024];
    char Writebuffer[1024];
    int bytes_received;

    while (1) {
        bytes_received = recv(client_fd, Readbuffer, sizeof(Readbuffer) - 1, 0);
        
        if (bytes_received > 0) {
            Readbuffer[bytes_received] = '\0';
            printf("Client: %s", Readbuffer);
            
            // Check for exit command from client
            if (strcmp(Readbuffer, "exit\n") == 0) {
                printf("Client requested exit\n");
                break;
            }
        } else if (bytes_received == 0) {
            printf("Client disconnected\n");
            break;
        } else {
            perror("recv");
            break;
        }

        // Get server response
        printf("Server: ");
        if (fgets(Writebuffer, sizeof(Writebuffer), stdin) == NULL)
            break;
            
        // Check for server exit
        if (strcmp(Writebuffer, "exit\n") == 0) {
            send(client_fd, Writebuffer, strlen(Writebuffer), 0);
            break;
        }
        
        send(client_fd, Writebuffer, strlen(Writebuffer), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}