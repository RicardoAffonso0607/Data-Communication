#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

// ImGui header files
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// GLFW and GLAD header files
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Header for cryptography class
#include "../../include/cripto/Cripto.h"

// Header for socket class
#include "../../include/socket/Socket.h"

// Header for configuration class
#include "../../include/config/Config.h"

// Header for encoding class (NRZ, RZ)
#include "../../include/encoder/Encoder.h"

class WindowManager {
private:
    // Sender
    // Buffers to support longer messages (multiple sentences)
    // Previous size: 2048
    static constexpr size_t MESSAGE_BUF_SIZE = 16384; // 16 KB
    char originalMessage[MESSAGE_BUF_SIZE];
    char binaryMessage[MESSAGE_BUF_SIZE]; 
    char encryptedMessage[MESSAGE_BUF_SIZE];
    char nrzMessage[MESSAGE_BUF_SIZE];
    char rzMessage[MESSAGE_BUF_SIZE];
    
    // Track actual sizes of encrypted/binary messages (since they contain binary data)
    size_t encryptedMessage_size = 0;
    size_t binaryMessage_size = 0;
    size_t nrzMessage_size = 0;
    size_t rzMessage_size = 0;

    // Receiver
    char decryptedMessage[MESSAGE_BUF_SIZE];
    char receivedMessage[MESSAGE_BUF_SIZE];
    char receivedNrzMessage[MESSAGE_BUF_SIZE];
    char receivedRzMessage[MESSAGE_BUF_SIZE];
    size_t receivedMessage_size = 0;
    size_t receivedNrzMessage_size = 0;
    size_t receivedRzMessage_size = 0;

    vector<float> encryptionWaveform;
    vector<float> decryptionWaveform;

    // New encoding fields (Sender)
    vector<float> senderNRZ_Waveform;
    vector<float> senderRZ_Waveform;
    Encoder::Tipo senderActiveEncoding = Encoder::NRZ;
    
    // New encoding fields (Receiver)
    vector<float> receiverNRZ_Waveform;
    vector<float> receiverRZ_Waveform;
    Encoder::Tipo receiverActiveEncoding = Encoder::NRZ;

    GLFWwindow* window;

    Crypto* crypto;
    Socket* sender_socket;
    Socket* receiver_socket;
    
    // Connection state
    bool sender_connected;
    
    // Receiver state
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