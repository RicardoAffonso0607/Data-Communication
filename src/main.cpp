#include "../include/window_manager/WindowManager.h"

// Header files do ImGui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// Header files do GLFW e GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>


int main(int argc, char* argv[]) {

    bool isSender = false;
    if (argc > 1) {
        string arg1 = argv[1]; 
        if (arg1 == "sender") {
            isSender = true;
        }
    } 

    WindowManager* windowManager = new WindowManager();

    if (isSender) {
        windowManager->createSenderWindow();
    } else {
        windowManager->createReceiverWindow();
    }

    windowManager->destroyWindow();

    return 0;
}
