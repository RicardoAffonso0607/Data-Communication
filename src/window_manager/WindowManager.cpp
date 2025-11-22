#include "../../include/window_manager/WindowManager.h"

// GLFW callback function prototypes
void glfwErrorCallback(int error, const char* description);

WindowManager::WindowManager() {

    const char* key = "mykey123";
    size_t keyLength = strlen(key);
    this->crypto = new Crypto(key, keyLength);

    // Load configuration from config.txt file
    Config config;
    config.print();
    std::string receiver_ip = config.get("RECEIVER_IP", "127.0.0.1");
    int port = config.getInt("PORT", 4444);

    this->sender_socket = new Socket(receiver_ip, port);
    this->receiver_socket = new Socket(receiver_ip, port);
    this->sender_connected = false;
    this->message_received = false;
    this->message_decrypted = false;
    this->notification_timer = 0.0f;
    
    // Set callback for when a message is received
    this->receiver_socket->setMessageCallback([this](const std::string& data) {
        // data contains binary data, not necessarily a null-terminated string
        memcpy(this->receivedMessage, data.data(), data.size());
        this->receivedMessage_size = data.size();
        this->message_received = true;
        this->message_decrypted = false;
        this->notification_timer = 5.0f; // Notification for 5 seconds
        
        // Generate waveform from received binary message
        this->decryptionWaveform = this->crypto->generateWaveform(this->receivedMessage, data.size());
        
        // Generate encoding graphics
        std::string binary_str(this->receivedMessage, this->receivedMessage_size);
        this->receiverNRZ_Waveform = Codificacao::gerar(binary_str, Codificacao::NRZ);
        this->receiverRZ_Waveform = Codificacao::gerar(binary_str, Codificacao::RZ);
    });
    
    thread server_thread(&Socket::run_receiver_server, this->receiver_socket);
    server_thread.detach();  // Detach the thread to run independently
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
    if (this->crypto) {
        delete this->crypto;
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
            // Carrega configuração para obter IP e porta do receiver
            Config config;
            std::string receiver_ip = config.get("RECEIVER_IP", "127.0.0.1");
            int port = config.getInt("PORT", 4444);
            
            if (this->sender_socket->connectToReceiver(receiver_ip, port)) {
                this->sender_connected = true;
                std::cout << "✅ Conectado ao Receiver em " << receiver_ip << ":" << port << std::endl;
            } else {
                std::cout << "❌ Falha ao conectar ao Receiver em " << receiver_ip << ":" << port << std::endl;
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

        // Input field for original message
        ImGui::InputTextMultiline("Original Message", this->originalMessage, IM_ARRAYSIZE(this->originalMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            size_t msg_len = strlen(this->originalMessage);
            char* encrypted = this->crypto->encrypt(this->originalMessage, msg_len);
            memcpy(this->encryptedMessage, encrypted, msg_len);
            this->encryptedMessage_size = msg_len;
            delete[] encrypted;
            
            char* binary = this->crypto->toBinary(this->encryptedMessage, msg_len);
            size_t bin_len = msg_len * 8;
            memcpy(this->binaryMessage, binary, bin_len);
            this->binaryMessage_size = bin_len;
            this->binaryMessage[bin_len] = '\0';
            delete[] binary;
            
            this->encryptionWaveform = this->crypto->generateWaveform(this->binaryMessage, bin_len);
            
            // Update encoding graphics
            std::string binary_str(this->binaryMessage, this->binaryMessage_size);
            this->senderNRZ_Waveform = Codificacao::gerar(binary_str, Codificacao::NRZ);
            this->senderRZ_Waveform = Codificacao::gerar(binary_str, Codificacao::RZ);
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Editable field for encrypted message
        ImGui::Text("Encrypted Message");
        bool encrypted_edited = ImGui::InputTextMultiline("##encrypted", this->encryptedMessage, IM_ARRAYSIZE(this->encryptedMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        if (encrypted_edited) {
            // Update size based on what was typed (real-time)
            this->encryptedMessage_size = strlen(this->encryptedMessage);
            
            if (this->encryptedMessage_size > 0) {
                char* decrypted = this->crypto->decrypt(this->encryptedMessage, this->encryptedMessage_size);
                memset(this->originalMessage, 0, MESSAGE_BUF_SIZE);
                strcpy(this->originalMessage, decrypted);
                delete[] decrypted;
                
                char* binary = this->crypto->toBinary(this->encryptedMessage, this->encryptedMessage_size);
                size_t bin_len = this->encryptedMessage_size * 8;
                memcpy(this->binaryMessage, binary, bin_len);
                this->binaryMessage_size = bin_len;
                this->binaryMessage[bin_len] = '\0';
                delete[] binary;
                
                this->encryptionWaveform = this->crypto->generateWaveform(this->binaryMessage, bin_len);
                
                // Update encoding graphics
                std::string binary_str(this->binaryMessage, this->binaryMessage_size);
                this->senderNRZ_Waveform = Codificacao::gerar(binary_str, Codificacao::NRZ);
                this->senderRZ_Waveform = Codificacao::gerar(binary_str, Codificacao::RZ);
            }
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Editable field for binary message
        ImGui::Text("Binary Message");
        bool binary_edited = ImGui::InputTextMultiline("##binary", this->binaryMessage, IM_ARRAYSIZE(this->binaryMessage), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4));
        if (binary_edited) {
            // Update size based on what was typed (real-time)
            this->binaryMessage_size = strlen(this->binaryMessage);
            
            if (this->binaryMessage_size > 0 && this->binaryMessage_size % 8 == 0) {
                char* encrypted = this->crypto->toChar(this->binaryMessage, this->binaryMessage_size);
                size_t enc_len = this->binaryMessage_size / 8;
                memcpy(this->encryptedMessage, encrypted, enc_len);
                this->encryptedMessage_size = enc_len;
                delete[] encrypted;
                
                char* decrypted = this->crypto->decrypt(this->encryptedMessage, enc_len);
                memset(this->originalMessage, 0, MESSAGE_BUF_SIZE);
                strcpy(this->originalMessage, decrypted);
                delete[] decrypted;
                
                this->encryptionWaveform = this->crypto->generateWaveform(this->binaryMessage, this->binaryMessage_size);
                
                // Update encoding graphics
                std::string binary_str(this->binaryMessage, this->binaryMessage_size);
                this->senderNRZ_Waveform = Codificacao::gerar(binary_str, Codificacao::NRZ);
                this->senderRZ_Waveform = Codificacao::gerar(binary_str, Codificacao::RZ);
            }
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Graph of assembly process
        ImGui::Text("Waveform (Encryption)");
        if (!this->encryptionWaveform.empty()) {
            ImGui::PlotLines("##plot_enc", this->encryptionWaveform.data(), (int)this->encryptionWaveform.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(-1.0f, 100));
        } else {
            ImGui::Text("Graph will be generated after encryption.");
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // New Encoding Graph (NRZ vs RZ)
        ImGui::Text("Line Encoding");
        ImGui::Text("Algorithm: %s", Codificacao::getTypeName(this->senderActiveEncoding));
        
        // Checkbox to switch between NRZ and RZ
        bool use_nrz = (this->senderActiveEncoding == Codificacao::NRZ);
        if (ImGui::Checkbox("Use NRZ##sender", &use_nrz)) {
            this->senderActiveEncoding = use_nrz ? Codificacao::NRZ : Codificacao::RZ;
        }
        
        ImGui::SameLine();
        ImGui::Text("(uncheck for RZ)");
        
        // Show corresponding graph
        if (!this->binaryMessage_size == 0) {
            const std::vector<float>& active_waveform = 
                (this->senderActiveEncoding == Codificacao::NRZ) ? 
                this->senderNRZ_Waveform : this->senderRZ_Waveform;
            
            if (!active_waveform.empty()) {
                ImGui::PlotLines("##plot_codificacao", active_waveform.data(), (int)active_waveform.size(), 0, nullptr, -1.5f, 1.5f, ImVec2(-1.0f, 150));
            } else {
                ImGui::Text("Graph will be generated after encoding.");
            }
        } else {
            ImGui::Text("No binary data to encode.");
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // Send button
        if (ImGui::Button("Send")) {
            if (this->sender_connected) {
                // Sends binaryMessage with the correct size (binaryMessage_size)
                std::string payload(this->binaryMessage, this->binaryMessage_size);
                this->sender_socket->sendData(payload);
                ImGui::OpenPopup("Message Sent!");
            } else {
                ImGui::OpenPopup("Failed to connect with Receiver");
            }
        }

        ImGui::End();

        this->render();
    }
}

void WindowManager::createReceiverWindow() {
    
    while (!glfwWindowShouldClose(this->window)) {
        glfwPollEvents();
        
        // Decrease notification timer
        if (this->notification_timer > 0.0f) {
            this->notification_timer -= ImGui::GetIO().DeltaTime;
        }

        // Start a new frame of ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create the main project window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Crypto-Com", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        
        // --- Receiver Side (Reception Process) ---
        ImGui::Text("Receiver Side (Reception)");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        
        // Show notification if received message recently
        if (this->notification_timer > 0.0f && this->message_received) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "✓ Message Received!");
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Description generated. Click 'Decrypt' to view.");
        } else if (this->message_received) {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Previous message received.");
        } else {
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.0f, 1.0f), "⏳ Waiting for message...");
        }
        
        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        
        // Editable field for received message (READ ONLY)
        ImGui::Text("Received Message (Binary)");
        ImGui::InputTextMultiline("##received", this->receivedMessage, IM_ARRAYSIZE(this->receivedMessage), 
            ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4), ImGuiInputTextFlags_ReadOnly);

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        
        // Graph of disassembly process
        ImGui::Text("Waveform (Decryption)");
        if (!this->decryptionWaveform.empty()) {
            ImGui::PlotLines("##plot_dec", this->decryptionWaveform.data(), (int)this->decryptionWaveform.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(-1.0f, 100));
        } else {
            ImGui::Text("Graph will be generated when a message is received.");
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        // New Encoding Graph (NRZ vs RZ) - RECEIVER
        ImGui::Text("Line Encoding (Received)");
        ImGui::Text("Algorithm: %s", Codificacao::getTypeName(this->receiverActiveEncoding));
        
        // Checkbox to switch between NRZ and RZ
        bool use_nrz_receiver = (this->receiverActiveEncoding == Codificacao::NRZ);
        if (ImGui::Checkbox("Use NRZ##receiver", &use_nrz_receiver)) {
            this->receiverActiveEncoding = use_nrz_receiver ? Codificacao::NRZ : Codificacao::RZ;
        }
        
        ImGui::SameLine();
        ImGui::Text("(uncheck for RZ)");
        
        // Show corresponding graph
        if (this->message_received && this->receivedMessage_size > 0) {
            const std::vector<float>& active_waveform = 
                (this->receiverActiveEncoding == Codificacao::NRZ) ? 
                this->receiverNRZ_Waveform : this->receiverRZ_Waveform;
            
            if (!active_waveform.empty()) {
                ImGui::PlotLines("##plot_codificacao_receiver", active_waveform.data(), (int)active_waveform.size(), 0, nullptr, -1.5f, 1.5f, ImVec2(-1.0f, 150));
            } else {
                ImGui::Text("Graph will be generated after encoding.");
            }
        } else {
            ImGui::Text("Waiting for message to encode.");
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        
        // Field for encrypted message (converted from binary)
        ImGui::Text("Encrypted Message");
        if (this->message_received) {
            // If received message, show encrypted message
            size_t recv_len = this->receivedMessage_size > 0 ? this->receivedMessage_size : strlen(this->receivedMessage);
            char* from_binary = this->crypto->toChar(this->receivedMessage, recv_len);
            size_t enc_len = recv_len / 8;
            memcpy(this->encryptedMessage, from_binary, enc_len);
            this->encryptedMessage[enc_len] = '\0';
            delete[] from_binary;
        }
        ImGui::InputTextMultiline("##encrypted_receiver", this->encryptedMessage, IM_ARRAYSIZE(this->encryptedMessage), 
            ImVec2(-1.0f, ImGui::GetTextLineHeight() * 3), ImGuiInputTextFlags_ReadOnly);

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        
        // Decrypt button (disabled until message is received)
        ImGui::BeginDisabled(!this->message_received);
        if (ImGui::Button("Decrypt", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
            if (this->message_received) {
                // Decrypt: 
                // 1. From binary (string of 0s and 1s) → char (encrypted text)
                size_t recv_len = this->receivedMessage_size > 0 ? this->receivedMessage_size : strlen(this->receivedMessage);
                char* from_binary = this->crypto->toChar(this->receivedMessage, recv_len);
                size_t enc_len = recv_len / 8;
                
                // 2. From encrypted char → original text (using XOR)
                char* decrypted_text = this->crypto->decrypt(from_binary, enc_len);
                
                strcpy(this->decryptedMessage, decrypted_text);
                this->message_decrypted = true;
                
                // Clear allocated memory
                delete[] from_binary;
                delete[] decrypted_text;
            }
        }
        ImGui::EndDisabled();

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        
        // Editable field for decrypted message (READ ONLY)
        ImGui::Text("Decrypted Message");
        ImGui::InputTextMultiline("##decrypted", this->decryptedMessage, IM_ARRAYSIZE(this->decryptedMessage), 
            ImVec2(-1.0f, ImGui::GetTextLineHeight() * 4), ImGuiInputTextFlags_ReadOnly);
        
        if (!this->message_decrypted && this->message_received) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "ℹ Click 'Decrypt' to view the original message.");
        }

        ImGui::End();

        this->render();
    }
}

void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}