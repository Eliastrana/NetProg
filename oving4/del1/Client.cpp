#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    socklen_t addrlen = sizeof(serv_addr);

    // Create a socket once before the loop
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    while(true) {
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer each time

        char operation;
        int num1, num2;
        std::cout << "Enter your first number: ";
        std::cin >> num1;
        std::cout << "Enter your second number: ";
        std::cin >> num2;
        std::cout << "Enter the operation (+, -, *, /): ";
        std::cin >> operation; // Capture the operation
        std::cin.ignore(); // Clear input buffer to handle subsequent getline

        std::stringstream ss;
        ss << operation << " " << num1 << " " << num2; // Include the operation in the message
        std::string message = ss.str();

        // Send the message
        sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        // Receive the answer
        valread = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&serv_addr, &addrlen);
        std::cout << "Answer from server: " << buffer << std::endl;

        // Ask to continue or exit
        std::string decision;
        std::cout << "More math? (y/n): ";
        std::getline(std::cin, decision);
        if (decision != "y" && decision != "Y") {
            break;
        }
    }

    // Close the socket after the loop
    close(sock);
    return 0;
}
