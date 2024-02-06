#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <sstream>

void handle_client(int client_socket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    read(client_socket, buffer, 1023);

    // Skriv ut mottatt melding

    char operation;
    int num1, num2;
    std::istringstream ss(buffer);
    ss >> operation >> num1 >> num2;

    std::cout << "Received message: " << std::endl;
    std::cout << "Num1: " << num1<< std::endl;
    std::cout << "Operation: " << operation<< std::endl;
    std::cout << "Num2: " << num2<< std::endl;
    std::cout << "\n" << std::endl;




    int result;
    switch(operation) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if(num2 == 0) {
                std::string errorMsg = "Error: Division by zero";
                write(client_socket, errorMsg.c_str(), errorMsg.length());
                close(client_socket);
                return;
            }
            result = num1 / num2;
            break;
        default:
            std::string errorMsg = "Error: Invalid operation";
            write(client_socket, errorMsg.c_str(), errorMsg.length());
            close(client_socket);
            return;
    }

    std::string response = "Result: " + std::to_string(result);
    write(client_socket, response.c_str(), response.length());
    close(client_socket);
}


int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int port = 9000;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        std::thread t([=](){ handle_client(new_socket); });
        t.detach();
    }

    return 0;
}
