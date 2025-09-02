#include <iostream>
#include <vector>
#include <string>

// Header files do ImGui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// Header files do GLFW e GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Definições de protótipos de funções de callback para o GLFW
void glfwErrorCallback(int error, const char* description);

// Variáveis globais para o estado da interface
static char originalMessage[256] = "";
static char binaryMessage[2048] = ""; // Aumentado para acomodar mais caracteres
static char encryptedMessage[2048] = "";
static char decryptedMessage[2048] = "";
static char receivedMessage[2048] = "";

// Simulação de dados para os gráficos
// Em um projeto real, estes dados seriam gerados pelo seu algoritmo
static std::vector<float> encryptionWaveform;
static std::vector<float> decryptionWaveform;

int main() {
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        return 1;
    }

    // Configurações do OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Cripto-Com Project", NULL, NULL);
    if (window == NULL) {
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return 1;
    }

    // Configuração do ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Inicia um novo frame do ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Cria a janela principal do projeto
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Cripto-Com", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        // --- Layout em duas colunas: Lado do Remetente e Lado do Receptor ---
        ImGui::Columns(2, "Two-sided layout");

        // --- Coluna 1: Processo de Envio (Remetente) ---
        ImGui::Text("Lado do Remetente (Envio)");
        ImGui::Separator();

        // Campo para digitar a mensagem original
        ImGui::InputTextMultiline("Mensagem Original", originalMessage, IM_ARRAYSIZE(originalMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        
        // Botão de Criptografar e Enviar
        if (ImGui::Button("Criptografar e Enviar")) {
            // Ação: Lógica de criptografia, geração de binário, gráfico e envio
        }

        // Campo editável para a mensagem em binário 
        ImGui::Text("Mensagem em Binário");
        ImGui::InputTextMultiline("##binary", binaryMessage, IM_ARRAYSIZE(binaryMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));

        // Campo editável para a mensagem criptografada 
        ImGui::Text("Mensagem Criptografada");
        ImGui::InputTextMultiline("##encrypted", encryptedMessage, IM_ARRAYSIZE(encryptedMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        
        // Gráfico do processo de montagem
        ImGui::Text("Forma de Onda (Codificação)");
        if (!encryptionWaveform.empty()) {
            ImGui::PlotLines("##plot_enc", encryptionWaveform.data(), encryptionWaveform.size(), 0, "Sinal", 0.0f, 1.0f, ImVec2(0, 80));
        } else {
            ImGui::Text("Gráfico gerado após a criptografia.");
        }
        
        ImGui::NextColumn();
        
        // --- Coluna 2: Processo de Recebimento (Receptor) ---
        ImGui::Text("Lado do Receptor (Recebimento)");
        ImGui::Separator();
        
        // Campo editável para a mensagem recebida
        ImGui::Text("Mensagem Recebida");
        ImGui::InputTextMultiline("##received", receivedMessage, IM_ARRAYSIZE(receivedMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));

        // Botão de Descriptografar
        if (ImGui::Button("Descriptografar")) {
            // Ação: Lógica de descriptografia
        }

        // Campo editável para a mensagem descriptografada 
        ImGui::Text("Mensagem Descriptografada");
        ImGui::InputTextMultiline("##decrypted", decryptedMessage, IM_ARRAYSIZE(decryptedMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        
        // Gráfico do processo de desmontagem 
        ImGui::Text("Forma de Onda (Decodificação)");
        if (!decryptionWaveform.empty()) {
            ImGui::PlotLines("##plot_dec", decryptionWaveform.data(), decryptionWaveform.size(), 0, "Sinal", 0.0f, 1.0f, ImVec2(0, 80));
        } else {
            ImGui::Text("Gráfico gerado após a descriptografia.");
        }

        ImGui::End();

        // Renderização
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_h, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Limpeza
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}