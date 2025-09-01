#include "../headers/mainMenuMain.h"
#include "../headers/windowManager.h"


int mainMenuClass::mainMenuCall() {
	//windowManager window_Manager;
	window_Manager.activateGLFW();
	GLFWwindow* window = window_Manager.createWindow(400, 400, "MAIN MENU");
	while (!glfwWindowShouldClose(window)) {
		window_Manager.showFPS();
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
