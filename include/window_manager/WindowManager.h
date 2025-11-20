#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

// Header files do ImGui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// Header files do GLFW e GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Header da classe para lidar com criptografia
#include "../../include/cripto/Cripto.h"

// Header da classe para lidar com sockets
#include "../../include/socket/Socket.h"

class WindowManager {
private:
    // Sender
    // Increased buffers to support longer messages (multiple sentences)
    // Previous size: 2048
    static constexpr size_t MESSAGE_BUF_SIZE = 16384; // 16 KB
    char originalMessage[MESSAGE_BUF_SIZE];
    char binaryMessage[MESSAGE_BUF_SIZE]; 
    char encryptedMessage[MESSAGE_BUF_SIZE];
    
    // Track actual sizes of encrypted/binary messages (since they contain binary data)
    size_t encryptedMessage_size = 0;
    size_t binaryMessage_size = 0;

    // Receiver
    char decryptedMessage[MESSAGE_BUF_SIZE];
    char receivedMessage[MESSAGE_BUF_SIZE];
    size_t receivedMessage_size = 0;

    vector<float> encryptionWaveform;
    vector<float> decryptionWaveform;

    GLFWwindow* window;

    Cripto* cripto;
    Socket* sender_socket;
    Socket* receiver_socket;
    
    // Estado das conexões
    bool sender_connected;
    
    // Estado do receiver
    bool message_received;
    bool message_decrypted;
    float notification_timer;

public:
    explicit WindowManager();
    ~WindowManager();

    GLFWwindow* getWindow() const { return window; }

    void destroyWindow();
    void render();
    void createSenderWindow();
    void createReceiverWindow();
};