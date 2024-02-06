#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>

void handle_client(int client_socket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

    std::cout << "Request from client:\n" << buffer << std::endl;

    // Bygger HTTP-respons
    std::string http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\n"
            "<!DOCTYPE html>"
            "<html><body>"
            "<h1>HELLO! You have connected to the web server! </h1>"
            "<h2>Client Headers:</h2>"
            "<ul>";

    // Splitter request for å hente ut headers
    std::istringstream request_stream(buffer);
    std::string line;
    while (std::getline(request_stream, line) && line != "\r") {
        http_response += "<li>" + line + "</li>";
    }
    http_response += "</ul></body></html>";

    // Sender responsen til klienten
    write(client_socket, http_response.c_str(), http_response.length());

    // Lukker forbindelsen
    close(client_socket);
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int port = 9050; // Velg en port over 1024

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        handle_client(new_socket);
        //break; // For å kun håndtere én klient og deretter avslutte
    }

    return 0;
}
