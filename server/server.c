#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main()
{
    int server_fd; // server listening socket file descriptor
    int client_fd; // client socket

    struct sockaddr_in server_addr, client_addr;

    socklen_t client_len = sizeof(client_addr); // size of cleint address

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;         // for IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // accept connections from any IP address
    server_addr.sin_port = htons(PORT);       // convert port number to network byte order

    // 3. Bind socket to IP and port
    if (bind(server_fd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    printf("Server bound to port %d\n", PORT);

    // 4. Start listening
    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Server listening...\n");

    // 5. Accept a client
    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &client_len);

    if (client_fd < 0)
    {
        perror("accept");
        close(server_fd);
        return 1;
    }

    char buffer[1024];

    int bytes_received;
    while (1)
    {
        bytes_received = recv(
            client_fd,
            buffer,
            sizeof(buffer) - 1,
            0);

        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            printf("Client says: %s", buffer);
        }
        else if(strcmp(buffer,"exit\n")==0)
        {
            break;
        }
    }

    printf("Client connected\n");

    close(server_fd);

    return 0;
}
