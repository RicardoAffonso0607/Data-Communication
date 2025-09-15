#pragma once

#include <iostream>
#include <vector>
#include <string>
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

class WindowManager {
private:
    // Sender
    char originalMessage[2048];
    char binaryMessage[2048]; 
    char encryptedMessage[2048];

    // Receiver
    char decryptedMessage[2048];
    char receivedMessage[2048];

    vector<float> encryptionWaveform;
    vector<float> decryptionWaveform;

    GLFWwindow* window;

    Cripto* cripto;

public:
    explicit WindowManager();
    ~WindowManager();

    GLFWwindow* getWindow() const { return window; }

    void destroyWindow();
    void render();
    void createSenderWindow();
    void createReceiverWindow();
};