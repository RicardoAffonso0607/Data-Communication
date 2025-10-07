#pragma once

#include <string>

// Condicionais para compilação em diferentes sistemas operacionais
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#define PORT 4444
#define RECEIVER_IP "127.0.0.1"

class Socket {
    private:
        int sock_fd;
        sockaddr_in server_addr;

    #ifdef _WIN32
        WSADATA wsaData;
    #endif

    public:
        Socket();
        ~Socket();

        bool createSocket();
        bool bindSocket(int port);
        bool listenForRequests(int backlog = 5);
        int acceptConnection();
        bool connectToReceiver(const std::string& ip, int port);

        bool sendData(const std::string& data);
        bool sendData(int target_sock_fd, const std::string& data);

        std::string receiveData();
        std::string receiveData(int target_sock_fd);

        void run_receiver_server();
        
        void closeSocket();
};