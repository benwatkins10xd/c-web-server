#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

void handle_request(int client_socket)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    // Receive the HTTP request
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0)
    {
        perror("Error receiving data");
        return;
    }

    // Print the received request
    printf("Received request:\n%s\n", buffer);

    // Prepare the HTTP response
    const char *response = "HTTP/1.1 200 OK\r\n\r\n<html><body>Helldedo, C Socket AvdmfamdfPI!</body></html>";
    send(client_socket, response, strlen(response), 0);

    // Close the client socket
    close(client_socket);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to a specific address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0)
    {
        perror("Error listening for connections");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        // Accept a connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0)
        {
            perror("Error accepting connection");
            continue;
        }

        // Handle the request in a separate function
        handle_request(client_socket);
    }

    // Close the server socket (not reached in this example)
    close(server_socket);

    return 0;
}
