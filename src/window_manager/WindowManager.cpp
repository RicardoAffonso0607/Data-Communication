#include "../../include/window_manager/WindowManager.h"

// Definições de protótipos de funções de callback para o GLFW
void glfwErrorCallback(int error, const char* description);

WindowManager::WindowManager() {

    const char* chave = "minhachave123";
    size_t tamanhoChave = strlen(chave);
    this->cripto = new Cripto(chave, tamanhoChave);

    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        exit(1);
    }

    // Configurações do OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(1280, 960, "Cripto-Com Project", NULL, NULL);
    if (this->window == NULL) {
        exit(1);
    }
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        exit(1);
    }

    // Configuração do ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF("./assets/Roboto-Regular.ttf", 20.0f);
    if (font) {
        ImGui::PushFont(font);
    }
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

WindowManager::~WindowManager() {
    destroyWindow();
}

void WindowManager::destroyWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void WindowManager::render() {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(this->window, &display_w, &display_h);
    glViewport(0, 0, display_h, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(this->window);
}

void WindowManager::createSenderWindow() {
    
    while (!glfwWindowShouldClose(this->window)) {
        glfwPollEvents();

        // Inicia um novo frame do ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Cria a janela principal do projeto
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Cripto-Com", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        // --- Coluna 1: Processo de Envio (Sender) ---
        ImGui::Text("Lado do Remetente (Envio)");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Campo para digitar a mensagem original
        ImGui::InputTextMultiline("Mensagem Original", this->originalMessage, IM_ARRAYSIZE(this->originalMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            strcpy(this->encryptedMessage, this->cripto->encrypt(this->originalMessage, strlen(this->originalMessage)));
            strcpy(this->binaryMessage, this->cripto->toBinary(this->encryptedMessage, strlen(this->originalMessage)));
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Campo editável para a mensagem criptografada 
        ImGui::Text("Mensagem Criptografada");
        ImGui::InputTextMultiline("##encrypted", this->encryptedMessage, IM_ARRAYSIZE(this->encryptedMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            strcpy(this->originalMessage, this->cripto->decrypt(this->encryptedMessage, strlen(this->encryptedMessage)));
            strcpy(this->binaryMessage, this->cripto->toBinary(this->encryptedMessage, strlen(this->encryptedMessage)));
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Campo editável para a mensagem em binário 
        ImGui::Text("Mensagem em Binário");
        ImGui::InputTextMultiline("##binary", this->binaryMessage, IM_ARRAYSIZE(this->binaryMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            strcpy(this->encryptedMessage, this->cripto->toChar(this->binaryMessage, strlen(this->binaryMessage)));
            strcpy(this->originalMessage, this->cripto->decrypt(this->encryptedMessage, strlen(this->encryptedMessage)));
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Gráfico do processo de montagem
        ImGui::Text("Forma de Onda (Codificação)");
        if (!this->encryptionWaveform.empty()) {
            ImGui::PlotLines("##plot_enc", this->encryptionWaveform.data(), this->encryptionWaveform.size(), 0, "Sinal", 0.0f, 1.0f, ImVec2(0, 80));
        } else {
            ImGui::Text("Gráfico gerado após a criptografia.");
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Botão de Enviar
        if (ImGui::Button("Enviar")) {
            // Ação: Lógica de envio
        }

        ImGui::End();

        this->render();
    }
}

void WindowManager::createReceiverWindow() {

    while (!glfwWindowShouldClose(this->window)) {
        glfwPollEvents();

        // Inicia um novo frame do ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Cria a janela principal do projeto
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Cripto-Com", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        
        // --- Coluna 2: Processo de Recebimento (Receiver) ---
        ImGui::Text("Lado do Receptor (Recebimento)");
        ImGui::Separator();
        
        // Campo editável para a mensagem recebida
        ImGui::Text("Mensagem Recebida");
        ImGui::InputTextMultiline("##received", this->receivedMessage, IM_ARRAYSIZE(this->receivedMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));

        // Botão de Descriptografar
        if (ImGui::Button("Descriptografar")) {
            // Ação: Lógica de descriptografia
        }

        // Campo editável para a mensagem descriptografada 
        ImGui::Text("Mensagem Descriptografada");
        ImGui::InputTextMultiline("##decrypted", this->decryptedMessage, IM_ARRAYSIZE(this->decryptedMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        
        // Gráfico do processo de desmontagem 
        ImGui::Text("Forma de Onda (Decodificação)");
        if (!this->decryptionWaveform.empty()) {
            ImGui::PlotLines("##plot_dec", this->decryptionWaveform.data(), this->decryptionWaveform.size(), 0, "Sinal", 0.0f, 1.0f, ImVec2(0, 80));
        } else {
            ImGui::Text("Gráfico gerado após a descriptografia.");
        }

        ImGui::End();

        this->render();
    }
}

void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}