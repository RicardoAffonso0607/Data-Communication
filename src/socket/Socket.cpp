# include "../../include/socket/Socket.h"

#include <iostream>
#include <cstring>
#include <cstdint>
#include <endian.h>

Socket::Socket() : sock_fd(-1), on_message_received(nullptr) {
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
        return false;
    }
    return true;
}

bool Socket::bindSocket(int port) {
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    if (bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        return false;
    }
    return true;
}

bool Socket::listenForRequests(int backlog) {
    if (listen(sock_fd, backlog) < 0) {
        return false;
    }
    return true;
}

int Socket::acceptConnection() {
    int client_sock_fd;
    socklen_t client_len = sizeof(sockaddr_in);
    sockaddr_in client_addr;

    client_sock_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_len);
    return client_sock_fd;
}

bool Socket::connectToReceiver(const std::string& ip, int port) {
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        return false;
    }
    return true;
}

bool Socket::sendData(const std::string& data) {
    return sendData(sock_fd, data);
}

bool Socket::sendData(int target_sock_fd, const std::string& data) {
    // Send a 8-byte length prefix (network byte order) followed by the data.
    // This handles binary data correctly (no null terminators).
    uint64_t len = static_cast<uint64_t>(data.size());
    uint64_t net_len = htobe64(len);  // or use manual byte swap if htobe64 unavailable

    // Helper: send all bytes
    size_t total_sent = 0;
    const char* ptr = reinterpret_cast<const char*>(&net_len);
    size_t to_send = sizeof(net_len);
    while (total_sent < to_send) {
        ssize_t sent = send(target_sock_fd, ptr + total_sent, to_send - total_sent, 0);
        if (sent <= 0) return false;
        total_sent += static_cast<size_t>(sent);
    }

    total_sent = 0;
    ptr = data.data();
    to_send = data.size();
    while (total_sent < to_send) {
        ssize_t sent = send(target_sock_fd, ptr + total_sent, to_send - total_sent, 0);
        if (sent <= 0) return false;
        total_sent += static_cast<size_t>(sent);
    }

    return true;
}

std::string Socket::receiveData() {
    return receiveData(sock_fd);
}

std::string Socket::receiveData(int target_sock_fd) {
    // Read a 8-byte length prefix, then read exactly that many bytes.
    uint64_t net_len = 0;
    char* len_ptr = reinterpret_cast<char*>(&net_len);
    size_t len_read = 0;
    while (len_read < sizeof(net_len)) {
        ssize_t r = recv(target_sock_fd, len_ptr + len_read, sizeof(net_len) - len_read, 0);
        if (r == 0) {
            // connection closed
            return "";
        }
        if (r < 0) {
            return "";
        }
        len_read += static_cast<size_t>(r);
    }

    uint64_t payload_len = be64toh(net_len);  // or use manual byte swap
    // Sanity cap to avoid OOM; allow up to 1 MB by default
    const uint64_t MAX_MESSAGE_SIZE = 1024 * 1024; // 1 MB
    if (payload_len == 0 || payload_len > MAX_MESSAGE_SIZE) {
        // treat as error or empty
        return "";
    }

    std::string buffer;
    buffer.resize(payload_len);
    size_t received = 0;
    while (received < payload_len) {
        ssize_t r = recv(target_sock_fd, &buffer[received], payload_len - received, 0);
        if (r == 0) {
            // connection closed prematurely
            return "";
        }
        if (r < 0) {
            return "";
        }
        received += static_cast<size_t>(r);
    }

    return buffer;
}

void Socket::run_receiver_server() {
    if (this->createSocket() && this->bindSocket(4444) && this->listenForRequests()) {
        std::cout << "Receiver server running on port 4444..." << std::endl;
        while (true) {
            int client_sock = this->acceptConnection();
            if (client_sock != -1) {
                // Recebe mensagens continuamente na mesma conexão
                while (true) {
                    std::string data = this->receiveData(client_sock);
                    
                    // Se recebeu dados vazios, conexão foi fechada
                    if (data.empty()) {
                        break;
                    }
                    
                    // Chama o callback se foi definido
                    if (this->on_message_received) {
                        this->on_message_received(data);
                    }
                }
                
                #ifdef _WIN32
                closesocket(client_sock);
                #else
                close(client_sock);
                #endif
            }
        }
    }
}

void Socket::setMessageCallback(MessageCallback callback) {
    this->on_message_received = callback;
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