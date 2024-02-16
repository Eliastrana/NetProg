#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>

void handle_message(const std::string& message, struct sockaddr_in& client_addr, int server_fd) {
    char operation;
    int num1, num2;
    std::istringstream ss(message);
    ss >> operation >> num1 >> num2;

    std::cout << "Received message: " << std::endl;
    std::cout << "Num1: " << num1 << std::endl;
    std::cout << "Operation: " << operation << std::endl;
    std::cout << "Num2: " << num2 << std::endl << std::endl;

    int result;
    std::string response;
    switch(operation) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if(num2 == 0) {
                response = "Error: Division by zero";
                sendto(server_fd, response.c_str(), response.length(), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
                return;
            }
            result = num1 / num2;
            break;
        default:
            response = "Error: Invalid operation";
            sendto(server_fd, response.c_str(), response.length(), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
            return;
    }

    response = "Result: " + std::to_string(result);
    sendto(server_fd, response.c_str(), response.length(), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
}

int main() {
    int server_fd;
    struct sockaddr_in address, client_addr;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024];
    int port = 9000;

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    std::cout << "UDP Server listening on port " << port << std::endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        socklen_t client_len = sizeof(client_addr);
        // Receive data from client
        ssize_t message_len = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
        if(message_len > 0) {
            std::string message(buffer, message_len);
            handle_message(message, client_addr, server_fd);
        }
    }

    return 0;
}
