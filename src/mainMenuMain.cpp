#include "../headers/mainMenuMain.h"
#include "../headers/windowManager.h"


int mainMenuClass::mainMenuCall(){
	//windowManager window_Manager;
	window_Manager.activateGLFW();
	if (!window_Manager.glfwActive()) {
		window_Manager.createWindow();
	}


	return 0;
}
