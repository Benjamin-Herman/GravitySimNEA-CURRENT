#include "../headers/gravityMain.h"
#include "../headers/mainMenuMain.h"

void mainMenu();
void gravityWindow();
void userInterface();

int main() {
    gravityWindow();
    return 0;
}

void mainMenu(){
    mainMenuClass main_Menu;
    main_Menu.mainMenuCall();
}

void gravityWindow(){
    gravityMain gCntr;
    gCntr.gravitySimMain();
}

void userInterface(){

}