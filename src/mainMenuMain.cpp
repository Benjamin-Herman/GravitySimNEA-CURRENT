#include "../headers/mainMenuMain.h"
#include "../headers/windowManager.h"
#include "../headers/GUI.h"

int mainMenuClass::mainMenuCall() {
    // Initialize window manager
    window_Manager.activateGLFW();
    GLFWwindow* window = window_Manager.createWindow(800, 600, "MAIN MENU");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable alpha blending for GUI text
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize GUI
    GUI* gui = new GUI(800, 600);
    gui->loadFont("fonts/Freedom-10eM.ttf"); // adjust path

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear default framebuffer for 3D scene
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 800, 600);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- PLACEHOLDER: render your 3D scene here ---
        // TODO: your 3D rendering code

        // Render GUI to its framebuffer
        gui->bindFramebuffer();
        gui->renderText("Hello GUI!", 50, 50, 1.0f, 1.0f, 1.0f, 1.0f);
        gui->unbindFramebuffer();

        // Draw GUI FBO texture on top of 3D scene
        glDisable(GL_DEPTH_TEST); // GUI on top
        glBindTexture(GL_TEXTURE_2D, gui->getTexture());
        glBindVertexArray(gui->getQuadVAO());
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete gui;
    glfwTerminate();
    return 0;
}
