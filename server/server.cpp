#include <cstring>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <thread>
#include <mutex>
#include <vector>
#include "../common/common.hpp"

#define BUFFER_SIZE 1024

std::vector<int>clientSockets;
std::mutex clientMutex;

void handle_client(int clientSocket) {
    char buffer[BUFFER_SIZE] = {0};
    while(true){
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Client disconnected" << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::cout << "Message from client: " << buffer << std::endl;

        std::lock_guard<std::mutex> lock(clientMutex);
        for (auto& socket : clientSockets) {
            if (socket != clientSocket) {
                send(socket, buffer, strlen(buffer), 0);
            }
        }
    }

    // close connection
    close(clientSocket);
}

int main(){
    char buffer[BUFFER_SIZE] = {0};
    int serverSocket = create_server_socket(8080); 
    std::vector<std::thread> threads;

    while(true){
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            std::cerr << "Accept failed" << std::endl;
            close(serverSocket);
            return 1;
        }

        std::cout << "Client "  << clientSocket <<  " connected" << std::endl;

        {
            std::lock_guard<std::mutex> lock(clientMutex);
            clientSockets.push_back(clientSocket);
        }

        threads.emplace_back(handle_client, clientSocket);
        threads.back().detach();

    }

    close(serverSocket);

    return 0;
}
