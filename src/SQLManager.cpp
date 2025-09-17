#include <iostream>
#include <sqlite3.h>
#include "../headers/SQLManager.h"

int SQL::init()
{
    cont = true;
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("databases/users.db", &DB);

    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return (-1);
    }
    else
        std::cout << "Opened Database Successfully!" << std::endl;
    sqlite3_close(DB);
    return 0;
}

void SQL::loop() {
    std::string user;
    std::cout << "ENTER USERNAME\n";
    std::cin >> user;

    std::string pass;
    std::cout << "ENTER PASSWORD\n";
    std::cin >> pass;

    std::string c;
    std::cout << "Do You Want To Go Again, Y yes, N no\n";
    std::cin >> c;

    
    if (c == "N") {
        cont = false;
    }
    else if (c != "Y") {
        std::cout << "INVALID\n";
    }

}