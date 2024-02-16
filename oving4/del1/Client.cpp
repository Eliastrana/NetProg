#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

int main() {
    while(true) {
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        char buffer[1024] = {0};
        socklen_t addrlen = sizeof(serv_addr);

        sock = socket(AF_INET, SOCK_DGRAM, 0);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(9000);

        inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

//        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
//            std::cerr << "Connection Failed" << std::endl;
//            return -1;
//        }

        char operation;
        int num1, num2;
        std::cout << "Enter your first number: ";
        std::cin >> num1;
        std::cout << "Enter your second number: ";
        std::cin >> num2;
        std::cout << "Enter the operation (+, -, *, /): ";
        std::cin >> operation; // Fanger opp operasjonen
        std::cin.ignore(); // Renser input bufferen for å håndtere etterfølgende getline

        std::stringstream ss;
        ss << operation << " " << num1 << " " << num2; // Inkluderer operasjonen i meldingen
        std::string message = ss.str();



        sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));


        valread = recvfrom (sock, buffer, 1024, 0, (struct sockaddr *)&serv_addr, &addrlen);
        std::cout << "Answer from server: " << buffer << std::endl;

        close(sock);

        // Spør om å fortsette eller avslutte
        std::string decision;
        std::cout << "More math? (y/n): ";
        std::getline(std::cin, decision);
        if (decision != "y" && decision != "Y") {
            break;
        }
    }
    return 0;
}
