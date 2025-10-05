#include "../headers/mainMenuMain.h"
#include "../headers/SQLManager.h"

void mainMenu();
void SQLCall();

int main() {
    mainMenu();
    //SQLCall();
    return 0;
}

void SQLCall() {
    SQL sql;
    sql.init();
    while (sql.cont) {
        sql.loop();
    }
}

void mainMenu(){
    mainMenuClass main_Menu;
    main_Menu.mainMenuCall();
    return;
}