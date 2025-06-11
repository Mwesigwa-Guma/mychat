#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

int main(){
    int serverSocket = socket(AF_INET, SOCK_STREAM , 0);
    if (serverSocket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        std::cerr << "Accept failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    char buffer[1024] = {0};
    ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0) {
        std::cerr << "Receive failed" << std::endl;
        close(clientSocket);
        close(serverSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';
    std::cout << "Message from client: " << buffer << std::endl;

    close(clientSocket);
    close(serverSocket);

    return 0;
}
