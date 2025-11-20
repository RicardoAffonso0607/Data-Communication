#include "../../include/window_manager/WindowManager.h"

#define PORT 4444
#define RECEIVER_IP "127.0.0.1"

// Definições de protótipos de funções de callback para o GLFW
void glfwErrorCallback(int error, const char* description);

WindowManager::WindowManager() {

    const char* key = "mykey123";
    size_t keyLength = strlen(key);
    this->cripto = new Cripto(key, keyLength);

    this->sender_socket = new Socket();
    this->receiver_socket = new Socket();
    this->sender_connected = false;
    this->message_received = false;
    this->message_decrypted = false;
    this->notification_timer = 0.0f;
    
    // Configura o callback para quando uma mensagem é recebida
    this->receiver_socket->setMessageCallback([this](const std::string& data) {
        // data contém dados binários, não é necessariamente string com null-terminator
        memcpy(this->receivedMessage, data.data(), data.size());
        this->receivedMessage_size = data.size();
        this->message_received = true;
        this->message_decrypted = false;
        this->notification_timer = 5.0f; // Notificação por 5 segundos
        
        // Gera o waveform a partir da mensagem binária recebida
        this->decryptionWaveform = this->cripto->generateWaveform(this->receivedMessage, data.size());
    });
    
    thread server_thread(&Socket::run_receiver_server, this->receiver_socket);
    server_thread.detach();  // Libera a thread para rodar independentemente
    this_thread::sleep_for(chrono::seconds(1));

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
    if (this->sender_socket) {
        this->sender_socket->closeSocket();
        delete this->sender_socket;
    }
    if (this->receiver_socket) {
        this->receiver_socket->closeSocket();
        delete this->receiver_socket;
    }
    if (this->cripto) {
        delete this->cripto;
    }

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
    
    // Tenta conectar ao receiver uma única vez
    if (!this->sender_connected) {
        if (this->sender_socket->createSocket()) {
            if (this->sender_socket->connectToReceiver(RECEIVER_IP, PORT)) {
                this->sender_connected = true;
            }
        }
    }
    
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
            size_t msg_len = strlen(this->originalMessage);
            char* encrypted = this->cripto->encrypt(this->originalMessage, msg_len);
            memcpy(this->encryptedMessage, encrypted, msg_len);
            this->encryptedMessage_size = msg_len;
            delete[] encrypted;
            
            char* binary = this->cripto->toBinary(this->encryptedMessage, msg_len);
            size_t bin_len = msg_len * 8;
            memcpy(this->binaryMessage, binary, bin_len);
            this->binaryMessage_size = bin_len;
            this->binaryMessage[bin_len] = '\0';
            delete[] binary;
            
            this->encryptionWaveform = this->cripto->generateWaveform(this->binaryMessage, bin_len);
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Campo editável para a mensagem criptografada 
        ImGui::Text("Mensagem Criptografada");
        ImGui::InputTextMultiline("##encrypted", this->encryptedMessage, IM_ARRAYSIZE(this->encryptedMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            size_t enc_len = this->encryptedMessage_size > 0 ? this->encryptedMessage_size : strlen(this->encryptedMessage);
            char* decrypted = this->cripto->decrypt(this->encryptedMessage, enc_len);
            strcpy(this->originalMessage, decrypted);
            delete[] decrypted;
            
            char* binary = this->cripto->toBinary(this->encryptedMessage, enc_len);
            size_t bin_len = enc_len * 8;
            memcpy(this->binaryMessage, binary, bin_len);
            this->binaryMessage_size = bin_len;
            this->binaryMessage[bin_len] = '\0';
            delete[] binary;
            
            this->encryptionWaveform = this->cripto->generateWaveform(this->binaryMessage, bin_len);
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Campo editável para a mensagem em binário 
        ImGui::Text("Mensagem em Binário");
        ImGui::InputTextMultiline("##binary", this->binaryMessage, IM_ARRAYSIZE(this->binaryMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            size_t bin_len = this->binaryMessage_size > 0 ? this->binaryMessage_size : strlen(this->binaryMessage);
            char* encrypted = this->cripto->toChar(this->binaryMessage, bin_len);
            size_t enc_len = bin_len / 8;
            memcpy(this->encryptedMessage, encrypted, enc_len);
            this->encryptedMessage_size = enc_len;
            delete[] encrypted;
            
            char* decrypted = this->cripto->decrypt(this->encryptedMessage, enc_len);
            strcpy(this->originalMessage, decrypted);
            delete[] decrypted;
            
            this->encryptionWaveform = this->cripto->generateWaveform(this->binaryMessage, bin_len);
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Gráfico do processo de montagem
        ImGui::Text("Forma de Onda (Codificação)");
        if (!this->encryptionWaveform.empty()) {
            ImGui::PlotLines("##plot_enc", this->encryptionWaveform.data(), (int)this->encryptionWaveform.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(-1.0f, 100));
        } else {
            ImGui::Text("Gráfico gerado após a criptografia.");
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Status da conexão
        if (this->sender_connected) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Conectado ao Receiver");
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Desconectado - Receiver não disponível");
        }

        // Botão de Enviar
        if (ImGui::Button("Enviar")) {
            if (this->sender_connected) {
                // Envia binaryMessage com o tamanho correto (binaryMessage_size)
                std::string payload(this->binaryMessage, this->binaryMessage_size);
                this->sender_socket->sendData(payload);
                ImGui::OpenPopup("Mensagem Enviada!");
            } else {
                ImGui::OpenPopup("Falha ao conectar com Receiver");
            }
        }

        ImGui::End();

        this->render();
    }
}

void WindowManager::createReceiverWindow() {
    
    while (!glfwWindowShouldClose(this->window)) {
        glfwPollEvents();
        
        // Diminui o timer de notificação
        if (this->notification_timer > 0.0f) {
            this->notification_timer -= ImGui::GetIO().DeltaTime;
        }

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
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        
        // Mostrar notificação se recebeu mensagem recentemente
        if (this->notification_timer > 0.0f && this->message_received) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "✓ Mensagem Recebida!");
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Descrição gerado. Clique em 'Descriptografar' para visualizar.");
        } else if (this->message_received) {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Mensagem anterior recebida.");
        } else {
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.0f, 1.0f), "⏳ Aguardando mensagem...");
        }
        
        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        
        // Campo editável para a mensagem recebida (APENAS LEITURA)
        ImGui::Text("Mensagem Recebida (Binária)");
        ImGui::InputTextMultiline("##received", this->receivedMessage, IM_ARRAYSIZE(this->receivedMessage), 
            ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4), ImGuiInputTextFlags_ReadOnly);

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        
        // Gráfico do processo de desmontagem 
        ImGui::Text("Forma de Onda (Decodificação)");
        if (!this->decryptionWaveform.empty()) {
            ImGui::PlotLines("##plot_dec", this->decryptionWaveform.data(), (int)this->decryptionWaveform.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(-1.0f, 100));
        } else {
            ImGui::Text("Gráfico será gerado quando uma mensagem for recebida.");
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        
        // Campo para a mensagem criptografada (convertida de binário)
        ImGui::Text("Mensagem Criptografada");
        if (this->message_received) {
            // Se recebeu mensagem, mostra a mensagem criptografada
            size_t recv_len = this->receivedMessage_size > 0 ? this->receivedMessage_size : strlen(this->receivedMessage);
            char* from_binary = this->cripto->toChar(this->receivedMessage, recv_len);
            size_t enc_len = recv_len / 8;
            memcpy(this->encryptedMessage, from_binary, enc_len);
            this->encryptedMessage[enc_len] = '\0';
            delete[] from_binary;
        }
        ImGui::InputTextMultiline("##encrypted_receiver", this->encryptedMessage, IM_ARRAYSIZE(this->encryptedMessage), 
            ImVec2(-1.0f, ImGui::GetTextLineHeight() * 3), ImGuiInputTextFlags_ReadOnly);

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        
        // Botão de Descriptografar (desabilitado até receber mensagem)
        ImGui::BeginDisabled(!this->message_received);
        if (ImGui::Button("Descriptografar", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
            if (this->message_received) {
                // Descriptografa: 
                // 1. De binário (string de 0s e 1s) → char (texto criptografado)
                size_t recv_len = this->receivedMessage_size > 0 ? this->receivedMessage_size : strlen(this->receivedMessage);
                char* from_binary = this->cripto->toChar(this->receivedMessage, recv_len);
                size_t enc_len = recv_len / 8;
                
                // 2. De char criptografado → texto original (usando XOR)
                char* decrypted_text = this->cripto->decrypt(from_binary, enc_len);
                
                strcpy(this->decryptedMessage, decrypted_text);
                this->message_decrypted = true;
                
                // Limpar memória alocada
                delete[] from_binary;
                delete[] decrypted_text;
            }
        }
        ImGui::EndDisabled();

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        
        // Campo editável para a mensagem descriptografada (APENAS LEITURA)
        ImGui::Text("Mensagem Descriptografada");
        ImGui::InputTextMultiline("##decrypted", this->decryptedMessage, IM_ARRAYSIZE(this->decryptedMessage), 
            ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4), ImGuiInputTextFlags_ReadOnly);
        
        if (!this->message_decrypted && this->message_received) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "ℹ Clique em 'Descriptografar' para visualizar a mensagem original.");
        }

        ImGui::End();

        this->render();
    }
}

void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}