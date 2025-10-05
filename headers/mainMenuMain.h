#pragma once

#ifndef MAIN_MENU_MAIN_H
#define MAIN_MENU_MAIN_H

class mainMenuClass {
public:
	int mainMenuCall();
    
    enum states {
        main,
        optionsMenu,
        loginMenu,
        saveLoadMenu,
        signUp,
        accountData,
        testing,
        null
    };

    states currentState;
private:
	
};


#endif