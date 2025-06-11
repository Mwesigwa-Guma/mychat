// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    if (connect(clientSocket, (struct sockaddr*)&serverAddress,
                sizeof(serverAddress)) < 0) {
        std::cerr << "Connection to server failed\n";
        close(clientSocket);
        return 1;
    }

    // sending data
    const char* message = "Hello, server!";
    if (send(clientSocket, message, strlen(message), 0) < 0) {
        std::cerr << "Sending message failed\n";
        close(clientSocket);
        return 1;
    }
    // closing socket
    close(clientSocket);

    return 0;
}
