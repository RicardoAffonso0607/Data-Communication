#pragma once

#include <string>
#include <functional>

// Conditional compilation for different operating systems
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

// Callback type for received messages
using MessageCallback = std::function<void(const std::string&)>;

class Socket {
    private:
        int sock_fd;
        sockaddr_in server_addr;
        MessageCallback on_message_received;
        std::string receiver_ip;
        int port;

    #ifdef _WIN32
        WSADATA wsaData;
    #endif

    public:
        Socket(const std::string& ip = "127.0.0.1", int p = 4444);
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
        void setMessageCallback(MessageCallback callback);
        void setConfig(const std::string& ip, int p);
        
        void closeSocket();
};