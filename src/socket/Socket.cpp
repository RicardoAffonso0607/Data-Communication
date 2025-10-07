# include "../../include/socket/Socket.h"

#include <iostream>

Socket::Socket() : sock_fd(-1) {
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
    }
#endif
}

Socket::~Socket() {
    closeSocket();
#ifdef _WIN32
    WSACleanup();
#endif
}

bool Socket::createSocket() {
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return false;
    }
    return true;
}

bool Socket::bindSocket(int port) {
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    if (bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
        return false;
    }
    return true;
}

bool Socket::listenForRequests(int backlog) {
    if (listen(sock_fd, backlog) < 0) {
        std::cerr << "Error listening." << std::endl;
        return false;
    }
    return true;
}

int Socket::acceptConnection() {
    int client_sock_fd;
    socklen_t client_len = sizeof(sockaddr_in);
    sockaddr_in client_addr;

    client_sock_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_sock_fd < 0) {
        std::cerr << "Error accepting connection." << std::endl;
    }
    return client_sock_fd;
}

bool Socket::connectToReceiver(const std::string& ip, int port) {
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error connecting to server." << std::endl;
        return false;
    }
    return true;
}

bool Socket::sendData(const std::string& data) {
    return sendData(sock_fd, data);
}

bool Socket::sendData(int target_sock_fd, const std::string& data) {
    if (send(target_sock_fd, data.c_str(), data.length(), 0) < 0) {
        std::cerr << "Error sending data." << std::endl;
        return false;
    }
    return true;
}

std::string Socket::receiveData() {
    return receiveData(sock_fd);
}

std::string Socket::receiveData(int target_sock_fd) {
    char buffer[1024] = {0};
    int bytes_read = recv(target_sock_fd, buffer, 1024, 0);
    if (bytes_read < 0) {
        std::cerr << "Error receiving data." << std::endl;
        return "";
    }
    return std::string(buffer, bytes_read);
}

void Socket::run_receiver_server() {
    if (this->createSocket() && this->bindSocket(4444) && this->listenForRequests()) {
        std::cout << "Receiver server running on port 4444..." << std::endl;
        while (true) {
            int client_sock = this->acceptConnection();
            if (client_sock != -1) {
                std::string data = this->receiveData(client_sock);
                std::cout << "Received: " << data << std::endl;
                #ifdef _WIN32
                closesocket(client_sock);
                #else
                close(client_sock);
                #endif
            }
        }
    }
}

void Socket::closeSocket() {
    if (sock_fd >= 0) {
#ifdef _WIN32
        closesocket(sock_fd);
#else
        close(sock_fd);
#endif
        sock_fd = -1;
    }
}