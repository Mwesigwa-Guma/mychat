// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <thread>

#define BUFFER_SIZE 1024

void receive_messages(int sock) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            std::cout << "[*] Connection closed by server or error occurred.\n";
            break;
        }
        std::cout << "[Server] " << buffer << std::endl;
    }
}

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

    std::thread receiver(receive_messages, clientSocket);
    std::string input;

    while(true){
        std::getline(std::cin, input);
        if(input == "exit") break;
        if (send(clientSocket, input.c_str(), input.size(), 0) < 0) {
            std::cerr << "Sending message failed\n";
            close(clientSocket);
            return 1;
        }
    }

    // closing socket
    close(clientSocket);
    receiver.join();

    return 0;
}
